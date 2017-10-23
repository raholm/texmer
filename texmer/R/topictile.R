#' @title
#' Segmenting text based on Latent Dirichlet Allocation.
#'
#' @description
#' description here.
#'
#' @details
#' details here.
#'
#' @param corpus
#' @param stopwords
#' @param mode
#' @param sentence_size
#' @param block_size
#' @param nsegments
#' @param liberal
#' @return A data frame with 'token' replaced by the segments in 'text'. 'id' contains the new ids and the old ids are preserved in 'docid'.
#'
#' @export
tf_topictile <- function(corpus,
                         stopwords,
                         mode,
                         sentence_size=20,
                         block_size=2,
                         nsegments=0,
                         liberal=TRUE,
                         smoothing_rounds=1,
                         smoothing_width=2) {
    .check_input_topictile(corpus, stopwords, mode,
                           sentence_size, block_size,
                           nsegments, liberal,
                           smoothing_rounds,
                           smoothing_width)

    topictile_tokens <- corpus %>%
        dplyr::group_by(id) %>%
        dplyr::summarise(tokens=list(token)) %>%
        dplyr::select(tokens)
    topictile_tokens <- as.character(topictile_tokens$tokens)

    segments <- mode$topictile(topictile_tokens, stopwords,
                               sentence_size, block_size,
                               nsegments, liberal,
                               smoothing_rounds,
                               smoothing_width) %>%
        reshape2::melt() %>%
        dplyr::rename(text=value, docid=L1) %>%
        dplyr::group_by(docid) %>%
        dplyr::mutate(id=row_number()) %>%
        dplyr::as_data_frame()

    segments
}

.check_input_topictile <- function(corpus, stopwords, mode,
                                   sentence_size, block_size,
                                   nsegments, liberal,
                                   smoothing_rounds,
                                   smoothing_width) {
    checkmate::assert_names(c("id", "token"), subset.of=names(corpus))
    checkmate::assert_character(stopwords)
    checkmate::assert_number(sentence_size, lower=1)
    checkmate::assert_number(block_size, lower=1)
    checkmate::assert_number(nsegments, lower=0)
    checkmate::assert_logical(liberal, len=1)
    checkmate::assert_number(smoothing_rounds, lower=0)
    checkmate::assert_number(smoothing_width, lower=1)

    if (!(smoothing_width %% 2 == 0))
        stop("Smoothing width must be even.")
}
