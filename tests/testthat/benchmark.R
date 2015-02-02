data(svm_breast_cancer_dataset)
ds <- svm.breastcancer.dataset
x <- subset(ds, select = -V1)
y <- as.matrix(ds['V1'])
formula <- V1 ~ .

library(e1071)
e_svm <- e1071::svm(formula, data=ds, type='C-classification', kernel="linear", cost=1, gamma=1)
e_pred <- as.numeric(as.matrix(fitted(e_svm)))

library(gmum.r)
glib_svm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="linear", C=1, verbosity=0, gamma=1)
glight_svm <- gmum.r::SVM(formula, ds, lib="svmlight", kernel="linear", C=1, gamma=1)
glib_esvm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="linear", prep="2e", C=1, verbosity=0)
# glight_esvm <- gmum.r::SVM(formula, ds, lib="svmlight", kernel="linear", prep="2e", C=1)

glib_pred <- predict(glib_svm, x)
glight_pred <- predict(glight_svm, x)
glib_epred <- predict(glib_esvm, x)
# glight_epred <- predict(glight_esvm, x)

acc <- svm.accuracy(e_pred, y)
print(sprintf("e1071 acc: %f", acc))

acc <- svm.accuracy(glib_pred, y)
print(sprintf("gmum libsvm acc: %f", acc))

acc <- svm.accuracy(glight_pred, y)
print(sprintf("gmum svmlight acc: %f", acc))

acc <- svm.accuracy(glib_epred, y)
print(sprintf("gmum libsvm 2e acc: %f", acc))

# NOTE: Accuracy 0.650073 is a bug of all negative answers (label = 2)

# acc <- svm.accuracy(glight_epred, y)
# print(sprintf("gmum svmlight 2e acc: %f", acc))
