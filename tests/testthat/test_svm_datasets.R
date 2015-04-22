library(testthat)
library('gmum.r')

data(svm_breast_cancer_dataset)
data(svm_two_ellipsoids_dataset)
data(svm_two_circles_dataset)

test_that('breast_cancer dataset works', {
  
  ds <- svm.breastcancer.dataset
  formula <- X1 ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", C=10)

  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x)
  diff <- length(pred) - length(target)
  
  expect_that(diff == 0, is_true())
})
print("test::breast cancer dataset")

test_that('2e dataset works', {
  
  ds <- svm.twoellipsoids.dataset
  formula <- V3 ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", C=10)
  
  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x)
  diff <- length(pred) - length(target)
  
  expect_that(diff == 0, is_true())
})
print("test::2e dataset")

test_that('2 circles dataset works', {
  
  ds <- svm.twocircles.dataset
  formula <- V3 ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", C=10)
  
  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x)
  diff <- length(pred) - length(target)
  
  expect_that(diff==0, is_true())
})
print("test::2 circles dataset")

test_that('xor dataset works', {
  
  ds <- as.data.frame(svm.dataset.xor())
  formula <- t ~ .
  svm <- SVM(formula, ds, lib="svmlight", kernel="linear", C=1e-3, verbosity=6)
  x <- svm$getX()
  target <- svm$getY()
  pred <- predict(svm, ds[,1:2]) 
  diff <- length(pred) - length(target)
  expect_that(diff == 0, is_true())
})
print("test::xor dataset")