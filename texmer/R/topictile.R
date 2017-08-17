#' Segmenting text based on Latent Dirichlet Allocation.
#'
#' @export
tf_topictile <- function(corpus, stopwords, model, gibbs_iters=50,
                         sentence_size=20, block_size=2,
                         n_segments=0, liberal=TRUE) {
    .check_input_topictile(corpus, stopwords,
                           model, gibbs_iters,
                           sentence_size, block_size,
                           n_segments, liberal)

    mode <- .create_mode_topictile()

    for (i in 1:gibbs_iters) {
        samples <- .generate_samples_topictile(model)
        mode$update(samples$token, samples$topic)
    }

    tokens <- corpus %>%
        texcur::tf_lowercase() %>%
        texcur::tf_tokenize()

    topictile_tokens <- tokens %>%
        dplyr::group_by(id) %>%
        dplyr::summarise(tokens=list(token)) %>%
        dplyr::select(tokens)
    topictile_tokens <- topictile_tokens$tokens

    segments <- mode$topictile(topictile_tokens, stopwords,
                               sentence_size, block_size,
                               n_segments, liberal) %>%
        reshape2::melt() %>%
        dplyr::rename(text=value, docid=L1) %>%
        dplyr::mutate(id=as.character(row_number()),
                      text=as.character(text),
                      docid=as.character(docid)) %>%
        dplyr::as_data_frame()

    segments
}

.check_input_topictile <- function(corpus, stopwords,
                                   model, gibbs_iters,
                                   sentence_size, block_size,
                                   n_segments, liberal) {
    checkr::assert_tidy_table(corpus, c("id", "text"))
    checkr::assert_character(stopwords)
    checkr::assert_integer(sentence_size, len=1, lower=1)
    checkr::assert_integer(block_size, len=1, lower=1)
    checkr::assert_integer(n_segments, len=1, lower=0)
    checkr::assert_logical(liberal, len=1)
}

.get_module_topictile <- function() {
    Module("mod_topictile", "texmer")
}

.create_mode_topictile <- function() {
    module <- .get_module_topictile()
    mode <- module$TypeTopicIndicatorMode
    new(mode)
}

.generate_samples_topictile <- function(model) {
    filename <- "./mallet_state_file_topictile.txt"

    if (file.exists(filename)) file.remove(filename)

    model$writeState(filename)
    tbl <- read.table(filename)
    names(tbl) <- c("doc", "source", "pos", "type", "token", "topic")

    if (file.exists(filename)) file.remove(filename)

    list(token=as.character(tbl$token), topic=as.numeric(tbl$topic))
}
