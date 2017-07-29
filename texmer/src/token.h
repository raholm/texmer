#ifndef TOKEN_H
#define TOKEN_H

#include <Rcpp.h>

Rcpp::IntegerVector get_token_segment_ids_cpp(const Rcpp::IntegerVector& ntoken_per_doc,
                                                   const Rcpp::IntegerVector& nseg_per_doc,
                                                   const Rcpp::IntegerVector& segsize_per_doc);
Rcpp::IntegerVector get_repeat_counts(int n_tokens, int segsize);
void print(const Rcpp::IntegerVector& vec);

#endif // TOKEN_H
