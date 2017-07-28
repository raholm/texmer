#' @title Segmenting text into multi-paragraph subtopic passages.
#'
#' @description TextTiling is segmenting text into multi-paragraph subtopic passages.
#' It is based on the paper 'TextTiling: Segmenting Text into Multi-paragraph Subtopic Passages'
#' written by Marti A. Hearst.
#'
#' @details The algorithm can be broken down into three parts:
#' \enumerate{
#' \item Tokenization
#' \item Lexical Score Determination
#' \item Boundary Identification
#' }
#' The following sections will describe the different parts in detail.
#'
#' @section Tokenization:
#' Tokenization refers to the division of the input document to lexical units. The document is first transformed to lowercase, non-alphanumeric characters are removed, and then is converted into so called token-sequences based on the \code{sentence_size}. Stopwords are contributed to the sentence size but are removed from the token-sequences. Given the stopwords ('are', 'doing') and a sentence size of two, the document 'Hello World! What are you doing?' will be converted to the token-sequences ('hello world', 'what', 'you').
#'
#' @section Lexical Score Determination:
#' In order to find subtopics in the document, the algorithm uses different lexical scoring methods. Current implementation provides two different methods, 'Block' and 'Vocabulary'.
#'
#' The block method groups \code{block_size} many adjacent token-sequences and is meant to approximate the average paragraph size. Adjacent blocks are then compared for overall lexical similarity based on normalized inner product of two blocks. The actual numbers in the computation are the frequgencies of the tokens.
#'
#' The vocabulary method uses the vocabulary to find lexical similatiry between pairs of token-sequences. The lexical score is based on the number of new terms up to that point, i.e., occuring for the first time in the document, between adjacent token-sequences. That is if there are few new terms then the score will be low and vice-verse.
#'
#' @section Boundary Identification:
#' Segmenting the document is equivalent to finding boundary points of subtopics in the document which is based on the lexical scores. The boundary indentification algorithm assigns a depth score, the depth of the valley if one occurs, to each lexical score. A deep valley means that, based on the lexical score, there have been a change in topic at that point. The \code{liberal_depth_cutoff} is used to determine if the valley is deep enough for a boundary point. As the name suggests if it is set to 'TRUE' it will be more liberal. When the boundary points have been identified the document is segmented at those points.
#'
#' @param corpus A data frame
#' @param stopwords A character vector of stopwords.
#' @param sentence_size The pseudo-sentence size, the number of tokens in a sentence. See 'Details' for more information.
#' @param block_size The block size as defined in the paper. See 'Details' for more information.
#' @param method The method for lexical scoring; currently are 'block' and 'vocabulary' supported.
#' @param liberal_depth_cutoff A logical. If 'TRUE' (the default) the depth cutoff will be more liberal. See 'Details' for more information.
#'
#' @importFrom stats sd
#'
#' @export
tf_texttile <- function(corpus, stopwords,
                        sentence_size, block_size,
                        method="block",
                        liberal_depth_cutoff=TRUE) {
    .check_input_texttile(corpus, stopwords,
                          sentence_size ,block_size,
                          method, liberal_depth_cutoff)

    result <- dplyr::data_frame()

    for (i in 1:nrow(corpus)) {
        id <- corpus$id[i]
        document <- corpus$text[i]

        segments <- .tf_texttile_doc(document, stopwords,
                                     sentence_size, block_size,
                                     method, liberal_depth_cutoff)
        segments$docid <- id

        result <- result %>%
            rbind(segments)
    }

    result$id <- 1:nrow(result)
    result
}

.tf_texttile_doc <- function(document, stopwords,
                             sentence_size, block_size,
                             method="block",
                             liberal_depth_cutoff=TRUE) {
    tokens <- .get_tokens(document)

    if (sentence_size > nrow(tokens)) {
        warning("Document is too small for the specified sentence size. Returning the original document.")
        return(dplyr::data_frame(id="-1", text=document))
    }

    token_sequences <- .get_token_sequences(tokens, sentence_size, stopwords)

    if (length(token_sequences) < 2) {
        stop("Not enough token sequences.")
    }

    if (method == "block") {
        lexical_scores <- .calculate_block_scores(token_sequences, block_size)
    } else {
        lexical_scores <- .calculate_vocabulary_introduction_scores(token_sequences, sentence_size)
    }

    if (length(lexical_scores) < 2) {
        stop("Not enough lexical scores.")
    }

    gap_boundaries <- find_gap_boundaries_cpp(lexical_scores, liberal_depth_cutoff)

    if (length(gap_boundaries) == 0) {
        warning("Could not find any boundaries. Returning the original document.")
        return(dplyr::data_frame(id="-1", text=document))
    }

    token_boundaries <- .convert_gap_boundaries_to_token_boundaries(gap_boundaries, sentence_size)
    .construct_segmented_document(tokens, token_boundaries)
}

.find_paragraph_breakpoints <- function(document, breakpoint) {
    dplyr::data_frame(text=unlist(stringr::str_split(document, breakpoint))) %>%
        dplyr::filter(text != "") %>%
            dplyr::rowwise() %>%
            dplyr::mutate(bp=nrow(texcur::tf_tokenize(dplyr::data_frame(text=text))),
                          text=NULL) %>%
            dplyr::ungroup() %>%
            dplyr::mutate(bp=cumsum(bp)) %>%
            dplyr::filter(row_number() != n())
}

.get_tokens <- function(document) {
    df_doc <- dplyr::data_frame(text=document)
    tokens <- df_doc %>%
        texcur::tf_lowercase() %>%
        texcur::tf_tokenize()

    tokens
}

.get_token_sequences <- function(tokens, sentence_size, stopwords) {
    df_stopwords <- dplyr::data_frame(token=stopwords)

    n <- nrow(tokens)
    r <- rep(1:floor(n / sentence_size), each=sentence_size)[1:n]

    token_sequences <- split(tokens, r)

    for (i in seq_along(token_sequences)) {
        token_sequences[[i]] <- token_sequences[[i]] %>%
            dplyr::count(token) %>%
            dplyr::anti_join(df_stopwords, by="token")
    }

    token_sequences
}

.calculate_block_scores <- function(token_sequences, block_size) {
    create_token_sequence_from_block <- function(block) {
        merge_token_sequence <- function(ts1, ts2) {
            ts1 %>%
                dplyr::full_join(ts2, by="token") %>%
                dplyr::rowwise() %>%
                dplyr::mutate(n=sum(n.x, n.y, na.rm=TRUE),
                              n.x=NULL,
                              n.y=NULL) %>%
                dplyr::ungroup()
        }

        block %>%
            Reduce(merge_token_sequence, .)
    }

    calculate_numerator <- function(ts1, ts2) {
        numerator <- ts1 %>%
            dplyr::inner_join(ts2, by="token") %>%
            dplyr::mutate(n=n.x * n.y,
                          n.x=NULL,
                          n.y=NULL) %>%
            dplyr::ungroup() %>%
            dplyr::summarize(n=sum(n))

        numerator$n
    }

    calculate_denominator <- function(ts1, ts2) {
        left_sq_sum <- ts1 %>%
            dplyr::summarize(n=sum(n^2))

        right_sq_sum <- ts2 %>%
            dplyr::summarize(n=sum(n^2))

        sqrt(left_sq_sum$n * right_sq_sum$n)
    }

    n_ts <- length(token_sequences)
    gap_idxs <- 1:(n_ts - 1)

    scores <- c(rep(NA, n_ts - 1), 0)

    for (gap_idx in gap_idxs) {
        k <- min(gap_idx, block_size, n_ts - gap_idx)

        left_idx <- gap_idx - k + 1
        right_idx <- gap_idx + k

        left_block <- token_sequences[left_idx:gap_idx]
        right_block  <- token_sequences[(gap_idx + 1):right_idx]

        left_token_sequence <- create_token_sequence_from_block(left_block)
        right_token_sequence <- create_token_sequence_from_block(right_block)

        numerator <- calculate_numerator(left_token_sequence,
                                         right_token_sequence)

        denominator <- calculate_denominator(left_token_sequence,
                                             right_token_sequence)

        score <- numerator / denominator

        scores[gap_idx] <- score
    }

    scores
}

.calculate_vocabulary_introduction_scores <- function(token_sequences, sentence_size) {
    n_ts <- length(token_sequences)
    denom <- 2 * sentence_size

    scores <- rep(NA, n_ts)

    left_tokens_seen <- dplyr::data_frame(token="")
    right_tokens_seen <- dplyr::data_frame(token=token_sequences[[1]]$token)

    for (i in 1:(n_ts - 1)) {
        left_new_tokens <- token_sequences[[i]] %>%
            dplyr::anti_join(left_tokens_seen, by="token")

        right_new_tokens <- token_sequences[[i + 1]] %>%
            dplyr::anti_join(right_tokens_seen, by="token")

        score <- (nrow(left_new_tokens) + nrow(right_new_tokens)) / denom
        scores[i] <- score

        left_tokens_seen <- left_tokens_seen %>%
            dplyr::full_join(left_new_tokens, by="token")

        right_tokens_seen <- right_tokens_seen %>%
            dplyr::full_join(right_new_tokens, by="token")
    }

    ## Last score where we only look at the last token sequence
    right_new_tokens <- token_sequences[[n_ts]] %>%
        dplyr::anti_join(left_tokens_seen, by="token")
    scores[n_ts] <- nrow(right_new_tokens) / denom

    scores
}

.convert_gap_boundaries_to_token_boundaries <- function(gap_boundaries, sentence_size) {
    gap_boundaries * sentence_size
}

.construct_segmented_document <- function(tokens, token_boundaries) {
    ids <- as.character(generate_texttile_segment_ids_from_boundry_points_cpp(token_boundaries,
                                                                              nrow(tokens)));
    tokens$id <- ids
    tokens %>%
        texcur::tf_merge_tokens()
}

.check_input_texttile <- function(corpus, stopwords,
                                  sentence_size, block_size,
                                  method, liberal_depth_cutoff) {
    checkr::assert_tidy_table(corpus, c("id", "text"))
    checkr::assert_character(stopwords)
    checkr::assert_integer(sentence_size, len=1, lower=1)
    checkr::assert_integer(block_size, len=1, lower=1)
    checkr::assert_choice(method, c("block", "vocabulary"))
    checkr::assert_logical(liberal_depth_cutoff, len=1)
}

## run_testthat_tests()
## tokensequence_test()
