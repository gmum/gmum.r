#' ---
#' title: "Pick types of clusters in CEC"
#' author: ""
#' date: ""
#' output:
#'  html_document:
#'    self_contained: false
#' ---

library(gmum.r)

data(cec_ts)
standard_cluster_param = list(method.type = "standard")
spherical_cluster_param = list(method.type = "sphere")
diagonal_cluster_param = list(method.type = 'diagonal')

c <- CEC(x = Tset, k = 4, params.mix = list(standard_cluster_param, spherical_cluster_param, diagonal_cluster_param, diagonal_cluster_param), control.nstart = 100, control.eps=0.09)

plot(c, ellipses = TRUE, centers = TRUE)