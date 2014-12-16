library(gmum.r)
library(testthat)

test_that("svmlight works" , {
  
  ds = svm.dataset.2e()
  formula = V3 ~ .
  svm <- SVM(formula, ds, lib="svmlight", kernel="linear", C=10);
  
  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x)  
  acc <- svm.accuracy(prediction=pred, target=target)
  print(acc)
  
  expect_that(acc, is_a("numeric"))
  expect_that(acc <= 1, is_true())
  expect_that(acc > 0.5, is_true())
})
print("test::svmlight works with simple 2e dataset")

test_that('accuracy is calculated', {
  
  ds = svm.dataset.breast_cancer()
  formula = X1 ~ .
  svm <- SVM(formula, ds, lib="svmlight", kernel="linear", C=100);
  
  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=target)
  print(acc)
  
  expect_that(acc, is_a("numeric"))
  expect_that(acc <= 1, is_true())
  expect_that(acc > 0.5, is_true())
  
})
print("test::accuracy")