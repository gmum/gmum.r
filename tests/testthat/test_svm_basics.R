library(testthat)
library('gmum.r')

data(svm_two_ellipsoids_dataset)
data(svm_breast_cancer_dataset)

test_that('SVM fucntions is fool proof', {
  
  ds <- svm.twoellipsoids.dataset
  f <- V3 ~ .

  expect_error( SVM(f, ds, lib="xyz"), paste(GMUM_WRONG_LIBRARY, ": bad library" ))
  expect_error( SVM(f, ds, kernel="xyz"), paste(GMUM_WRONG_KERNEL, ": bad kernel" ))
  expect_error( SVM(f, ds, prep="xyz"), paste(GMUM_BAD_PREPROCESS, ": bad preprocess" ))
  expect_error( SVM(f, ds, C = -1), paste(GMUM_WRONG_PARAMS, ": bad SVM parameters" ))
  expect_error( SVM(f, ds, gamma = -1), paste(GMUM_WRONG_PARAMS, ": bad SVM parameters" ))
  expect_error( SVM(f, ds, degree = 0), paste(GMUM_WRONG_PARAMS, ": bad SVM parameters" ))
  
  expect_warning( SVM(f, ds, kernel="linear", gamma=1), "Gamma parameter is not used with linear kernel" )
  expect_warning( SVM(f, ds, kernel="linear", degree=3), "Degree parameter is not used with linear kernel" )
  
})
print("test::SVM error")

test_that('formulas and data storing works', {
  
  dataset <- svm.breastcancer.dataset
  x1 <- data.matrix( dataset[,names(dataset) != "V1"] )
  y1 <- data.matrix( dataset[,"V1"] )
  svm <- SVM( V1 ~ ., dataset )
  x2 <- svm$getX()
  y2 <- svm$getY()
  expect_that(all.equal(x1,x2,check.attributes=FALSE), is_true())
  expect_that(all.equal(y1,y2,check.attributes=FALSE), is_true())
  
  formula <- V1 ~ V3 + V4 + V5
  data <- all.vars(update(formula,0~.))
  x3 <- data.matrix( dataset[,data] )
  y3 <- data.matrix( dataset[,"V1"] )

  svm2 <- SVM(formula, dataset)
  x4 <- svm2$getX()
  y4 <- svm2$getY()
  expect_that(all.equal(x3, x4, check.attributes=FALSE), is_true())
  expect_that(all.equal(y3, y4, check.attributes=FALSE), is_true())
  
})
print("test::SVM formula and dataset")



