#' @param sentence_size The Pseudo-sentence size
#' @param block_size The window size
#'
#' @export
tf_texttile <- function(corpus, sentence_size, block_size) {

}

tf_texttile_doc <- function(document, stopwords,
                            sentence_size, block_size,
                            method="block",
                            paragraph_breakpoint="\n",
                            liberal_depth_cutoff=TRUE) {
    checkr::assert_string(document)
    checkr::assert_integer(sentence_size, lower=1)
    checkr::assert_integer(block_size, lower=1)
    checkr::assert_character(stopwords)
    checkr::assert_subset(method, c("block", "vocabulary"))
    checkr::assert_string(paragraph_breakpoint)


    paragraph_breaks <- .find_paragraph_breakpoints(document, paragraph_breakpoint)
    tokens <- .get_tokens(document)
    token_sequences <- .get_token_sequences(tokens, sentence_size, stopwords)

    if (method == "block") {
        lexical_scores <- .calculate_block_scores(token_sequences, block_size)
    } else {
        lexical_scores <- .calculate_vocabulary_introduction_scores(token_sequences, sentence_size)
    }

    gap_boundaries <- .find_gap_boundaries(lexical_scores, liberal_depth_cutoff)

    if (length(gap_boundaries) == 0) {
        warning("Could not find any boundaries. Returning the original document.")
        return(document)
    }

    token_boundaries <- .convert_gap_boundaries_to_token_boundaries(gap_boundaries, sentence_size)
    .construct_segmented_document(tokens, token_boundaries)
}

#' @keywords internal
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

#' @keywords internal
.get_tokens <- function(document) {
    df_doc <- dplyr::data_frame(text=document)
    tokens <- df_doc %>%
        texcur::tf_lowercase() %>%
        texcur::tf_tokenize()

    tokens
}

#' @keywords internal
.get_token_sequences <- function(tokens, w, stopwords) {
    df_stopwords <- dplyr::data_frame(token=stopwords)

    n <- nrow(tokens)
    r <- rep(1:floor(n / w), each=w)[1:n]

    token_sequences <- split(tokens, r)

    for (i in seq_along(token_sequences)) {
        token_sequences[[i]] <- token_sequences[[i]] %>%
            dplyr::count(token) %>%
            dplyr::anti_join(df_stopwords, by="token")
    }

    token_sequences
}

#' TODO: Fix so that it calculates scores for all token_sequences (DON'T START AT BLOCK_SIZE but at 1)
#' @keywords internal
.calculate_block_scores <- function(token_sequences, block_size) {
    create_token_sequence_from_block <- function(block) {
        join_token_sequence <- function(ts1, ts2) {
            ts1 %>%
                dplyr::full_join(ts2, by="token") %>%
                dplyr::rowwise() %>%
                dplyr::mutate(n=sum(n.x, n.y, na.rm=TRUE),
                              n.x=NULL,
                              n.y=NULL) %>%
                dplyr::ungroup()
        }

        token_sequence <- block[[1]]

        for (seq_idx in 2:length(block)) {
            token_sequence <- token_sequence %>%
                join_token_sequence(block[[seq_idx]])
        }

        token_sequence
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
    gap_idxs <- block_size:(n_ts - block_size)

    scores <- rep(NA, length(gap_idxs))

    for (gap_idx in gap_idxs) {
        left_idx <- gap_idx - block_size + 1
        right_idx <- gap_idx + block_size

        left_block <- token_sequences[left_idx:gap_idx]
        right_block  <- token_sequences[(gap_idx + 1):right_idx]

        left_token_sequence <- create_token_sequence_from_block(left_block)
        right_token_sequence <- create_token_sequence_from_block(right_block)

        numerator <- calculate_numerator(left_token_sequence,
                                         right_token_sequence)

        denominator <- calculate_denominator(left_token_sequence,
                                             right_token_sequence)

        score <- numerator / denominator

        scores[gap_idx - block_size + 1] <- score
    }

    scores
}

#' @keywords internal
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

#' @keywords internal
.find_gap_boundaries <- function(lexical_scores, liberal) {
    cutoff_score <- .calculate_depth_cutoff_score(lexical_scores, liberal)
    boundaries <- c()

    for (i in seq_along(lexical_scores)) {
        score <- lexical_scores[i]

        left_depth_score <- .calculate_depth_score_by_side(lexical_scores, i, TRUE)
        right_depth_score <- .calculate_depth_score_by_side(lexical_scores, i, FALSE)

        depth_score <- left_depth_score + right_depth_score

        if (depth_score >= cutoff_score) {
            boundaries <- c(boundaries, i)
        }
    }

    boundaries
}

#' @keywords internal
.calculate_depth_cutoff_score <- function(lexical_scores, liberal=TRUE) {
    avg <- mean(lexical_scores)
    stdev <- sd(lexical_scores)

    if (liberal) {
        avg - stdev
    } else {
        avg - stdev / 2
    }
}

#' @keywords internal
.calculate_depth_score_by_side <- function(lexical_scores, gap, left=TRUE) {
    depth_score <- 0
    current_gap <- gap

    while ((lexical_scores[current_gap] - lexical_scores[gap]) >= depth_score) {
        depth_score <- lexical_scores[current_gap] - lexical_scores[gap]

        if (left) {
            current_gap <- current_gap - 1
        } else {
            current_gap <- current_gap + 1
        }

        if ((current_gap < 1 & left) | (current_gap > length(lexical_scores) & !left)) {
            break
        }
    }

    depth_score
}

#' @keywords internal
.convert_gap_boundaries_to_token_boundaries <- function(gap_boundaries, sentence_size) {
    gap_boundaries * sentence_size
}

#' @keywords internal
.construct_segmented_document <- function(tokens, token_boundaries) {
    n <- length(token_boundaries) + 1

    ids <- unlist(lapply(1:n, function(idx) {
        if (idx == 1) {
            rep(idx, token_boundaries[idx])
        } else if (idx == n) {
            rep(idx, nrow(tokens) - token_boundaries[idx - 1])
        } else {
            rep(idx, token_boundaries[idx] - token_boundaries[idx - 1])
        }
    }))

    tokens$id <- as.character(ids)

    tokens %>%
        texcur::tf_merge_tokens()
}

head(tf_texttile_doc(document, c("the", "is", "that", "in", "of"), 15, 2, method="vocabulary")$text)

segments[[1]] %>%
    dplyr::full_join(segments[[2]], by="token") %>%
    dplyr::rowwise() %>%
    dplyr::mutate(n=sum(n.x, n.y, na.rm=TRUE),
                  n.x=NULL,
                  n.y=NULL) %>%
    dplyr::ungroup()


segments[[1]] %>%
    dplyr::summarize(n=sum(n))

stopwords <- dplyr::data_frame(token=c("the", "is", "that"))


document <- "Campylobacteriosis remains the most commonly reported bacterial foodborne disease in humans worldwide . The incidence of campylobacteriosis has risen recently, with more than 200,000 confirmed cases in the European Union reported each year , .  () infections are clinically manifested by diarrhoea, fever, and abdominal cramps, and, in certain cases, may be followed by long-term sequelae such as Guillain-Barr syndrome or reactive arthritis . Although it is generally agreed that the major source of human infections is contaminated poultry meat, there is evidence that surface and/or drinking water also acts as a vehicle for  and  transmissions to humans , , , , . Rivers or other natural aquatic environments can be contaminated with thermotolerant  by raw sewage, discharge from wastewater-treated agricultural land, or faeces from wild or domestic animals , , , , .



Assessments of microbiological water quality generally focus on testing for indicator bacteria, like , which are used to estimate the exposure of drinking water to faecal contamination , . Nevertheless, the onset of human  infections or outbreaks may necessitate the inclusion of thermotolerant  in microbial water analyses , . Water analysis laboratories often use the ISO standard method 17995:2005 , which initially includes a filtration step of the water samples, followed by bacterial enrichment and cultivation on selective agar plates. However, the ISO method is time consuming and often fails to detect  from water samples . This may be the result of higher amounts of injured or viable but nonculturable (VBNC) cells under stressful environmental conditions, as is the case in water . To circumvent the limitations of culture-based methods, quantitative real-time PCR (qPCR) approaches for the detection of  from water have been developed and successfully applied , , , . However, the lack of differentiation between DNA from viable and nonviable cells restricts the implementation of these PCR-based techniques for routine diagnostic applications . Recently, a sample pretreatment with intercalating dyes like ethidium monoazide (EMA) or propidium monoazide (PMA) was proposed to address this problem. These dyes cross the membranes of damaged cells, covalently bind to DNA after photoactivation, and, thus, block PCR amplification of DNA from nonviable cells . EMA- or PMA-qPCR assays have been successfully used to quantify  from poultry products , ,  but little is known about the applicability of a viability qPCR to determine the quantities of  from water samples , .



Therefore, the objectives of this study were to assess the general suitability of an EMA-qPCR method for the quantification of  cells from water samples. For this purpose, two methods for the recovery of cells from water samples were comparatively analysed, and live/dead ratios after inoculation of cells in different type of water were considered."
