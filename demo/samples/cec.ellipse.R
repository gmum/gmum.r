#' ---
#' title: "Fit any gauss family to dataset in CEC"
#' author: ""
#' date: ""
#' output:
#'  html_document:
#'    self_contained: false
#' ---

# Load our library
library(gmum.r)

# Load ellipse_gauss data set
data(cec.ellipsegauss)

# Try to fit 5 gaussians to the data set
cec <- CEC(k=5, x=cec.ellipsegauss, method.init="random")

# Note that we set 5 cluster. Only 4 was needed.
plot(cec, centers=TRUE, ellipses=TRUE)

# Show information about clustering
summary(cec)
