library(MASS)
library(SparseM)
library(testthat)
library(gmum.r)
library(caret)

test_that("ovo and ova multiclass schemes work for SVM on simple datasets", {
  set.seed(777)
  
  # sciagnij jesli nie masz, z tego co pamietam gmum.r i tak wymaga tej paczki
  
  # funkcja wygeneruje tyle klas ile chcesz
  generate_gaussians <- function(sizes, centers, simgas) {
    
    n <- length(centers)
    if (n != length(sigmas)) stop ("paramters length must match")
    if (n != length(sizes)) stop ("paramters length must match")
    
    df <- c()
    for(i in 1:n){
      df <- rbind(df, cbind(mvrnorm(n=sizes[[i]], mu=centers[[i]], Sigma=sigmas[[i]]), rep(i,sizes[[i]])))
    }
    df <- data.frame(df)
    colnames(df) <- c("x1","x2", "y")
    return(as.matrix(df))
  }
  
  
  # tu przykladowe wywloanie dla 3 klas
  # jesli chcesz wiecej klas -> daj wiecej elementow na kazdej liscie
  
  # srodki gaussow jako lista
  centers <- list(c(0,0),  # srodek pierwszej klasy
                  c(10,0), # drugiej
                  c(0,10),
                  c(3,3)
                    ) # itd.
  
  # macierze kowariancji 2-wymiarowych gaussow, tym mozesz sie pobawic
  # jak chcesz inne ksztalty gaussow
  sigmas <- list(matrix(c(1, 0, 0, 1), nrow=2), # macierz kowariancji pierwszej klasy
                 matrix(c(1, 0, 0, 1), nrow=2),  # drugiej
                 matrix(c(1, 0, 0, 1), nrow=2),
                 matrix(c(1, 0, 1, 1), nrow=2))  # itd.
  
  # rozmiary kazdej klasy, mozna zmienic ale na razie najlepiej testowac na zbalanoswanych danych
  # tzn. kazda klasa ma tyle samo elementow
  sizes <- list(100, 100, 100, 101)
  
  # wywolanie
  df <- generate_gaussians(sizes, centers, sigmas)
  
  
  
  sv <- SVM(x=df[,1:2], y=df[,3], class.type="one.versus.all")
  preds <- predict(sv, df[,1:2])
  acc <- sum(diag(table(preds, df[,3])))/sum(table(preds, df[,3]))
  expect_that(acc > 0.9, is_true())
  
  
  data(iris)
  
  sv.ova <- SVM(Species ~ ., data=iris, class.type="one.versus.all")
  preds <- predict(sv.ova, iris[,1:4])
  acc.ova <- sum(diag(table(preds, iris$Species)))/sum(table(preds, iris$Species))
  
  
  sv.ovo <- SVM(x=iris[,1:4], y=iris[,5], class.type="one.versus.one")
  preds <- predict(sv.ovo, iris[,1:4])
  acc.ovo <- sum(diag(table(preds, iris$Species)))/sum(table(preds, iris$Species))
  
  
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