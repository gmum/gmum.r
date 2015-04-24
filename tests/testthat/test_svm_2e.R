library(testthat)

data(svm_two_ellipsoids_dataset)
data(svm_breast_cancer_dataset)

test_that("2e svm works with 2e dataset", {
  
  ds <- svm.twoellipsoids.dataset
  ds[,'V3'] <- as.factor(ds[,'V3'])
  formula <- V3 ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", prep = "2e", C=10);
  x <- ds[,-3]
  target <- ds[,3]
  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=target)
  expect_that(acc==1, is_true())
})
print("test::2eSVM works with 2e dataset")

test_that("2e svm works with breast cancer dataset", {  
  ds <- svm.breastcancer.dataset
  ds[,'X1'] <- as.factor(ds[,'X1'])
  formula <- X1 ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", prep = "2e", C=10);
  x <- ds[,-1]
  target <- ds[,1]
  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=target)
  
  expect_that(acc>0.95, is_true())
})
print("test::2eSVM works with breast cancer dataset")

test_that("2e svm works better then normal SVM with breast cancer dataset", {

  ds <- svm.breastcancer.dataset
  ds[,'X1'] <- as.factor(ds[,'X1'])
  formula <- X1 ~ .
  
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", prep = "none", C=1.5);
  twoe_svm <- SVM(formula, ds, lib="libsvm", kernel="linear", prep = "2e", C=1.5);

  x <- ds[,-1]
  target <- ds[,1]
  
  pred <- predict(svm, x)
  twoe_pred <- predict(twoe_svm, x)
  
  acc <- svm.accuracy(prediction=pred, target=target)
  twoe_acc <- svm.accuracy(prediction=twoe_pred, target=target)

  expect_true(twoe_acc>acc)
})
print("test::2eSVM worksslightly better with breast cancer dataset")
