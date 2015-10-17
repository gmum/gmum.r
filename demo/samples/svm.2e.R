#' ---
#' title: "Two Ellipsoid SVM preprocessing technique"
#' author: ""
#' date: ""
#' output:
#'  html_document:
#'    self_contained: false
#' ---

# For linear kernel gmum.R provides new preprocessing technique
# called Two Ellipsoid SVM, which accounts for different covariances
# of classes
library(gmum.r)
library(caret)
data(svm_breast_cancer_dataset)
ds <- svm.breastcancer.dataset

# We can trigger 2e by passing prep parameter. It can work with either of the libraries
svm.2e <- SVM(x=ds[,-1], y=ds[,1], core="libsvm", kernel="linear", prep = "2e", C=10);
acc.2e <- svm.accuracy(prediction=predict(svm.2e, ds[,-1]), target=ds[,1])


svm <- SVM(x=ds[,-1], y=ds[,1], core="libsvm", kernel="linear", C=10);
acc <- svm.accuracy(prediction=predict(svm, ds[,-1]), target=ds[,1])

# Seems that 2e preprocessing helps! 

print(paste("LIBSVM + linear kernel accuracy: ", round(acc,3), sep=""))
print(paste("LIBSVM + linear kernel + 2e accuracy: ", round(acc.2e,3), sep=""))

plot(svm.2e, mode="pca")
plot(svm, mode="pca")

# We can also perform rigorous CV on breast cancer dataset

ds$X1 <- as.factor(ds$X1) # Caret requires passing factor for classification :)
inTraining <- createDataPartition(ds$X1, p = .75, list = FALSE)
training <- ds[ inTraining,]
testing  <- ds[-inTraining,]

fitControl <- trainControl(method = "cv",
                           ## 8-fold CV...
                           number = 8,
                           repeats = 1,
                           verboseIter=FALSE
)

# First fit standard linear SVM using LIBSVM
fit.results <- train(X1 ~ ., data = training,
                     method = caret.gmumSvmLinear,
                     preProc = c("center", "scale"),
                     tuneLength = 8,       
                     seed = 777,
                     trControl = fitControl,
                     tuneGrid = expand.grid(C=10^(c(-3:3))),
                     core = "libsvm", # gmum.R parameter - pick library
                     verbosity = 0 # no outputs
)

# Now use 2e preprocessing
fit.results.2e <- train(X1 ~ ., data = training,
                        method = caret.gmumSvmLinear,
                        preProc = c("center", "scale"),
                        tuneLength = 8,             
                        trControl = fitControl,
                        seed = 777,
                        tuneGrid = expand.grid(C=10^(c(-3:3))),
                        core = "libsvm", # gmum.R parameter - pick library
                        verbosity = 0, # no outputs
                        prep = "2e" # Use 2e preprocessing
)

acc <- svm.accuracy(predict(fit.results$finalModel, testing[,-1]), testing[,1])
acc.2e <- svm.accuracy(predict(fit.results.2e$finalModel, testing[,-1]), testing[,1])

print(paste("LIBSVM + linear kernel 8 CV accuracy: ", round(acc,3), sep=""))
print(paste("LIBSVM + linear kernel + 2e 8 CV accuracy: ", round(acc.2e,3), sep=""))

