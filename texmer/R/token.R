#' Segment text into segments with fixed number of tokens.
#'
#' @description
#' Segments each document in a corpus based on a fixed maximum number of tokens per segment.
#'
#' @details
#' The input corpus is expected to have a column 'id' that is equal for each token of the same document, i.e., it identifies which document each token is part of.
#'
#' @param corpus A data frame containing columns 'id' and 'token'. See 'details' for more information.
#' @param seg_size An integer determining the maximum number of tokens per segment.
#' @return A data frame with 'token' replaced by the segments in 'text'. 'id' contains the new ids and the old ids are preserved in 'docid'.
#'
#' @export
tf_token_seg <- function(corpus, seg_size) {
    .check_input_token_seg(corpus, seg_size)

    tokens <- corpus %>%
        dplyr::mutate(docid=id)

    seg_stats <- tokens %>%
        dplyr::group_by(id) %>%
        dplyr::summarise(ntoken=n(),
                         segsize=min(seg_size, ntoken),
                         nseg=ceiling(ntoken / segsize)) %>%
        dplyr::ungroup() %>%
        dplyr::mutate(id=NULL)

    ids <- get_token_segment_ids_cpp(seg_stats$ntoken,
                                     seg_stats$nseg,
                                     seg_stats$segsize)
    tokens$id <- ids

    tokens %>%
        texcur::tf_merge_tokens(delim=" ") %>%
        dplyr::group_by(docid) %>%
        dplyr::mutate(id=row_number()) %>%
        dplyr::as_data_frame()
}

.check_input_token_seg <- function(corpus, seg_size) {
    checkmate::assert_names(c("id", "token"), subset.of=names(corpus))
    checkmate::assert_number(seg_size, lower=1)
    ## checkr::assert_character(corpus$id)
    ## checkr::assert_character(corpus$token)
    ## checkr::assert_integer(seg_size, len=1, lower=1)
}
