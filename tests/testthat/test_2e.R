library(testthat)
library('gmum.r')

test_that("2e svm works with 2e dataset", {
  
  ds = svm.dataset.2e()
  formula = V3 ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", prep = "2e", C=10);
  
  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=pred)
  print(acc)
})

test_that("2e svm works with breast cancer dataset", {
  
  ds = svm.dataset.breast_cancer()
  formula = X1 ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", prep = "2e", C=10);
  
  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=pred)
  print(acc)
})