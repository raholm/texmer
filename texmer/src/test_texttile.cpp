#include <testthat.h>

#include "texttile.h"

bool integer_vector_equality(const Rcpp::IntegerVector& l,
                             const Rcpp::IntegerVector& r) {
  for (unsigned i = 0; i < l.size(); ++i) {
    if (!(l[i] == r[i])) return false;
  }
  return true;
}

context("Texttile segmentation routines") {
  test_that("find_gap_boundaries_cpp finds the correct boundaries (left side)") {
    Rcpp::NumericVector scores = Rcpp::NumericVector::create(5, 2, 3, 1, 1, 4, 6, 4);

    Rcpp::IntegerVector actual = find_gap_boundaries_cpp(scores, true);
    Rcpp::IntegerVector expected = Rcpp::IntegerVector::create(2, 4, 5, 6, 8);
    expect_true(integer_vector_equality(actual, expected));
  }

  test_that("find_gap_boundaries_cpp finds the correct boundaries (right side)") {
    Rcpp::NumericVector scores = Rcpp::NumericVector::create(5, 2, 3, 1, 1, 4, 6, 4);

    Rcpp::IntegerVector actual = find_gap_boundaries_cpp(scores, false);
    Rcpp::IntegerVector expected = Rcpp::IntegerVector::create(2, 4, 5);
    expect_true(integer_vector_equality(actual, expected));
  }

  test_that("get_depth_cutoff_score is calculating correctly (liberal)") {
    Rcpp::NumericVector scores = Rcpp::NumericVector::create(1, 2, 3);

    double actual = get_depth_cutoff_score(scores, true);
    double expected = 2 - 1;
    expect_true(actual == expected);
  }

  test_that("get_depth_cutoff_score is calculating correctly (not liberal)") {
    Rcpp::NumericVector scores = Rcpp::NumericVector::create(1, 2, 3);

    double actual = get_depth_cutoff_score(scores, false);
    double expected = 2 - 0.5f;
    expect_true(actual == expected);
  }

  test_that("get_depth_score_by_side calculates the score correctly (left side)") {
    Rcpp::NumericVector scores = Rcpp::NumericVector::create(5, 2, 3, 1, 1, 6, 7, 4);
    unsigned gap = 4;

    double actual = get_depth_score_by_side(scores, gap, true);
    double expected = 3 - 1;
    expect_true(actual == expected);
  }

  test_that("get_depth_score_by_side calculates the score correctly (right side)") {
    Rcpp::NumericVector scores = Rcpp::NumericVector::create(5, 2, 3, 1, 1, 6, 7, 4);
    unsigned gap = 4;

    double actual = get_depth_score_by_side(scores, gap, false);
    double expected = 7 - 1;
    expect_true(actual == expected);
  }

  test_that("get_depth_score_by_side calculates the score correctly (left side boundary)") {
    Rcpp::NumericVector scores = Rcpp::NumericVector::create(5, 1, 4);
    unsigned gap = 2;

    double actual = get_depth_score_by_side(scores, gap, true);
    double expected = 5 - 1;
    expect_true(actual == expected);
  }

  test_that("get_depth_score_by_side calculates the score correctly (right side boundary)") {
    Rcpp::NumericVector scores = Rcpp::NumericVector::create(5, 1, 4);
    unsigned gap = 2;

    double actual = get_depth_score_by_side(scores, gap, true);
    double expected = 4 - 1;
    expect_true(actual == expected);
  }
}
