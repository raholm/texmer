# Generated by using Rcpp::compileAttributes() -> do not edit by hand
# Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

get_texttile_segments_cpp <- function(tokens, stopwords, sentence_size, block_size, method, liberal, smoothing_rounds, smoothing_width) {
    .Call('_texmer_get_texttile_segments_cpp', PACKAGE = 'texmer', tokens, stopwords, sentence_size, block_size, method, liberal, smoothing_rounds, smoothing_width)
}

get_token_segment_ids_cpp <- function(n_tokens_per_doc, n_segs_per_doc, segsize_per_doc) {
    .Call('_texmer_get_token_segment_ids_cpp', PACKAGE = 'texmer', n_tokens_per_doc, n_segs_per_doc, segsize_per_doc)
}

run_testthat_tests <- function() {
    .Call('_texmer_run_testthat_tests', PACKAGE = 'texmer')
}

