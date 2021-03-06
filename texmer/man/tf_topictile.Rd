% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/topictile.R
\name{tf_topictile}
\alias{tf_topictile}
\title{Segmenting text based on Latent Dirichlet Allocation.}
\usage{
tf_topictile(corpus, stopwords, mode, sentence_size = 20, block_size = 2,
  nsegments = 0, liberal = TRUE, smoothing_rounds = 1,
  smoothing_width = 2)
}
\value{
A data frame with 'token' replaced by the segments in 'text'. 'id' contains the new ids and the old ids are preserved in 'docid'.
}
\description{
description here.
}
\details{
details here.
}
