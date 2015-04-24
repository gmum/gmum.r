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
    
})
print("test::SVM throws user errors")

test_that('formulas and data storing works', {
  
  dataset <- svm.breastcancer.dataset
  x1 <- data.matrix( dataset[,names(dataset) != "X1"] )
  y1 <- as.factor( dataset[,"X1"] )
  svm <- SVM( X1 ~ ., dataset )
  x2 <- svm$getX()
  y2 <- svm$getY()
  expect_that(all.equal(x1,x2,check.attributes=FALSE), is_true())

  formula <- X1 ~ X3 + X4 + X5
  data <- all.vars(update(formula,0~.))
  x3 <- data.matrix( dataset[,data] )
  y3 <- as.factor(dataset[,"X1"] )

  svm2 <- SVM(formula, dataset)
  x4 <- svm2$getX()
  y4 <- svm2$getY()
  expect_that(all.equal(x3, x4, check.attributes=FALSE), is_true())
  
})
print("test::SVM formula and dataset")

test_that("SVM both constructor works", {
  
  data(svm_breast_cancer_dataset)
  ds <- svm.breastcancer.dataset
  x <- subset(ds, select = -X1)
  y <- as.vector(unlist(ds['X1']))
  f <- X1 ~ .
  
  svm1 <- SVM(x,y)
  svm2 <- SVM(f, ds)
  
  pred1 <- predict(svm1, x)
  pred2 <- predict(svm2, x)
  
  expect_that(all.equal(pred1, pred2), is_true())
})
print("test::SVM both constructors work")


test_that("svm accepts and deals properly with factors", {
  
  library(mlbench)
  data(Sonar)
  library(caret)
  set.seed(998)
  
  inTraining <- createDataPartition(Sonar$Class, p = .75, list = FALSE)
  training <- Sonar[ inTraining,]
  svm <- SVM(Class ~ ., training)
  pred <- predict(svm, training[, 1:60])
  
  expect_is(pred, "factor")
  
})

test_that("svm max.iter works", {
  
  data(svm_breast_cancer_dataset)
  ds <- svm.breastcancer.dataset
  x <- subset(ds, select = -X1)
  y <- factor(unlist(ds['X1']))

  
  start_time <- Sys.time()
  svm1 <- SVM(x,y, lib="libsvm", max.iter = 10)
  restricted_time <- Sys.time() - start_time
  
  start_time <- Sys.time()
  svm2 <- SVM(x,y, lib="libsvm")
  normal_time <- Sys.time() - start_time
  
  expect_true(restricted_time < normal_time)
  
  start_time <- Sys.time()
  svm1 <- SVM(x,y, lib="svmlight", max.iter = 10)
  restricted_time <- Sys.time() - start_time
  
  start_time <- Sys.time()
  svm2 <- SVM(x,y, lib="svmlight")
  normal_time <- Sys.time() - start_time
  
  expect_true(restricted_time < normal_time)
})



