#' ---
#' title: "SVMLight transductive learning"
#' author: ""
#' date: ""
#' output:
#'  html_document:
#'    self_contained: false
#' ---


# We enable different unique features, for instance transductive learning in SVMLight 
library(gmum.r)
library(e1071)

data(svm.transduction)

# Prepare data
train.transduction <- svm.transduction$tr
train.induction <- svm.transduction$ind
test <-svm.transduction$test
test$x <- train.transduction$x[11:610, ] # Saves some space :)

svm.induction <- SVM(x=train.induction$x, y=train.induction$y, core="svmlight")
svm.induction.pred <- predict(svm.induction, test$x)

# We pass special labels for transductive learning
train.transduction$y[10:12]

# Train transductive, it takes longer as it uses much more data
svm.transduction <- SVM(x=train.transduction$x, y=train.transduction$y,
                        transductive.learning=TRUE,
                        core="svmlight")
svm.transduction.pred <- predict(svm.transduction, test$x)

# As expected - transductively trained model performs much better on test data
svm.accuracy(svm.induction.pred, test$y)
svm.accuracy(svm.transduction.pred, test$y)

# Plot projection of data onto first 2 dimensions
plot(svm.transduction, X=test$x)
plot(svm.induction, X=test$x)

