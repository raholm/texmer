#ifndef TEXTTILE_H
#define TEXTTILE_H

#include <Rcpp.h>

Rcpp::IntegerVector find_gap_boundaries_cpp(Rcpp::NumericVector& lexical_scores,
                                            bool liberal);
double get_depth_cutoff_score(Rcpp::NumericVector& lexical_scores,
                              bool liberal);
double get_depth_score_by_side(Rcpp::NumericVector& lexical_scores,
                               unsigned gap,
                               bool left);

Rcpp::IntegerVector generate_texttile_segment_ids_from_boundry_points_cpp(Rcpp::IntegerVector& boundary_pts,
                                                                          unsigned n);
void fill_ids(Rcpp::IntegerVector* ids, unsigned* from, int with, int times);

#endif // TEXTTILE_H
