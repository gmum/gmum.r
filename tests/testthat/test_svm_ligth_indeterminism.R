library(gmum.r)
set.seed(777)
test_that("svmlight is deterministic: gives always this same number of iterations and SVs for breastcancer", {
  data(svm_breast_cancer_dataset)
  ds <- svm.breastcancer.dataset
  ds[,'X1'] <- as.factor(ds[,'X1'])
  formula <- X1 ~ .
  models <-  sapply(1:10, function(x) svm <- SVM(formula, ds, core="svmlight", kernel="linear", C=100))
  first_iteration = models[[1]]$iterations
  first_n_SVs = models[[1]]$numberSV
  for (model in models) {
    print(model$iterations)
    expect_that(first_iteration == model$iterations, is_true())
    expect_that(first_n_SVs == model$numberSV, is_true())
  }
})
print("Finally svmlight is deterministic (However, not multithreaded safe. #FIXME).")
