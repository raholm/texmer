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
    topictile_tokens <- topictile_tokens$tokens

    segments <- mode$topictile(topictile_tokens, stopwords,
                               sentence_size, block_size,
                               nsegments, liberal,
                               smoothing_rounds,
                               smoothing_width) %>%
        reshape2::melt() %>%
        dplyr::rename(text=value, docid=L1) %>%
        dplyr::mutate(id=as.character(row_number()),
                      text=as.character(text),
                      docid=as.character(docid)) %>%
        dplyr::as_data_frame()

    segments
}

.check_input_topictile <- function(corpus, stopwords, mode,
                                   sentence_size, block_size,
                                   nsegments, liberal,
                                   smoothing_rounds,
                                   smoothing_width) {
    checkr::assert_tidy_table(corpus, c("id", "token"))
    checkr::assert_character(corpus$id)
    checkr::assert_character(corpus$token)
    checkr::assert_character(stopwords)
    checkr::assert_integer(sentence_size, len=1, lower=1)
    checkr::assert_integer(block_size, len=1, lower=1)
    checkr::assert_integer(nsegments, len=1, lower=0)
    checkr::assert_logical(liberal, len=1)
    checkr::assert_integer(smoothing_rounds, len=1, lower=0)
    checkr::assert_integer(smoothing_width, len=1, lower=1)

    if (!(smoothing_width %% 2 == 0))
        stop("Smoothing width must be even.")
}
