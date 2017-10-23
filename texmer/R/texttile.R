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
#' Segmenting the document is equivalent to finding boundary points of subtopics in the document which is based on the lexical scores. The boundary identification algorithm assigns a depth score, the depth of the valley if one occurs, to each lexical score. A deep valley means that, based on the lexical score, there have been a change in topic at that point. The \code{liberal_depth_cutoff} is used to determine if the valley is deep enough for a boundary point. As the name suggests if it is set to 'TRUE' it will be more liberal. When the boundary points have been identified the document is segmented at those points.
#'
#' @param corpus A data frame with columns 'id' and 'token'.
#' @param stopwords A character vector of stopwords.
#' @param sentence_size The pseudo-sentence size, the number of tokens in a sentence. See 'Details' for more information.
#' @param block_size The block size as defined in the paper. See 'Details' for more information.
#' @param method The method for lexical scoring; currently are 'block' and 'vocabulary' supported.
#' @param liberal A logical. If 'TRUE' (the default) the depth cutoff will be more liberal. See 'Details' for more information.
#' @return A data frame with 'token' replaced by the segments in 'text'. 'id' contains the new ids and the old ids are preserved in 'docid'.
#'
#' @importFrom stats sd
#'
#' @export
tf_texttile <- function(corpus,
                        stopwords,
                        sentence_size,
                        block_size,
                        method="block",
                        liberal=TRUE,
                        smoothing_rounds=1,
                        smoothing_width=2) {
    .check_input_texttile(corpus, stopwords,
                          sentence_size ,block_size,
                          method, liberal, smoothing_rounds,
                          smoothing_width)

    texttile_tokens <- corpus %>%
        dplyr::group_by(id) %>%
        dplyr::summarise(tokens=list(token)) %>%
        dplyr::select(tokens)
    texttile_tokens <- texttile_tokens$tokens

    segments <- get_texttile_segments_cpp(texttile_tokens, stopwords,
                                          sentence_size, block_size,
                                          method, liberal,
                                          smoothing_rounds, smoothing_width) %>%
        reshape2::melt() %>%
        dplyr::rename(text=value, docid=L1) %>%
        dplyr::group_by(docid) %>%
        dplyr::mutate(id=row_number()) %>%
        dplyr::as_data_frame()

    segments
}

.check_input_texttile <- function(corpus,
                                  stopwords,
                                  sentence_size,
                                  block_size,
                                  method,
                                  liberal,
                                  smoothing_rounds,
                                  smoothing_width) {
    checkmate::assert_names(c("id", "token"), subset.of=names(corpus))
    checkmate::assert_character(stopwords)
    checkmate::assert_number(sentence_size, lower=1)
    checkmate::assert_number(block_size, lower=1)
    checkmate::assert_choice(method, c("block", "vocabulary"))
    checkmate::assert_logical(liberal, len=1)
    checkmate::assert_number(smoothing_rounds, lower=0)
    checkmate::assert_number(smoothing_width, lower=1)

    if (!(smoothing_width %% 2 == 0))
        stop("Smoothing width must be even.")
}
