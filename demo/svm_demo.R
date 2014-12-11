library('gmum.r')

# Load a dataset, here we have provided an example 
ds <- svm.dataset.breast_cancer()

# Create new SVM object
svm <- SVM( formula = X1~. ,
            data = ds,
            lib = "libsvm",
            kernel = "linear",
            prep = "none",
            C = 1,
            gamma = 0.01,
            coef0 = 0) 

# You can access the dataset 
x <- dataset.X(svm)
y <- dataset.Y(svm)

# You can print short summary of your SVM object
summary(svm)

# Classify your dataset using predict function
prediction <- predict(svm, x)

# Check models accuracy
acc <- svm.accuracy(prediction=y, target=prediction)
