// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// generate_texttile_segment_ids_from_boundry_points_cpp
Rcpp::IntegerVector generate_texttile_segment_ids_from_boundry_points_cpp(Rcpp::IntegerVector& boundary_pts, unsigned n);
RcppExport SEXP _texmer_generate_texttile_segment_ids_from_boundry_points_cpp(SEXP boundary_ptsSEXP, SEXP nSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::IntegerVector& >::type boundary_pts(boundary_ptsSEXP);
    Rcpp::traits::input_parameter< unsigned >::type n(nSEXP);
    rcpp_result_gen = Rcpp::wrap(generate_texttile_segment_ids_from_boundry_points_cpp(boundary_pts, n));
    return rcpp_result_gen;
END_RCPP
}
// tokensequence_test
void tokensequence_test();
RcppExport SEXP _texmer_tokensequence_test() {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    tokensequence_test();
    return R_NilValue;
END_RCPP
}
// generate_token_segment_ids_cpp
Rcpp::IntegerVector generate_token_segment_ids_cpp(const Rcpp::IntegerVector& ntoken_per_doc, const Rcpp::IntegerVector& nseg_per_doc, const Rcpp::IntegerVector& segsize_per_doc);
RcppExport SEXP _texmer_generate_token_segment_ids_cpp(SEXP ntoken_per_docSEXP, SEXP nseg_per_docSEXP, SEXP segsize_per_docSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const Rcpp::IntegerVector& >::type ntoken_per_doc(ntoken_per_docSEXP);
    Rcpp::traits::input_parameter< const Rcpp::IntegerVector& >::type nseg_per_doc(nseg_per_docSEXP);
    Rcpp::traits::input_parameter< const Rcpp::IntegerVector& >::type segsize_per_doc(segsize_per_docSEXP);
    rcpp_result_gen = Rcpp::wrap(generate_token_segment_ids_cpp(ntoken_per_doc, nseg_per_doc, segsize_per_doc));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_texmer_generate_texttile_segment_ids_from_boundry_points_cpp", (DL_FUNC) &_texmer_generate_texttile_segment_ids_from_boundry_points_cpp, 2},
    {"_texmer_tokensequence_test", (DL_FUNC) &_texmer_tokensequence_test, 0},
    {"_texmer_generate_token_segment_ids_cpp", (DL_FUNC) &_texmer_generate_token_segment_ids_cpp, 3},
    {NULL, NULL, 0}
};

RcppExport void R_init_texmer(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
