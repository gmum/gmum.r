library(e1071)
library(klaR)
library(gmum.r)
library(testthat)

# You need svmlight executables for this test to work!

eps <- 0.05

test_that("our models have not too differents results than other models", {

  data(svm_breast_cancer_dataset)
  ds <- svm.breastcancer.dataset
  ds[,'X1'] <- as.factor(ds[,'X1'])
  x <- subset(ds, select = -X1)
  y <- factor(ds[,'X1'])
  formula <- X1 ~ .
  klar <- FALSE # set TRUE for svmlight testing if you have svmlight baniaries
  
  if (!klar){
    warning("svmlight portion is disabled, enable it if you have svmlight binaries")
  }
  
  # Linear kernel
  e_svm <- e1071::svm(formula, data=ds, type='C-classification', kernel="linear", cost=1, gamma=1, scale=FALSE)
  glib_svm <- gmum.r::SVM(formula, ds, core="libsvm", kernel="linear", C=1, verbosity=0)
  glight_svm <- gmum.r::SVM(formula, ds, core="svmlight", kernel="linear", C=1)
  if(klar) klar_svmlight <- klaR::svmlight(formula, data=ds, type="C", svm.options="-v 0")
  glib_esvm <- gmum.r::SVM(formula, ds, core="libsvm", kernel="linear", prep="2e", C=1, verbosity=0)
  e_pred <- as.numeric(as.matrix(fitted(e_svm)))
  glib_pred <- predict(glib_svm, x)
  glight_pred <- predict(glight_svm, x)
  # NOTE: No idea how to mute this... (tried: invisible, capture.output, sink)
  if(klar) klar_svmlight_pred <- as.numeric(as.matrix(predict(klar_svmlight, x)$class))
  glib_epred <- predict(glib_esvm, x)
  
  print("0. linear kernel:")
  
  y.factor <- as.factor(y)
  e_acc <- svm.accuracy(e_pred, y)
  print(sprintf("e1071 acc: %f", e_acc))
  glib_acc <- svm.accuracy(glib_pred, y.factor)
  print(sprintf("gmum libsvm acc: %f", glib_acc))
  if(klar) klar_acc <- svm.accuracy(klar_svmlight_pred,  y)
  if(klar) print(sprintf("klaR svmlight acc: %f", klar_acc))
  glight_acc <- svm.accuracy(glight_pred,  y.factor)
  print(sprintf("gmum svmlight acc: %f", glight_acc))
  acc <- svm.accuracy(glib_epred,  y.factor)
  print(sprintf("gmum libsvm 2e acc: %f", acc))
  
  expect_that( abs(e_acc-glib_acc) < eps, is_true())
  if(klar) expect_that( abs(klar_acc-glight_acc) < eps, is_true())
  
  # Poly kernel
  e_svm <- e1071::svm(formula, data=ds, type='C-classification', kernel="poly", cost=1, gamma=1, scale=FALSE)
  glib_svm <- gmum.r::SVM(formula, ds, core="libsvm", kernel="poly", C=1, verbosity=0, gamma=1)
  glight_svm <- gmum.r::SVM(formula, ds, core="svmlight", kernel="poly", C=1, gamma=1, coef0=1)
  if(klar) klar_svmlight <- klaR::svmlight(formula, data=ds, type="C", svm.options="-v 0 -t 1 -s 1 -r 1")
  
  e_pred <- as.numeric(as.matrix(fitted(e_svm)))
  glib_pred <- predict(glib_svm, x)
  glight_pred <- predict(glight_svm, x)
  if(klar) klar_svmlight_pred <- as.numeric(as.matrix(predict(klar_svmlight, x)$class))
  
  print ("1. poly kernel:")
  
  y.factor <- as.factor(y)
  e_acc <- svm.accuracy(e_pred, y)
  print(sprintf("e1071 acc: %f", e_acc))
  glib_acc <- svm.accuracy(glib_pred, y.factor)
  print(sprintf("gmum libsvm acc: %f", glib_acc))
  if(klar) klar_acc <- svm.accuracy(klar_svmlight_pred, y)
  if(klar) print(sprintf("klaR svmlight acc: %f", klar_acc))
  glight_acc <- svm.accuracy(glight_pred, y.factor)
  print(sprintf("gmum svmlight acc: %f", glight_acc))
  
  expect_that( e_acc-glib_acc < eps, is_true())
  if(klar) expect_that( abs(klar_acc-glight_acc) < eps, is_true())
  
  # RBF kernel
  e_svm <- e1071::svm(formula, data=ds, type='C-classification', kernel="radial", cost=1, gamma=1, scale=FALSE)
  glib_svm <- gmum.r::SVM(formula, ds, core="libsvm", kernel="rbf", C=1, verbosity=0, gamma=1)
  glight_svm <- gmum.r::SVM(formula, ds, core="svmlight", kernel="rbf", C=1, gamma=1)
  if(klar) klar_svmlight <- klaR::svmlight(formula, data=ds, type="C", svm.options="-v 0 -t 2 -s 1 -r 1")
  
  e_pred <- as.numeric(as.matrix(fitted(e_svm)))
  glib_pred <- predict(glib_svm, x)
  glight_pred <- predict(glight_svm, x)
  if(klar) klar_svmlight_pred <- as.numeric(as.matrix(predict(klar_svmlight, x)$class))
  

  print ("2. rbf kernel:")
  y.factor <- as.factor(y)
  e_acc <- svm.accuracy(e_pred, y)
  print(sprintf("e1071 acc: %f", e_acc))
  glib_acc <- svm.accuracy(glib_pred, y.factor)
  print(sprintf("gmum libsvm acc: %f", glib_acc))
  if(klar) klar_acc <- svm.accuracy(klar_svmlight_pred, y)
  if(klar) print(sprintf("klaR svmlight acc: %f", klar_acc))
  glight_acc <- svm.accuracy(glight_pred,y.factor)
  print(sprintf("gmum svmlight acc: %f", glight_acc))
  expect_that( abs(e_acc-glib_acc) < eps, is_true())
  if(klar) expect_that( abs(klar_acc-glight_acc) < eps, is_true())
  
  # Sigmoid kernel
  e_svm <- e1071::svm(formula, data=ds, type='C-classification', kernel="sigmoid", cost=1, gamma=1, scale=FALSE)
  glib_svm <- gmum.r::SVM(formula, ds, core="libsvm", kernel="sigmoid", C=1, verbosity=0)
  glight_svm <- gmum.r::SVM(formula, ds, core="svmlight", kernel="sigmoid", C=1)
  # -r and -s are sigmoid kernel parameters. 1/dimensions = 1/11 = 0.09090909
  if(klar) klar_svmlight <- klaR::svmlight(formula, data=ds, type="C", svm.options="-v 0 -t 3 -r -1 -s 0.09090909")
  
  e_pred <- as.numeric(as.matrix(fitted(e_svm)))
  glib_pred <- predict(glib_svm, x)
  glight_pred <- predict(glight_svm, x)
  if(klar) klar_svmlight_pred <- as.numeric(as.matrix(predict(klar_svmlight, x)$class))
  
  print ("3. sigmoid kernel:")
  y.factor <- as.factor(y)
  e_acc <- svm.accuracy(e_pred, y)
  print(sprintf("e1071 acc: %f", e_acc))
  glib_acc <- svm.accuracy(glib_pred, y.factor)
  print(sprintf("gmum libsvm acc: %f", glib_acc))
  if(klar) klar_acc <- svm.accuracy(klar_svmlight_pred, y)
  if(klar) print(sprintf("klaR svmlight acc: %f", klar_acc))
  glight_acc <- svm.accuracy(glight_pred, y.factor)
  print(sprintf("gmum svmlight acc: %f", glight_acc))
  
  expect_that( abs(e_acc-glib_acc) < eps || glib_acc > e_acc, is_true())
  if(klar) expect_that( abs(klar_acc-glight_acc) < eps, is_true())

})

