library(testthat)
library('gmum.r')

test_that('libsvm predicts something', {

    a <- SVM()
    x <- a$getX()
    t <- a$getY()
    train(a)
    #y <- predict(a,x)
    #y
  
    expect_that(length(y), equals(length(t)))
})