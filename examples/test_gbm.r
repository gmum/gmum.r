

library(mlbench)
data(Sonar)
library(caret)
set.seed(998)
inTraining <- createDataPartition(Sonar$Class, p = 0.75, list = FALSE)
training <- Sonar[inTraining, ]
testing <- Sonar[-inTraining, ]

library(gmum.r)
library(caret)
gbmFit1 <- train(Class ~ ., data = training, method = "gbm")





library('caret')
library('devtools')
devtools::install(".")
devtools::load_all(".")
library('gmum.r')
# Create new SVM object
svm <- SVM(lib = "libsvm",
           kernel = "linear",
           prep = "none",
           C = 1,
           gamma = 0.01,
           coef0 = 0) 

# Load a dataset, here we have provided an example 
ds <- read.libsvm( breast_cancer.path, 10 )

# All functions are generic, meaning you nedd to provide your model as a first argument
load_dataset(svm, ds)

# You can access the dataset 
x <- dataset.X(svm)
y <- dataset.Y(svm)

# You can change the dataset, but that requires class methods
svm$setX(x)
svm$setY(y)

# You can change model paramaters using params function for specific parameter
params.C(svm, 100)

# or change multiple parameters at once
params(svm, C=1, gamma=0.01)

# To view specific parameter you can use the same function without a values, or print a summary for your model
params.C(svm)
print(svm)

# Use train(object) function to train your model on provided dataset
train(svm)

# Classify your dataset using predict function
prediction <- predict(svm, x)

prediction

plot.svm(svm, 1, 2)

class(svm)


print(svm)

c = data.frame()

print(c)

plot(svm)


