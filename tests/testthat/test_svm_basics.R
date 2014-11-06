library(testthat)
library('gmum.r')

test_that('data swapping works', {
  a <- SVM()
  xor <- dataset.xor()
  x <- xor[,c(1,2)]
  y <- xor[,3]

  a$setX(x)
  a$setY(y)
  
  expect_that(dataset.X(a), equals(x))
  # expect_that(dataset.Y(a), equals(y))
})

test_that('parameters setters and getters work', {
  
  a <- SVM( lib = "libsvm",
            kernel = "linear",
            prep = "none",
            C = 1,
            gamma = 0,
            coef0 = 0,
            degree = 1,
            shrinking = TRUE,
            probability = FALSE,
            cache_size = 100)
  
  # check getters
  expect_that( params.lib(a), equals("libsvm"))
  expect_that( params.kernel(a), equals("linear"))
  expect_that( params.preprocess(a), equals("none"))
  expect_that( params.C(a), equals(1) )
  expect_that( params.gamma(a), equals(0) )
  expect_that( params.coef0(a), equals(0) )
  expect_that( params.degree(a), equals(1) )
  expect_that( params.shrinking(a), equals(TRUE))
  expect_that( params.probability(a), equals(FALSE))
  expect_that( params.cache_size(a), equals(100))
  
  # check setters
   # params.lib("light") // not yet
  params.kernel(a, "rbf")
  params.preprocess(a, "2e")
  params.C(a,10)
  params.gamma(a,0.01)
  params.coef0(a,1)
  params.degree(a,2)
  params.shrinking(a,FALSE)
  params.probability(a,TRUE)
  params.cache_size(a,1000)
  
  expect_that( params.kernel(a), equals("rbf"))
  expect_that( params.preprocess(a), equals("2e"))
  expect_that( params.C(a), equals(10) )
  expect_that( params.gamma(a), equals(0.01) )
  expect_that( params.coef0(a), equals(1) )
  expect_that( params.degree(a), equals(2) )
  expect_that( params.shrinking(a), equals(FALSE))
  expect_that( params.probability(a), equals(TRUE))
  expect_that( params.cache_size(a), equals(1000))
  
  #check multiple
  params(a, 
         kernel="linear",
         preproces="none",
         C = 1,
         gamma = 0,
         coef0 = 0,
         degree = 1,
         shrinking = TRUE,
         probability = FALSE,
         cache_size = 100)
  
  expect_that( params.lib(a), equals("libsvm"))
  expect_that( params.kernel(a), equals("linear"))
  expect_that( params.preprocess(a), equals("none"))
  expect_that( params.C(a), equals(1) )
  expect_that( params.gamma(a), equals(0) )
  expect_that( params.coef0(a), equals(0) )
  expect_that( params.degree(a), equals(1) )
  expect_that( params.shrinking(a), equals(TRUE))
  expect_that( params.probability(a), equals(FALSE))
  expect_that( params.cache_size(a), equals(100))
  
})

test_that('getters and setters are foulproof', {
  
  svm <- SVM()
  
  expect_error(params.lib(svm, "asd"), "No such library. Avaiable are: libsvm")
  expect_error(params.kernel(svm, "asd"), "No such kernel type. Avaiable are: linear, poly, rbf, sigmoid")
  expect_error(params.preprocess(svm, "asd"), "No such preprocess type, Avaiable are: 2e, none")
})



