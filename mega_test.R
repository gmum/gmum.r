library(e1071)
library(Sparse)
library(caret)
library(mlbench)

# Tested: cv time, final model time, accuracy, #sv, cv scores similarity

# Read binary datasets
datasets = lapply(list.files(path="data_local"), function(x){ ds=read.matrix.csr(paste("data_local",x,sep="/")); ds$name=x; ds})

source("caret.svm.models.R")


data(Sonar)
ds <- Sonar
cls <- ds$Class

set.seed(998)
inTraining <- createDataPartition(cls, p = .75, list = FALSE)
training <- ds[ inTraining,]
testing  <- ds[-inTraining,]

fitControl <- trainControl(method = "cv",
                           ## 10-fold CV...
                           number = 2,
                           ## repeated ten times
                           repeats = 1,
                           verboseIter=TRUE
)

C <- 10^(-7:11)
Gamma <- 10^(-10:10)
params <- expand.grid(C=C, gamma=Gamma)

model <- train(Class ~ ., data = training,
               method = gmum.r.svm.radial,
               preProc = c("center", "scale"),
               tuneLength = 8,
               tuneGrid = params,               
               trControl = fitControl)
pred <- predict(model$finalModel,testing[,1:60] )
cf <- confusionMatrix(pred, testing$Class)
cf$overall


results = list(acc=model$results$Accuracy, trainTime=as.numeric(model$times$everything[1]),
               finalModelTime=as.numeric(model$times$final[1]))

model2 <- train(Class ~ ., data = training,
                method = "svmRadial",
                preProc = c("center", "scale"),
                tuneLength = 8,
                tuneGrid = expand.grid(C=C, sigma=Gamma),               
                trControl = fitControl, scale=FALSE)
max(model2$results$Accuracy)
cf2 <- confusionMatrix(predict(model2$finalModel,testing[,1:60] ), testing$Class)
cf2$overall



plot(model, plotType="level")
plot(model2, plotType="level")
