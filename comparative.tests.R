library(e1071)
library(Sparse)
library(caret)
library(SparseM)
library(mlbench)
source("caret.svm.models.R")

# Tested: cv time, final model time, accuracy, #sv, cv scores similarity

# Read binary datasets
datasets = lapply(list.files(path="data_local"), function(x){ ds=read.matrix.csr(paste("data_local",x,sep="/")); ds$name=x; ds})

get.results <- function(data, class.column.name, params, model, seed=777){
  set.seed(seed)
  
  class.column.index <- which(colnames(data)==class.column.name)
  inTraining <- createDataPartition(data[, class.column.index], p = .75, list = FALSE)
  training <- data[ inTraining,]
  testing  <- data[-inTraining,]
  
  
  
  fitControl <- trainControl(method = "cv",
                             ## 10-fold CV...
                             number = 10,
                             ## repeated 2 times
                             repeats = 2,
                             verboseIter=TRUE
  )
  
  
  
  model <- train(formula(paste(class.column.name,"~ .", sep=" ")), data = training,
                 method = model,
                 preProc = c("center", "scale"),
                 tuneLength = 8,
                 tuneGrid = params,               
                 trControl = fitControl)
  
  
  class.column.index < which(colnames(data)==class.column.name)
  pred <- predict(model$finalModel, predict(model$preProcess, subset(testing, select=-c(class.column.index))))
  cf <- confusionMatrix(pred, testing[, class.column.index])
  
  list(trainAcc=max(model$results$Accuracy), 
                 trainAccStd=model$results$AccuracySD[which.max(model$results$Accuracy)], 
                 testAcc=as.numeric(cf$overall[1]),
                 trainTime=as.numeric(model$times$everything[1]),
                 finalModelTime=as.numeric(model$times$final[1]))
}

# Coarse CV grid
C <- 10^(-7:11)
Gamma <- 10^(-9:9)
degree <- c(2,3)
model.names=list("gmum.r::svm.radial", "gmum.r::svm.linear", "gmum.r::svm.poly", 
                 "kernLab::svm.radial", "kernLab::svm.linear", "kernLab::svm.poly")
model.calls=list(gmum.r.svm.radial, gmum.r.svm.linear, gmum.r.svm.poly,
                                "svmRadial", "svm", "svmPoly")
model.tuneGrids = list(expand.grid(C=C, gamma=Gamma), expand.grid(C=C), 
                       expand.grid(C=C, gamma=Gamma, degree=degree),
                       expand.grid(C=C, sigma=Gamma), expand.grid(C=C), 
                       expand.grid(C=C, scale=Gamma,  degree=degree))



ds <- datasets[[2]]
df <- as.data.frame(as.matrix(ds$x))
df$y = ds$y

model.results <- list()
for(model.index in 1:length(model.names)){
    append(model.results,
           get.results(df, "y", model.tuneGrids[[model.index]], model.calls[[model.index]])
           )
}


