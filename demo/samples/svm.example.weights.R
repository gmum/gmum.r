#' ---
#' title: "Example weight support in gmum.R"
#' author: ""
#' date: ""
#' ---

library(gmum.r)
library(ggplot2)
library(SparseM)
library(Matrix)

# Initialize exemplary data
x <- cbind(c(2.76405235,  1.97873798,  2.86755799,  1.95008842,  0.89678115,  1.14404357,
             1.76103773,  1.44386323,  2.49407907,  1.3130677,  -2.55298982,  0.8644362,
             2.26975462,  0.04575852,  1.53277921,  0.15494743, -0.88778575, -0.34791215,
             1.23029068, -0.38732682),
           c(1.40015721,  3.2408932,   0.02272212,  0.84864279,  1.4105985,   2.45427351,
             1.12167502,  1.33367433,  0.79484174,  0.14590426,  0.6536186,  -0.74216502,
             -1.45436567, -0.18718385,  1.46935877,  0.37816252, -1.98079647,  0.15634897,
             1.20237985, -0.30230275))
y <- c(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1)
weights <- c(1.04855297,  1.42001794,  1.70627019,  1.9507754,   0.50965218,  0.4380743,
             1.25279536,  0.77749036,  1.61389785,  3.1911042,   0.89546656,  0.3869025,
             0.51080514,  1.18063218,  0.02818223,  2.14165935,  0.33258611,  1.51235949,
             3.17161047,  1.81370583)


df <- data.frame(x, y)

svm.rbf <- SVM(formula=y~., data=df, lib="libsvm", kernel="rbf", C=1.0, gamma=0.5)
weighted.svm.rbf <- SVM(formula=y~., data=df, lib="svmlight", kernel="rbf", C=1.0, 
                        gamma=0.5, example.weights=weights)

# Plot to see how decision boundary is affected
plot(svm.rbf, mode="contour")
plot(weighted.svm.rbf, mode="contour", radius.max=10)
