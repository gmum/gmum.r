library(testthat)

test_that("Basic caret support for SVM works ", {
  library(caret)
  library(mlbench)
  
  set.seed(777)
  
  data(Sonar)
  
  print("Fitting SVM to Sonar dataset")
  
  inTraining <- createDataPartition(Sonar$Class, p = .75, list = FALSE)
  training <- Sonar[ inTraining,]
  testing  <- Sonar[-inTraining,]
  
  
  
  fitControl <- trainControl(method = "cv",
                             ## 10-fold CV...
                             number = 2,
                             ## repeated 2 times
                             repeats = 1,
                             verboseIter=FALSE
  )
  
  
  
  model <- train(Class ~ ., data = training,
                 method = caret.gmumSvmRadial,
                 preProc = c("center", "scale"),
                 tuneLength = 8,             
                 trControl = fitControl,
                 tuneGrid = expand.grid(C=c(1,10,100), gamma=c(0.001, 0.01, 0.1)),
                 verbosity = 0)
  

  expect_that(max(model$results$Accuracy) > 0.7, is_true())
})
print("test::svm caret")

