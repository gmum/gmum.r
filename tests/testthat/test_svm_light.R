library(gmum.r)
library(testthat)

test_that("svmlight works" , {
  data(svm_two_ellipsoids_dataset)
  ds <- svm.twoellipsoids.dataset
  ds[,'V3'] <- as.factor(ds[,'V3'])
  formula <- V3 ~ .
  svm <- SVM(formula, ds, core="svmlight", kernel="linear", C=10, verbosity = 3);
  
  x <- ds[,-3]
  target <- ds[,3]
  
  pred <- predict(svm, x)  
  acc <- svm.accuracy(prediction=pred, target=target)
  print(acc)
  
  expect_that(acc, is_a("numeric"))
  expect_that(acc == 1, is_true())
})
print("test::svmlight works with simple 2e dataset")

test_that('accuracy is calculated', {
  
  data(svm_breast_cancer_dataset)
  ds <- svm.breastcancer.dataset
  ds[,'X1'] <- as.factor(ds[,'X1'])
  formula <- X1 ~ .
  svm <- SVM(formula, ds, core="svmlight", kernel="linear", C=100);
  
  x <- ds[,-1]
  target <- ds[,1]
  
  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=target)
  print(acc)
  
  expect_that(acc, is_a("numeric"))
  expect_that(acc <= 1, is_true())
  expect_that(acc > 0.5, is_true())
  
})
print("test::accuracy")