context("token segmentation")

test_that("tf_token_seg raises an error for invalid input", {
    expect_error(tf_token_seg(data.frame(), 1))
    expect_error(tf_token_seg(dplyr::data_frame(), 1))
    expect_error(tf_token_seg(dplyr::data_frame(id=c("1", "2"), body=c("1", "2")), 1))
    expect_error(tf_token_seg(dplyr::data_frame(docid=c("1", "2"), token=c("1", "2")), 1))
    expect_error(tf_token_seg(dplyr::data_frame(id=c("1", "2"), token=c(1, 2)), 1))
    expect_error(tf_token_seg(dplyr::data_frame(id=c("1", "2"), token=c("a", "b")), 0))
    expect_error(tf_token_seg(dplyr::data_frame(id=c("1", "2"), token=c("a", "b")), ""))
})

test_that("tf_token_seg does not raise an error for valid input", {
    expect_message(tf_token_seg(dplyr::data_frame(id=c("1", "2"), token=c("1", "2")), 1), NA)
})

test_that("tf_token_seg segmentize corpus", {
    corpus <- dplyr::data_frame(id=c("1", "2", "3", "4"),
                                text=c("what are you doing out here friend",
                                       "this is not a stream of fresh water",
                                       "hello",
                                       "hello world")) %>% texcur::tf_tokenize()
    seg_size <- 2

    actual <- tf_token_seg(corpus, seg_size)
    expected <- dplyr::data_frame(id=as.character(1:10),
                                  docid=c(rep("1", 4), rep("2", 4), rep("3", 1), rep("4", 1)),
                                  text=c("what are", "you doing", "out here", "friend",
                                         "this is", "not a", "stream of", "fresh water",
                                         "hello",
                                         "hello world")) %>% dplyr::arrange(id)

    expect_equal(actual, expected)
})
