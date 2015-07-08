library(MASS)
library(SparseM)
library(testthat)
library(gmum.r)
library(caret)

{
  set.seed(777)
  
  centers <- list(c(0,0),  
                  c(10,0), 
                  c(0,10),
                  c(3,3))
  
  sigmas <- list(matrix(c(1, 0, 0, 1), nrow=2), 
                 matrix(c(1, 0, 0, 1), nrow=2),  
                 matrix(c(1, 0, 0, 1), nrow=2),
                 matrix(c(1, 0, 1, 1), nrow=2))  
  
  sizes <- list(100, 100, 100, 101)
  
  n <- length(centers)  
  df <- c()
  for(i in 1:n){
    df <- rbind(df, cbind(mvrnorm(n=sizes[[i]], mu=centers[[i]], Sigma=sigmas[[i]]), rep(i,sizes[[i]])))
  }
  df <- data.frame(df)
  colnames(df) <- c("x1","x2", "y")
  
  df[,3] <- as.factor(df[,3])
  
  sv <- SVM(x=df[,1:2], y=df[,3], class.type="one.versus.all")
  preds <- predict(sv, df[,1:2])
  acc <- sum(diag(table(preds, df[,3])))/sum(table(preds, df[,3]))
  #expect_that(acc > 0.96, is_true())
  plot(sv, X=df[,1:2])
  data(iris)
  
  sv.ova <- SVM(Species ~ ., data=iris, class.type="one.versus.all")
  preds <- predict(sv.ova, iris[,1:4])
  acc.ova <- sum(diag(table(preds, iris$Species)))/sum(table(preds, iris$Species))  
  plot(sv.ova)
  
  sv.ovo <- SVM(x=iris[,1:4], y=iris[,5], class.type="one.versus.one")
  preds <- predict(sv.ovo, iris[,1:4])
  acc.ovo <- sum(diag(table(preds, iris$Species)))/sum(table(preds, iris$Species))
  plot(sv.ovo)
  
  e1.sv <- e1071::svm(Species ~., data=iris, kernel='linear')
  preds <- predict(e1.sv, iris[,1:4])
  acc.e1 <- sum(diag(table(preds, iris$Species)))/sum(table(preds, iris$Species))

  expect_that(acc.ova < acc.ovo && abs(acc.e1 - acc.ovo) < 1e-1, is_true())
  
  
  data(fgl)
  X <- fgl[,1:9]
  Y <- fgl[,10]
  
  sv.ova <- SVM(type ~ ., data=fgl, class.type="one.versus.all")
  preds <- as.factor(predict(sv.ova, X))
  acc.ova <- sum(preds==as.factor(Y)) / length(preds)
  
  sv.ovo <- SVM(type ~ ., data=fgl,  class.type="one.versus.one", C=1)
  preds <- predict(sv.ovo, X)
  acc.ovo <- sum(preds==as.factor(Y)) / length(preds)
  
  e1.sv <- e1071::svm(type ~., data=fgl, kernel='linear', C=1)
  preds <- predict(e1.sv, X)
  acc.e1 <- sum(diag(table(preds, Y)))/sum(table(preds, Y))
  
  
  expect_that(acc.ova < acc.ovo && abs(acc.e1 - acc.ovo) < 1e-1, is_true())
}