library(gmum.r)
library(caret)

data_file <- system.file("data_sets", "svm", "large", "news20.binary", mustWork=TRUE, package="gmum.r")



if (!file.exists(x_file) || !file.exists(y_file)) {
  stop("Missing dataset file")
}

data <- read.matrix.csr(data_file)
x <- data$x
y <- data$y

i <- as.integer(createDataPartition(y, p=0.25, list=FALSE))

news20.part.x <- x[i]
news20.part.y <- y[i]

news20.part <- list(x=news20.part.x, y=news20.part.y)
save(news20.part, file="news20_part.RData", compress = TRUE)

### After saving data

data_file <- system.file("inst", "data_sets", "svm", "large", "news20_part.RData", mustWork=TRUE, package="gmum.r")
load(data_file)


library(gmum.r)
library(caret)
load("~/Downloads/news20_part.RData")
x <- news20.part$x
y <- news20.part$y

library(caret)
set.seed(777)
train <- as.integer(createDataPartition(y, p=0.1, list=FALSE))

lib <- "svmlight"
kernel <- "linear"

library(e1071)

train_start <- proc.time()
gmum_svm <- SVM(x[train],y[train], lib=lib, kernel=kernel, C=1, gamma=1)
gmum_train <- as.numeric((proc.time() - train_start)[3])

if (kernel=="rbf") kernel <- "radial"
train_start <- proc.time()
e_svm <- e1071::svm(x=x[train], y=y[train], type='C-classification', kernel=kernel, cost=1, gamma=1, scale=FALSE, fitted=FALSE)
e_train <- as.numeric((proc.time() - train_start)[3])


test_start <- proc.time()
gmum_pred <- predict(gmum_svm, x[-train])
gmum_test <- as.numeric((proc.time() - test_start)[3])

test_start <- proc.time()
e_pred <- predict(e_svm, x[-train])
e_test <- as.numeric((proc.time() - test_start)[3])

gmum_acc <- svm.accuracy(y[-train], gmum_pred)
e_acc <- svm.accuracy(y[-train], e_pred)

print(sprintf("gmum lib: %s", lib))
print(sprintf("kernel: %s", kernel))

print(sprintf("gmum  acc: %.4f", gmum_acc))
print(sprintf("e1071 acc: %.4f", e_acc))

print(sprintf("gmum  train time: %.4f", gmum_train))
print(sprintf("e1071 train time: %.4f", e_train))

print(sprintf("gmum  test time: %.4f", gmum_test))
print(sprintf("e1071 test time: %.4f", e_test))







