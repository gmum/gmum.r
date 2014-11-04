library(testthat)
library('gmum.r')

test_that('accuracy is calculated', {
  
  ds = svm.dataset.breast_cancer()
  formula = X1 ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", C=100);
  
  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=pred)
  print(acc)
  
  expect_that(acc, is_a("numeric"))
  expect_that(acc <= 1, is_true())
  expect_that(acc > 0.5, is_true())
  
})
print("test::accuracy")

test_that('accuracy checks input size', {
          
  ds = svm.dataset.xor()
  formula = t ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", C=100);
  
  x <- svm$getX()
  pred <- predict(svm, x)
  target <- c(1,1)
  
  expect_error(svm.accuracy(target,pred), "Prediction's and target's length don't match!")          
})
print("test::accuracy error")



