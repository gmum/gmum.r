library(testthat)
library('gmum.r')

data(svm_two_ellipsoids_dataset)
data(svm_breast_cancer_dataset)

test_that('SVM functions are fool proof', {
  
  ds <- svm.twoellipsoids.dataset
  ds[,'V3'] <- as.factor(ds[,'V3'])
  f <- V3 ~ .

  expect_error( SVM(f, ds, lib="xyz"), paste(GMUM_WRONG_LIBRARY, ": bad library" ))
  expect_error( SVM(f, ds, kernel="xyz"), paste(GMUM_WRONG_KERNEL, ": bad kernel" ))
  expect_error( SVM(f, ds, prep="xyz"), paste(GMUM_BAD_PREPROCESS, ": bad preprocess" ))
  expect_error( SVM(f, ds, C = -1), paste(GMUM_WRONG_PARAMS, ": bad SVM parameters" ))
  expect_error( SVM(f, ds, gamma = -1), paste(GMUM_WRONG_PARAMS, ": bad SVM parameters" ))
  expect_error( SVM(f, ds, degree = 0), paste(GMUM_WRONG_PARAMS, ": bad SVM parameters" ))
    
})
print("test::SVM throws user errors")

test_that("SVM both constructor works", {
  
  data(svm_breast_cancer_dataset)
  ds <- svm.breastcancer.dataset
  ds[,'X1'] <- as.factor(ds[,'X1'])
  x <- subset(ds, select = -X1)
  y <- ds[,'X1']
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



