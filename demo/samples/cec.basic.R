#' ---
#' title: "Basic clustering of mouse dataset using CEC"
#' author: ""
#' date: ""
#' output:
#'  html_document:
#'    self_contained: false
#' ---

# Load our library
library(gmum.r)

# Fitting to mouse like data set
data(cec.mouse1.spherical)

# Look at Mouse-like data set
plot(cec.mouse1.spherical, main="Mouse-like data set")

# Fit spherical gaussians to the data set
cec <- CEC(k=3, x=cec.mouse1.spherical, control.nstart=10, method.type="spherical")

# Plot results
plot(cec, centers=TRUE, ellipses=TRUE)

# Predict cluster which given points (1,1) and (2,2) would belong to:
predict(cec, c(1,1))
predict(cec, c(2,2))