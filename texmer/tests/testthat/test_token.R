test_that("token_seg raises an error for invalid input", {
    expect_error(token_seg(data.frame(), 1, NULL))
    expect_error(token_seg(dplyr::data_frame(), 1, NULL))
    expect_error(token_seg(dplyr::data_frame(body=c("a", "b")), 1, NULL))
    expect_error(token_seg(dplyr::data_frame(text=c(1, 2)), 1, NULL))
    expect_error(token_seg(dplyr::data_frame(text=c("a", "b")), 0, NULL))
    expect_error(token_seg(dplyr::data_frame(text=c("a", "b")), "", NULL))
    expect_error(token_seg(dplyr::data_frame(text=c("a", "b")), 1, data.frame()))
    expect_error(token_seg(dplyr::data_frame(text=c("a", "b")), 1, dplyr::data_frame()))
    expect_error(token_seg(dplyr::data_frame(text=c("a", "b")), 1, dplyr::data_frame(text=c("a", "b"))))
    expect_error(token_seg(dplyr::data_frame(text=c("a", "b")), 1, dplyr::data_frame(token=c(1, 2))))
})

test_that("token_seg segmentize corpus", {
    corpus <- dplyr::data_frame(id=c("1", "2", "3", "4"),
                                text=c("what are you doing out here friend",
                                       "this is not a stream of fresh water",
                                       "hello",
                                       "hello world"))

    actual <- token_seg(corpus, 2)
    expected <- dplyr::data_frame(id=as.character(1:10),
                                  docid=c(rep("1", 4), rep("2", 4), rep("3", 1), rep("4", 1)),
                                  text=c("what are", "you doing", "out here", "friend",
                                         "this is", "not a", "stream of", "fresh water",
                                         "hello",
                                         "hello world"))

    expect_equal(actual, expected)
})
