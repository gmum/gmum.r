library(gmum.r)
library(caret)
library(testthat)

test_that("svmlight is faster than e1071 on big sparse data", {

data_file <- system.file("data_sets", "svm", "large", "news20_part.RData", mustWork=TRUE, package="gmum.r")

if (!file.exists(data_file)) {
  stop("Missing dataset file. Run `download_large.sh` from `inst/data_sets/svm`.")
}

load(data_file)

x <- news20.part$x
y <- news20.part$y

library(caret)
set.seed(666)
train <- as.integer(createDataPartition(y, p=0.2, list=FALSE))

core <- "svmlight"
kernel <- "linear"

library(e1071)

print("traning on news20...")

train_start <- proc.time()
gmum_svm <- SVM(x[train],y[train], core=core, kernel=kernel, C=1, gamma=1)
gmum_train <- as.numeric((proc.time() - train_start)[3])

if (kernel=="rbf") kernel <- "radial"
train_start <- proc.time()
e_svm <- e1071::svm(x=x[train], y=y[train], type='C-classification', kernel=kernel, cost=1, gamma=1, scale=FALSE, fitted=FALSE)
e_train <- as.numeric((proc.time() - train_start)[3])

print("testing on news20...")

test_start <- proc.time()
gmum_pred <- predict(gmum_svm, x[-train])
gmum_test <- as.numeric((proc.time() - test_start)[3])

test_start <- proc.time()
e_pred <- predict(e_svm, x[-train])
e_test <- as.numeric((proc.time() - test_start)[3])

gmum_acc <- svm.accuracy(y[-train], gmum_pred)
e_acc <- svm.accuracy(y[-train], e_pred)

print(sprintf("gmum core: %s", core))
print(sprintf("kernel: %s", kernel))

print(sprintf("gmum  acc: %.4f", gmum_acc))
print(sprintf("e1071 acc: %.4f", e_acc))

print(sprintf("gmum  train time: %.4f", gmum_train))
print(sprintf("e1071 train time: %.4f", e_train))

print(sprintf("gmum  test time: %.4f", gmum_test))
print(sprintf("e1071 test time: %.4f", e_test))

expect_true(abs(gmum_acc - e_acc) < 1e-2)
expect_true(gmum_train < e_train)
expect_true(gmum_test < e_test)
})

print("test::svm sparse news20 part")
