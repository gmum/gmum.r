#I HATE R

# General results: SVMLight lack of control over convergence time, by default much longer convergence in RBF case

# Tested: cv time, final model time, accuracy, #sv, cv scores similarity

library(e1071)
library(doMC)
registerDoMC() # parallel foreach
library(caret)
library(SparseM)
library(mlbench)
library(knitr)
library(klaR)
#source("caret.svm.models.R")


# Download libsvm datasets to data_local folder:
#splice

datasets = lapply(list.files(path="data_local"), function(x){ ds=read.matrix.csr(paste("data_local",x,sep="/")); ds$name=x; ds})

ds=datasets[[1]]

data <- as.data.frame(as.matrix(ds$x))
data$y = ds$y
class.column.index <- which(colnames(data)=='y')
class.column.name <- "y"

p = 0.66
inTraining <- 1:round(nrow(data) * p)
training <- data[ inTraining,]
testing  <- data[-inTraining,]


for (i in 1:5 ) {

  C <- 1
  model <- SVM(y ~ ., training, kernel="linear", C=C, core="svmlight")

  l <-c()
  row_length <- 0
  class.column.index < which(colnames(data)==class.column.name)
  pred_test <- predict(model, subset(testing, select=-c(class.column.index)))
  pred_train <- predict(model, subset(training, select=-c(class.column.index)))
  cf_test <- confusionMatrix(pred_test, testing[, class.column.index])
  cf_train <- confusionMatrix(pred_train, training[, class.column.index])
  
  score_row = c("svlight", as.numeric(cf_train$overall[1]), as.numeric(cf_test$overall[1]), model$getNumberSV())
  print(score_row)
}