library(testthat)
library('gmum.r')

test_that('SVM fucntions is fool proof', {
  
  ds = svm.dataset.2e()
  f = V3 ~ .
  
  expect_error( SVM(f, ds, lib="xyz"), "Error 20: bad library" )
  expect_error( SVM(f, ds, kernel="xyz"), "Error 21: bad kernel")
  expect_error( SVM(f, ds, prep="xyz"), "Error 22: bad preprocess" )
  expect_error( SVM(f, ds, C = -1), "Error 24: bad parameters" )
  expect_error( SVM(f, ds, gamma = -1), "Error 24: bad parameters" )
  expect_error( SVM(f, ds, degree = 0), "Error 24: bad parameters" )
  
  expect_warning( SVM(f, ds, kernel="linear", gamma=1), "Gamma parameter is not used with linear kernel" )
  expect_warning( SVM(f, ds, kernel="linear", degree=3), "Degree parameter is not used with linear kernel" )
  
})
print("test::SVM error")

test_that('formulas and data storing works', {
  
  dataset <- svm.dataset.breast_cancer()
  x1 = data.matrix( dataset[,names(dataset) != "X1"] )
  y1 = data.matrix( dataset[,"X1"] )
  svm <- SVM( X1 ~ ., dataset )
  x2 = svm$getX()
  y2 = svm$getY()
  expect_that(all.equal(x1,x2,check.attributes=FALSE), is_true())
  expect_that(all.equal(y1,y2,check.attributes=FALSE), is_true())
  
  formula = X1 ~ X2 + X3 + X4
  data = all.vars(update(formula,0~.))
  x3 = data.matrix( dataset[,data] )
  y3 = data.matrix( dataset[,"X1"] )
  svm2 <- SVM(formula, dataset)
  x4 = svm2$getX()
  y4 = svm2$getY()
  expect_that(all.equal(x3,x4,check.attributes=FALSE), is_true())
  expect_that(all.equal(y3,y4,check.attributes=FALSE), is_true())
  
})
print("test::SVM formula and dataset")



