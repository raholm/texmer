#ifndef TOKEN_H
#define TOKEN_H

#include <Rcpp.h>

Rcpp::IntegerVector generate_token_segment_ids_cpp(const Rcpp::IntegerVector& ntokens_per_doc,
                                                   const Rcpp::IntegerVector& nsegments_per_doc,
                                                   const Rcpp::IntegerVector& segment_size_per_doc);
Rcpp::IntegerVector generate_repeat_count_vec(int ntokens, int segsize);
Rcpp::IntegerVector repeat_int(int value, int n);
void print_vec(const Rcpp::IntegerVector& vec);

#endif // TOKEN_H
