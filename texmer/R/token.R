#' Segment text into segments with fixed number of tokens.
#'
#' @description
#' Segments each document in a corpus based on a fixed maximum number of tokens per segment.
#'
#' @param corpus A data frame containing columns 'id' and 'text'
#' @param seg_size An integer determining the maximum number of tokens per segment
#'
#' @export
tf_token_seg <- function(corpus, seg_size, ...) {
    .check_input(corpus, seg_size)

    tokens <- corpus %>%
        texcur::tf_tokenize(token="words", ...) %>%
        dplyr::mutate(docid=id)

    seg_stats <- tokens %>%
        dplyr::group_by(id) %>%
        dplyr::summarise(ntokens=n(),
                         segsize=min(seg_size, ntokens),
                         nsegs=ceiling(ntokens / segsize)) %>%
        dplyr::ungroup() %>%
        dplyr::mutate(id=NULL)


    max_id <- 0

    gen_ids <- function(x) {
        ntokens <- x[1]
        segsize <- x[2]
        nsegs <- x[3]

        start_pos <- seq(1, ntokens, by=segsize)
        end_pos <- c(start_pos[-1], ntokens + 1)

        ids <- unlist(lapply(1:nsegs, function(id) {
            rep(id, end_pos[id] - start_pos[id])
        })) + max_id

        max_id <<- max(ids)

        ids
    }

    ids <- as.character(unlist(apply(seg_stats, 1, gen_ids)))
    tokens$id <- ids

    tokens %>%
        texcur::tf_merge_tokens(delim=" ")
}

.check_input <- function(corpus, seg_size) {
    checkr::assert_type(corpus, "tbl_df")
    checkr::assert_subset(c("id", "text"), names(corpus))
    ## checkr::assert_factor(corpus$id)
    checkr::assert_character(corpus$text)
    checkr::assert_integer(seg_size, lower=1)
}
