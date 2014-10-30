library(testthat)
library('gmum.r')

test_that('formulas work', {
  
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