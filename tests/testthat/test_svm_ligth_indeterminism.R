#I HATE R

library(gmum.r)
set.seed(777)#source("caret.svm.models.R")


test_that("svmlight is deterministic: gives always this same number of iterations and SVs for breastcancer", {
  data(svm_breast_cancer_dataset)
  ds <- svm.breastcancer.dataset
  ds[,'X1'] <- as.factor(ds[,'X1'])
  formula <- X1 ~ .
  models <-  sapply(1:10, function(x) svm <- SVM(formula, ds, core="svmlight", kernel="linear", C=100))
  first_iteration = models[[1]]$getIterations()
  first_n_SVs = models[[1]]$getNumberSV()
  for (model in models) {
    print(model$getIterations())
    expect_that(first_iteration == model$getIterations(), is_true())
    expect_that(first_n_SVs == model$getNumberSV(), is_true())
  }
})
print("Finally svmlight is deterministic (However, not multithreaded safe. #FIXME).")
