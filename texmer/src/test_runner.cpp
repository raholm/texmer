#define CATCH_CONFIG_RUNNER
#include <testthat.h>
#include <Rcpp.h>

// [[Rcpp::export]]
bool run_testthat_tests() {
  bool success = testthat::run_tests();
  return success;
}
