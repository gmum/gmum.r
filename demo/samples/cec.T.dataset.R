#' ---
#' title: "Pick types of clusters in CEC"
#' author: ""
#' date: ""
#' output:
#'  html_document:
#'    self_contained: false
#' ---

# Load our library
library(gmum.r)

# Load T-like data set
data(cec_ts)

# We will be using 3 types of clusters
standard_cluster_param = list(method.type = "standard")
spherical_cluster_param = list(method.type = "spherical")
diagonal_cluster_param = list(method.type = "diagonal")

# 100 times run CEC algorithm on the T-like dataset. We start with standard, spherical and 2 diagonal cluster.
c <- CEC(x = Tset, params.mix = list(standard_cluster_param, spherical_cluster_param, diagonal_cluster_param, diagonal_cluster_param), control.nstart = 100, control.eps=0.09)

# Plot centroids with elipses and centers
plot(c, ellipses = TRUE, centers = TRUE)