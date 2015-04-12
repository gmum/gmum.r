library(gmum.r)
library(testthat)
library(ggplot2)

lib <- "svmlight"
verbose <- FALSE # use TRUE only for local testing

if (verbose) {
  print(sprintf("Library used for non-weights svm: %s", lib))
}

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

sk_no_weights_SV_rbf <- cbind(c(-2.55298982,  0.8644362,   2.26975462,  1.53277921,  0.15494743, -0.88778575,
                         1.23029068,  2.76405235,  1.97873798,  2.86755799,  0.89678115,  1.14404357,
                         1.76103773,  1.44386323,  1.3130677),
                       c(0.6536186,  -0.74216502, -1.45436567,  1.46935877,  0.37816252, -1.98079647,
                         1.20237985,  1.40015721,  3.2408932,   0.02272212,  1.4105985,   2.45427351,
                         1.12167502,  1.33367433,  0.14590426))

sk_no_weights_SV_linear <- cbind(c(2.26975462,  1.53277921,  0.15494743,  1.23029068,  2.86755799,  1.95008842,
                                0.89678115,  1.3130677),
                              c(-1.45436567,  1.46935877,  0.37816252,  1.20237985,  0.02272212,  0.84864279,
                                1.4105985,   0.14590426))

sk_weights_SV_rbf <- cbind(c(-2.55298982,  0.8644362,   2.26975462,  0.04575852,  1.53277921,  0.15494743,
                          -0.88778575,  1.23029068,  2.76405235,  1.97873798,  2.86755799,  0.89678115,
                          1.14404357,  1.76103773,  1.44386323,  1.3130677),
                        c(0.6536186,  -0.74216502, -1.45436567, -0.18718385,  1.46935877,  0.37816252,
                          -1.98079647,  1.20237985,  1.40015721,  3.2408932,   0.02272212,  1.4105985,
                          2.45427351,  1.12167502,  1.33367433,  0.14590426) )

sk_weights_SV_linear <- cbind(c(0.8644362,   2.26975462,  1.53277921,  0.15494743,  1.23029068,  0.89678115,
                             1.14404357,  1.76103773,  1.44386323,  1.3130677),
                           c(-0.74216502, -1.45436567,  1.46935877,  0.37816252,  1.20237985,  1.4105985,
                             2.45427351,  1.12167502,  1.33367433,  0.14590426))

df <- data.frame(x, y)

# -------------------------------------------------------

no_weights_svm_rbf <- SVM(formula=y~., data=df, lib=lib, kernel="rbf", C=1.0, gamma=0.5)
SV_rbf <- no_weights_svm_rbf$get_SV()

if (verbose) {

  print(sprintf("  sklearn rbf nSV: %i", nrow(sk_no_weights_SV_rbf)))
  print(sprintf("  gmum.r rbf nSV %i", nrow(SV_rbf)))
  
  p1 <- ggplot() + 
    geom_point(data=df, aes(X1,X2, color=factor(y), size=2)) + 
    geom_point(data=data.frame(SV_rbf), aes(X1, X2), color="black") +
    ggtitle("gmum.r rbf") +
    theme(legend.position="none")
  
  p2 <- ggplot() + 
    geom_point(data=df, aes(X1,X2, color=factor(y), size=2)) + 
    geom_point(data=data.frame(sk_no_weights_SV_rbf), aes(X1, X2), color="black") +
    ggtitle("sklearn rbf") +
    theme(legend.position="none")
  
  multiplot(p1, p2, cols=2)
}
# -------------------------------------------------------

no_weights_svm_linear <- SVM(formula=y~., data=df, lib=lib, kernel="linear", C=1.0)
SV_linear <- no_weights_svm_linear$get_SV()

if (verbose) {

  print(sprintf("  sklearn linear nSV: %i", nrow(sk_no_weights_SV_linear)))
  print(sprintf("  gmum.r linear nSV %i", nrow(SV_linear)))
  
  p3 <- ggplot() + 
    geom_point(data=df, aes(X1,X2, color=factor(y), size=2)) + 
    geom_point(data=data.frame(SV_linear), aes(X1, X2), color="black") +
    ggtitle("gmum.r linear") +
    theme(legend.position="none")
  
  p4 <- ggplot() + geom_point(data=df, aes(X1,X2, color=factor(y), size=2)) + 
    geom_point(data=data.frame(sk_no_weights_SV_linear), aes(X1, X2), color="black") +
    ggtitle("sklearn linear") +
    theme(legend.position="none")
  
  multiplot(p3, p4, cols=2)
}

# -------------------------------------------------------
if (verbose) {
  print("For weighted examples only svmlight is available")
}

weights_svm_rbf <- SVM(formula=y~., data=df, lib="svmlight", kernel="rbf", C=1.0, gamma=0.5, example_weights=weights)
weights_SV_rbf <- weights_svm_rbf$get_SV()

if (verbose) {

  print(sprintf("  sklearn weighted rbf nSV: %i", nrow(sk_weights_SV_rbf)))
  print(sprintf("  gmum.r weighted rbf nSV %i", nrow(weights_SV_rbf)))
  
  p5 <- ggplot() + 
    geom_point(data=df, aes(X1,X2, color=factor(y), size=2)) + 
    geom_point(data=data.frame(weights_SV_rbf), aes(X1, X2), color="black") +
    ggtitle("gmum.r weighted rbf") +
    theme(legend.position="none")
  
  p6 <- ggplot() + 
    geom_point(data=df, aes(X1,X2, color=factor(y), size=2)) + 
    geom_point(data=data.frame(sk_weights_SV_rbf), aes(X1, X2), color="black") +
    ggtitle("sklearn weighted rbf") +
    theme(legend.position="none")
  
  multiplot(p5, p6, cols=2)
}
# -------------------------------------------------------

weights_svm_linear <- SVM(formula=y~., data=df, lib="svmlight", kernel="linear", C=1.0, example_weights=weights)
weights_SV_linear <- weights_svm_linear$get_SV()

if (verbose) {
    
  print(sprintf("  sklearn weighted linear nSV: %i", nrow(sk_weights_SV_linear)))
  print(sprintf("  gmum.r weighted linear nSV %i", nrow(weights_SV_linear)))
  
  p7 <- ggplot() + 
    geom_point(data=df, aes(X1,X2, color=factor(y), size=2)) + 
    geom_point(data=data.frame(weights_SV_linear), aes(X1, X2), color="black") +
    ggtitle("gmum.r weighted linear") +
    theme(legend.position="none")
  
  p8 <- ggplot() + 
    geom_point(data=df, aes(X1,X2, color=factor(y), size=2)) + 
    geom_point(data=data.frame(sk_weights_SV_linear), aes(X1, X2), color="black") +
    ggtitle("sklearn weighted linear") +
    theme(legend.position="none")
  
  multiplot(p7, p8, cols=2)
}

diff <- sum(abs(weights_SV_rbf[order(weights_SV_rbf[,1]),] - sk_weights_SV_rbf[order(sk_weights_SV_rbf[,1]),]))
if (verbose) {
  print(sprintf("Difference of (compatible) weighted rbf SVs: %.10f", diff))
}

test_that("weighted exaples in svmlight work like in sklearn SVC", {
  expect_that(diff, is_less_than(1e-2))
})
print("test::SVM weighted exaples in svmlight work like in sklearn SVC")
