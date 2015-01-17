test_that("2e works with really big dataset", {
  
  ds = svm.dataset.duke()
  ds2 = svm.dataset.duke()
  formula = X1 ~ .
  
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", prep = "none", C=10);
  twoe_svm <- SVM(formula, ds, lib="libsvm", kernel="linear", prep = "2e", C=10);
  
  x <- svm$getX()
  target <- svm$getY()
  
  pred <- predict(svm, x)
  twoe_pred <- predict(twoe_svm, x)
  
  acc <- svm.accuracy(prediction=pred, target=target)
  twoe_acc <- svm.accuracy(prediction=twoe_pred, target=target)
  
  print(sprintf("SVM acc: %f", acc))
  print(sprintf("2eSVM acc: %f", twoe_acc))
  
})