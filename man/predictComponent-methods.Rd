% Generated by roxygen2 (4.1.1): do not edit by hand
% Please edit documentation in R/gng.R
\docType{methods}
\name{predictComponent}
\alias{predictComponent}
\title{predictComponent}
\usage{
predictComponent(object, x)
}
\arguments{
\item{object}{GNG object}

\item{x}{Can be either \code{vector} or \code{data.frame}.}
}
\description{
Finds connected component closest to given vector(s). On the first
execution of function strongly connected components are calculated using igraph::cluster function.
}
\details{
Find closest component
}
\examples{
gng <- GNG(gng.preset.sphere(100))
# Find closest component to c(1,1,1)
predictComponent(gng,  c(1,1,1))
}

