library(testthat)
library('gmum.r')

data(svm_breast_cancer_dataset)

test_that('accuracy is calculated', {
  
  data(svm_breast_cancer_dataset)
  ds <- svm.breastcancer.dataset
  x <- subset(ds, select = -X1)
  y <- as.factor(ds[,'X1'])
  formula <- X1 ~ .
  
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", C=10); 
  pred <- predict(svm, x)
  length(pred)
  acc <- svm.accuracy(prediction=pred, target=y)
  
  expect_that(acc, is_a("numeric"))
  expect_that(acc <= 1, is_true())
  expect_that(acc > 0.5, is_true())
  
})
print("test::accuracy")

test_that('accuracy checks input size', {
          
  ds <- svm.dataset.xor()
  formula <- t ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", C=100);
  
  x <- svm$getX()
  pred <- predict(svm, x)
  target <- c(1, 1)
  
  expect_error(svm.accuracy(target, pred), "Prediction's and target's length don't match!")          
})
print("test::accuracy chcecks inputs size")



