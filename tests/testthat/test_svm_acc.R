library(testthat)
library(gmum.r)

data(svm_breast_cancer_dataset)

test_that('accuracy is calculated', {
  
  data(svm_breast_cancer_dataset)
  ds <- svm.breastcancer.dataset
  ds[,'X1'] <- as.factor(ds[,'X1'])
  x <- ds[,-1]
  y <- ds[,1]
  formula <- X1 ~ .
  
  svm <- SVM(formula, ds, core="libsvm", kernel="linear", C=10); 
  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=y)
  
  expect_that(acc, is_a("numeric"))
  expect_that(acc <= 1, is_true())
  expect_that(acc > 0.95, is_true())
})
print("test::svm accuracy")

test_that('accuracy checks input size', {
          
  ds <- as.data.frame(svm.dataset.xor())
  ds[,3] <- as.factor(ds[,3])
  formula <- t ~ .
  svm <- SVM(formula, ds, core="libsvm", kernel="linear", C=100);
  
  x <- ds[,-3]
  pred <- predict(svm, x)
  target <- c(1, 1)
  
  expect_error(svm.accuracy(target, pred), "Prediction's and target's length don't match!")          
})
print("test::accuracy chcecks inputs size")



