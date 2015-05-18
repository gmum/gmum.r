#' ---
#' title: "Fit any gauss family to dataset in CEC"
#' author: ""
#' date: ""
#' output:
#'  html_document:
#'    self_contained: false
#' ---


library(gmum.r)
data(cec_ellipse_gauss)

# Try to fit 5 gaussians to the dataset
cec <- CEC(k=5, x=cec.ellipsegauss, method.init="random")

# Note that we set 5 cluster. Only 4 was needed.
plot(cec, centers=TRUE, ellipses=TRUE)
summary(cec)

