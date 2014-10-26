library(testthat)
library('gmum.r')

# DEPRECATED !!!

# test_that('accuracy is calculated', {
#   
#   # self trained
#   svm <- SVM()
#   x <- svm$getX()
#   target <- svm$getY()
#   train(svm)
#   result <- predict(svm, x)
#   acc <- accuracy.svm(y=result, t=target)
#   print(acc)
#   
#   expect_that(acc, is_a("numeric"))
#   expect_that(acc <= 1, is_true())
#   expect_that(acc >= 0, is_true())
#   
# })
# 
# test_that('accuracy is calculated with training', {
#   
#   # with training
#   svm <- SVM()
#   target <- svm$getY()
#   acc <- accuracy.svm(object=svm, t=target)
#   print(acc)
#   
#   expect_that(acc, is_a("numeric"))
#   expect_that(acc <= 1, is_true())
#   expect_that(acc >= 0, is_true())
# })
# 
# test_that('accuracy is foulproof', {
#   
#   svm <- SVM()
#   target <- c(1,1,1,1)
#   expect_that(accuracy.svm(object=svm, t=target), throws_error("Result's and target's length dont match!"))
#   expect_that(accuracy.svm(), throws_error("You need to provide either model and target, or result and target"))
#   
# })



