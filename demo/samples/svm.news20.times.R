#' ---
#' title: "Comparing times of gmum.r SVM and e1071 on large data"
#' author: ""
#' date: ""
#' output:
#'  html_document:
#'    self_contained: false
#' ---
#' 
library(gmum.r)
library(caret)
library(e1071)

# load the dataset
data_file <- system.file("data_sets", "svm", "large", "news20_part.RData", mustWork=TRUE, package="gmum.r")
load(data_file)

# acquire data and labels
x <- news20.part$x
y <- news20.part$y

# the dataset has pretty high dimensionality
print(ncol(x))

# prepare vectors for saving evaluation times
lens <- c()
gmum_train_times <- c()
e_train_times <- c()
gmum_test_times <- c()
e_test_times <- c()

# run the main loop
for (p in seq(0.01, 0.3, 0.01)){  
  # partitom the data
  train <- as.integer(createDataPartition(y, p=p, list=FALSE))
  
  # save number of traning examples
  lens <- c(lens, length(train))
  
  # train gmum.r SVM
  train_start <- proc.time()
  gmum_svm <- SVM(x[train],y[train], core="svmlight", kernel="linear", C=1, gamma=1)
  gmum_train <- as.numeric((proc.time() - train_start)[3])
  
  # calculate gmum.r training time
  gmum_train_times <- c(gmum_train_times, gmum_train)
  
  # train e1071 SVM
  train_start <- proc.time()
  e_svm <- e1071::svm(x=x[train], y=y[train], type='C-classification', kernel="linear", cost=1, gamma=1, scale=FALSE, fitted=FALSE)
  e_train <- as.numeric((proc.time() - train_start)[3])
  
  # calculate e1071 training time
  e_train_times <- c(e_train_times, e_train)
  
  # test gmum.r SVM
  test_start <- proc.time()
  gmum_pred <- predict(gmum_svm, x[-train])
  gmum_test <- as.numeric((proc.time() - test_start)[3])
  
  # calculate gmum testing
  gmum_test_times <- c(gmum_test_times, gmum_test)
  
  # test e1071
  test_start <- proc.time()
  e_pred <- predict(e_svm, x[-train])
  e_test <- as.numeric((proc.time() - test_start)[3])
  
  # and calculate its testing time
  e_test_times <- c(e_test_times, e_test)
}

# reverse testing data as numbers were decreasing
test_lens <- rep(nrow(x), length(lens)) - lens
gmum_test_times <- rev(gmum_test_times)
e_test_times <- rev(e_test_times)

# preapre data for plotting
library(reshape2)
train_times <- melt(data.frame(x=lens, gmum.r=gmum_train_times, e1071=e_train_times), id='x')
test_times <- melt(data.frame(x=test_lens, gmum.r=gmum_test_times, e1071=e_test_times), id='x')

# load plotting library
library(ggplot2)

# and use some ggplot2 magic 
ggplot(data=train_times, aes(x=x, y=value, colour=variable)) + 
  geom_line() +
  geom_text(data=train_times[train_times$x==1452,], aes(hjust=1.1, label=variable, colour=variable)) +
  theme(legend.position="none") + 
  xlab("samples number") +
  ylab("time")
        
ggplot(data=test_times, aes(x=x, y=value, colour=variable)) + 
  geom_line() +
  geom_text(data=test_times[test_times$x==4900,], aes(hjust=1.1, label=variable, colour=variable)) +
  theme(legend.position="none") +
  xlab("samples number") +
  ylab("time")
