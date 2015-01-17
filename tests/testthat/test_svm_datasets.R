library(testthat)
library('gmum.r')

test_that('breast_cancer dataset works', {
  
  ds <- svm.dataset.breast_cancer()
  formula <- X1 ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", C=100)
  
  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x)
  diff <- length(pred) - length(target)
  
  expect_that(diff == 0, is_true())
})
print("test::breast cancer dataset")

test_that('2e dataset works', {
  
  ds <- svm.dataset.2e()
  formula <- V3 ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", C=100)
  
  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x)
  diff <- length(pred) - length(target)
  
  expect_that(diff == 0, is_true())
})
print("test::2e dataset")

test_that('xor dataset works', {
  
  ds <- svm.dataset.xor()
  formula <- t ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", C=100)
  
  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x) 
  diff <- length(pred) - length(target)
  
  expect_that(diff == 0, is_true())
})
print("test::xor dataset")