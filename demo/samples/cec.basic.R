#' ---
#' title: "Basic clustering of mouse dataset using CEC"
#' author: ""
#' date: ""
#' output:
#'  html_document:
#'    self_contained: false
#' ---

library(gmum.r)

# Fitting to mouse like dataset
data(cec_mouse_1_spherical)
plot(cec.mouse1spherical, main="Mouse-like dataset")

# Fit spherical gaussians to the dataset
cec <- CEC(k=3, x=cec.mouse1spherical, control.nstart=10, method.type="sphere")

# Plot results
plot(cec, centers=TRUE, ellipses=TRUE)