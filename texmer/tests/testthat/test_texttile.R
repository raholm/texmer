test_that("tf_texttile_doc throws error when document contains less tokens than the sentence size", {
    document <- "hello world"
    stopwords <- c("is")
    sentence_size <- 3
    block_size <- 2

    expect_error(tf_texttile_doc(document, stopwords, sentence_size, block_size, method="vocabulary"))
})

test_that(".find_paragraph_breakpoints finds the correct breakpoints", {
    document <- "Hello, world!\nWhat are you doing tonight?\nIs that a fishstick?"
    breakpoint <- "\n"

    actual <- .find_paragraph_breakpoints(document, breakpoint)
    expected <- dplyr::data_frame(bp=as.integer(c(2, 7)))
    expect_equal(actual, expected)
})

test_that(".get_tokens returns correct tokens", {
    document <- "Hello, world!\nWhat are you doing tonight?\nIs that a fishstick?"

    actual <- .get_tokens(document)
    expected <- dplyr::data_frame(token=c("hello", "world",
                                          "what", "are", "you", "doing", "tonight",
                                          "is", "that", "a", "fishstick"))
    expect_equal(actual, expected)
})

test_that(".get_token_sequences returns the correct token sequences", {
    tokens <- dplyr::data_frame(token=c("is", "good",
                                        "what", "are", "that", "doing", "here", "here"))
    sentence_size <- 2
    stopwords <- c("is", "that")

    actual <- .get_token_sequences(tokens, sentence_size, stopwords)
    expected <- dplyr::lst("1"=dplyr::data_frame(token=c("good"), n=as.integer(c(1))),
                           "2"=dplyr::data_frame(token=c("what", "are"), n=as.integer(c(1, 1))),
                           "3"=dplyr::data_frame(token=c("doing"), n=as.integer(c(1))),
                           "4"=dplyr::data_frame(token=c("here"), n=as.integer(c(2))))
    expect_equal(actual, expected)
})

test_that(".calculate_block_scores calculates the scores correctly", {
    token_sequences <- dplyr::lst("1"=dplyr::data_frame(token=c("good"), n=as.integer(c(1))),
                                  "2"=dplyr::data_frame(token=c("what", "are", "doing"), n=as.integer(c(1, 1, 2))),
                                  "3"=dplyr::data_frame(token=c("good", "doing"), n=as.integer(c(2, 3))),
                                  "4"=dplyr::data_frame(token=c("here", "good", "what"), n=as.integer(c(2, 4, 3))))
    block_size <- 2

    token_sequences[2:2]

    .calculate_block_scores(token_sequences, block_size)


    actual <- .calculate_block_scores(token_sequences, block_size)
    expected <- c(
        0,
        (6 * 1 + 3 * 1 + 3 * 2) / sqrt(sum(c(1, 1, 1, 2)^2) * sum(c(6, 3, 2, 3)^2)),
        (2 * 4) / sqrt(sum(c(2, 3)^2) * sum(c(2, 4, 3)^2))
    )
    expect_equal(actual, expected)
})

test_that(".calculate_vocabulary_introduction_scores calculates the scores correctly", {
    token_sequences <- dplyr::lst("1"=dplyr::data_frame(token=c("good"), n=as.integer(c(1))),
                                  "2"=dplyr::data_frame(token=c("what", "are"), n=as.integer(c(1, 1))),
                                  "3"=dplyr::data_frame(token=c("good"), n=as.integer(c(1))),
                                  "4"=dplyr::data_frame(token=c("here"), n=as.integer(c(2))))
    sentence_size <- 2

    actual <- .calculate_vocabulary_introduction_scores(token_sequences, sentence_size)
    expected <- c(3, 2, 1, 1) / (2 * sentence_size)
    expect_equal(actual, expected)
})

test_that(".find_gap_boundaries finds the correct boundaries", {
    scores <- c(5, 2, 3, 1, 1, 4, 6, 4)

    actual <- .find_gap_boundaries(scores, TRUE)
    expected <- c(2, 4, 5, 6, 8)
    expect_equal(actual, expected)

    actual <- .find_gap_boundaries(scores, FALSE)
    expected <- c(2, 4, 5)
    expect_equal(actual, expected)
})

test_that(".calculate_depth_cutoff_score is calculating correctly", {
    scores <- c(1, 2, 3)

    actual <- .calculate_depth_cutoff_score(scores, TRUE)
    expected <- 2 - 1
    expect_equal(actual, expected)

    actual <- .calculate_depth_cutoff_score(scores, FALSE)
    expected <- 2 - 0.5
    expect_equal(actual, expected)
})


test_that(".calculate_depth_score_by_side calculates the score correctly", {
    scores <- c(5, 2, 3, 1, 1, 6, 7, 4)
    gap <- 4

    actual <- .calculate_depth_score_by_side(scores, gap, TRUE)
    expected <- 3 - 1
    expect_equal(actual, expected)

    actual <- .calculate_depth_score_by_side(scores, gap, FALSE)
    expected <- 7 - 1
    expect_equal(actual, expected)
})

test_that(".calculate_depth_score_by_side calculates the score correctly (boundary cases)", {
    scores <- c(5, 1, 4)
    gap <- 2

    actual <- .calculate_depth_score_by_side(scores, gap, TRUE)
    expected <- 5 - 1
    expect_equal(actual, expected)

    actual <- .calculate_depth_score_by_side(scores, gap, FALSE)
    expected <- 4 - 1
    expect_equal(actual, expected)
})

test_that(".convert_gap_boundaries_to_token_boudaries is converting correctly", {
    gap_boundaries <- c(2, 5, 9)
    sentence_size <- 5

    actual <- .convert_gap_boundaries_to_token_boundaries(gap_boundaries, sentence_size)
    expected <- gap_boundaries * sentence_size
    expect_equal(actual, expected)
})

test_that(".construct_segmented_document is constructing correctly", {
    tokens <- dplyr::data_frame(token=c("hello", "world",
                                        "what", "are", "you", "doing", "tonight",
                                        "is", "that", "a", "fishstick"))
    token_boundaries <- c(2, 7)

    actual <- .construct_segmented_document(tokens, token_boundaries)
    expected <- dplyr::data_frame(id=c("1", "2", "3"),
                                  text=c("hello world",
                                         "what are you doing tonight",
                                         "is that a fishstick"))
    expect_equal(actual, expected)
})
