library('gmum.r')

# Create new SVM object
svm <- SVM( lib = "libsvm",
            kernel = "linear",
            prep = "none",
            C = 1,
            gamma = 0,
            coef0 = 0,
            degree = 1,
            shrinking = TRUE,
            probability = FALSE,
            cache_size = 100)

# Load a dataset, here we have provided an example 
dataset <- read.libsvm( breast_cancer.path, 10 )

# All functions are generic, meaning you nedd to provide your model as a first argument
load_dataset(svm, dataset)

# You can access the dataset 
x <- dataset.X(svm)
y <- dataset.Y(svm)

# You can change the dataset, but that requires class methods
svm$setX(x)
svm$setY(y)

# You can change model paramaters using params function for specific parameter
params.C(svm, 100)

# or change multiple parameters at once
params(svm, C=10, gamma=0.1)

# To view specific parameter you can use the same function without a values, or print a summary for your model
params.C(svm)
print(svm)

# Use train(object) function to train your model on provided dataset
train(svm)

# Classify your dataset using predict function
prediction <- predict(svm, x)
