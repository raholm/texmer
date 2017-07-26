#ifndef TOKEN_H
#define TOKEN_H

#include <Rcpp.h>

Rcpp::IntegerVector generate_token_segment_ids_cpp(const Rcpp::IntegerVector& ntoken_per_doc,
                                                   const Rcpp::IntegerVector& nseg_per_doc,
                                                   const Rcpp::IntegerVector& segsize_per_doc);
Rcpp::IntegerVector generate_repeat_count_vec(int ntoken, int segsize);
Rcpp::IntegerVector repeat_int(int value, int n);
void print_vec(const Rcpp::IntegerVector& vec);

#endif // TOKEN_H
