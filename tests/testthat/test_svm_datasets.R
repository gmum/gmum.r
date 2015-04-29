library(testthat)
library('gmum.r')

data(svm_breast_cancer_dataset)
data(svm_two_ellipsoids_dataset)
data(svm_two_circles_dataset)

test_that('breast_cancer dataset works', {
  
  ds <- svm.breastcancer.dataset
  ds[,'X1'] <- as.factor(ds[,'X1'])
  formula <- X1 ~ .
  svm <- SVM(formula, ds, core="libsvm", kernel="linear", C=1)

  x <- ds[,-1]
  y <- ds[,1]
  
  pred <- predict(svm, x)
  diff <- length(pred) - length(y)
  
  expect_that(diff == 0, is_true())
})
print("test::breast cancer dataset")

test_that('2e dataset works', {
  
  ds <- svm.twoellipsoids.dataset
  ds[,3] <- as.factor(ds[,3])
  formula <- V3 ~ .
  svm <- SVM(formula, ds, core="libsvm", kernel="linear", C=1)
  
  x <- ds[,-3]
  target <- ds[,3]
  
  pred <- predict(svm, x)
  diff <- length(pred) - length(target)
  
  expect_that(diff == 0, is_true())
})
print("test::2e dataset")

test_that('2 circles dataset works', {
  
  ds <- svm.twocircles.dataset
  ds[,3] <- as.factor(ds[,3])
  formula <- V3 ~ .
  svm <- SVM(formula, ds, core="libsvm", kernel="linear", C=1)
  
  x <- ds[,-3]
  target <- ds[,3]
  
  pred <- predict(svm, x)
  diff <- length(pred) - length(target)
  
  expect_that(diff==0, is_true())
})
print("test::2 circles dataset")

test_that('xor dataset works', {
  
  ds <- as.data.frame(svm.dataset.xor())
  ds[,3] <- as.factor(ds[,3])
  formula <- t ~ .
  svm <- SVM(formula, ds, core="svmlight", kernel="linear", C=1)
  x <- ds[,-3]
  target <- ds[,3]
  pred <- predict(svm, x) 
  diff <- length(pred) - length(target)
  expect_that(diff == 0, is_true())
})
print("test::xor dataset")