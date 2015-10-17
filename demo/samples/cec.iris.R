#' ---
#' title: "PCA applied for iris data set clustered with CEC"
#' author: ""
#' date: ""
#' output:
#'  html_document:
#'    self_contained: false
#' ---

# Load iris data set (dimension = 4)
dataset_points <- iris

# Load our library
library(gmum.r)

# Try to fit 4 gaussians to the data set
cec <- CEC(x=dataset_points, k=3, method.type = "spherical")

# Scatterplot of the data set clustered with CEC
plot(cec, slice=c(1,2,3,4))

# Apply PCA in plotting;
plot(cec, pca=TRUE)

# Show info about clustering
summary(cec)
