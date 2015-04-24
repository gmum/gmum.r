library(testthat)

data(svm_two_ellipsoids_dataset)
data(svm_breast_cancer_dataset)

libraries <- c("libsvm", "svmlight")

test_that("2e svm works with 2e dataset", {
  ds <- svm.twoellipsoids.dataset
  formula <- V3 ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", prep = "2e", C=10);

  x <- svm$getX()
  target <- svm$getY()

  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=target)
  expect_that(acc>0.5, is_true())
})
print("test::2eSVM works with 2e dataset")

test_that("2e svm works with breast cancer dataset", {

  ds <- svm.breastcancer.dataset
  formula <- X1 ~ .
  svm <- SVM(formula, ds, lib="libsvm", kernel="linear", prep = "2e", C=10);

  x <- svm$getX()
  target <- svm$getY()

  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=target)

  expect_that(acc>0.5, is_true())
})
print("test::2eSVM works with breast cancer dataset")

test_that("2e svm works better than normal SVM with breast cancer dataset", {

    for (lib_i in libraries) {

        print("####### Start 2e vs normal svm #########")
        print(sprintf("Library: %s", lib_i))
        ds <- svm.breastcancer.dataset
        ds2 <- svm.breastcancer.dataset
        formula <- X1 ~ .

        svm <- SVM(
            formula, ds, lib=lib_i, kernel="linear", prep = "none", C=1.5);

        x <- svm$getX()
        target <- svm$getY()

        pred <- predict(svm, x)
        acc <- svm.accuracy(prediction=pred, target=target)
        print(sprintf("SVM acc: %f", acc))

        print("Learning...")
        twoe_svm <- SVM(
            formula, ds, lib=lib_i, kernel="linear", prep = "2e", C=1.5);
        print("Prediction...")
        twoe_pred <- predict(twoe_svm, x)
        twoe_acc <- svm.accuracy(prediction=twoe_pred, target=target)
        print(sprintf("2eSVM acc: %f", twoe_acc))

        print("####### End 2e vs normal svm #########")
        print("")

    }
})

