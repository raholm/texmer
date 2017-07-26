#' Segment text into segments with fixed number of tokens.
#'
#' @description
#' Segments each document in a corpus based on a fixed maximum number of tokens per segment.
#'
#' @param corpus A data frame containing columns 'id' and 'text'
#' @param seg_size An integer determining the maximum number of tokens per segment
#' @param token The definition of token. See \code{\link[tidytext]{unnest_tokens}} for options.
#' @param ... Options to \code{\link[tidytext]{unnest_tokens}}.
#'
#' @export
tf_token_seg <- function(corpus, seg_size, token="words", ...) {
    .check_input_token_seg(corpus, seg_size)

    tokens <- corpus %>%
        texcur::tf_tokenize(token=token, ...) %>%
        dplyr::mutate(docid=id)

    seg_stats <- tokens %>%
        dplyr::group_by(id) %>%
        dplyr::summarise(ntoken=n(),
                         segsize=min(seg_size, ntoken),
                         nseg=ceiling(ntoken / segsize)) %>%
        dplyr::ungroup() %>%
        dplyr::mutate(id=NULL)

    ids <- as.character(generate_token_segment_ids_cpp(seg_stats$ntoken,
                                                       seg_stats$nseg,
                                                       seg_stats$segsize))
    tokens$id <- ids

    tokens %>%
        texcur::tf_merge_tokens(delim=" ")
}

.check_input_token_seg <- function(corpus, seg_size) {
    checkr::assert_type(corpus, "tbl_df")
    checkr::assert_subset(c("id", "text"), names(corpus))
    ## checkr::assert_factor(corpus$id)
    checkr::assert_character(corpus$text)
    checkr::assert_integer(seg_size, lower=1)
}
