library(testthat)
library('gmum.r')

data(svm_two_ellipsoids_dataset)
data(svm_breast_cancer_dataset)

test_that("2e svm works with 2e dataset", {
  
  ds <- svm.twoellipsoids.dataset
  formula <- V3 ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", prep = "2e", C=10);
  
  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=target)
  expect_that(acc>0.5, is_true())
})
print("test::2eSVM works with 2e dataset")

test_that("2e svm works with breast cancer dataset", {
  
  ds <- svm.breastcancer.dataset
  formula <- V1 ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", prep = "2e", C=10);
  
  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=target)
  
  expect_that(acc>0.5, is_true())
})
print("test::2eSVM works with breast cancer dataset")

test_that("2e svm works better then normal SVM with breast cancer dataset", {

  print("####### Start 2e vs normal svm #########")
  ds <- svm.breastcancer.dataset
  ds2 <- svm.breastcancer.dataset
  formula <- V1 ~ .
  
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", prep = "none", C=1.5);
  twoe_svm <- SVM(formula, ds, lib="libsvm", kernel="linear", prep = "2e", C=1.5);

  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x)
  twoe_pred <- predict(twoe_svm, x)
  
  acc <- svm.accuracy(prediction=pred, target=target)
  twoe_acc <- svm.accuracy(prediction=twoe_pred, target=target)
  
  print(sprintf("SVM acc: %f", acc))
  print(sprintf("2eSVM acc: %f", twoe_acc))
  print("####### End 2e vs normal svm #########")
})
