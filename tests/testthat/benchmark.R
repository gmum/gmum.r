data(svm_breast_cancer_dataset)
ds <- svm.breastcancer.dataset
x <- subset(ds, select = -X1)
y <- as.matrix(ds['X1'])
formula <- X1 ~ .

print("0. linear kernel:")

library(e1071)
e_svm <- e1071::svm(formula, data=ds, type='C-classification', kernel="linear", cost=1, gamma=1)
e_pred <- as.numeric(as.matrix(fitted(e_svm)))

library(gmum.r)
glib_svm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="linear", C=1, verbosity=0)
glight_svm <- gmum.r::SVM(formula, ds, lib="svmlight", kernel="linear", C=1)
glib_esvm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="linear", prep="2e", C=1, verbosity=0)

glib_pred <- predict(glib_svm, x)
glight_pred <- predict(glight_svm, x)
glib_epred <- predict(glib_esvm, x)

acc <- svm.accuracy(e_pred, y)
print(sprintf("e1071 acc: %f", acc))
acc <- svm.accuracy(glib_pred, y)
print(sprintf("gmum libsvm acc: %f", acc))
acc <- svm.accuracy(glight_pred, y)
print(sprintf("gmum svmlight acc: %f", acc))
acc <- svm.accuracy(glib_epred, y)
print(sprintf("gmum libsvm 2e acc: %f", acc))

print ("1. poly kernel:")

e_svm <- e1071::svm(formula, data=ds, type='C-classification', kernel="poly", cost=1, gamma=1)
glib_svm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="poly", C=1, verbosity=0, gamma=1)
glight_svm <- gmum.r::SVM(formula, ds, lib="svmlight", kernel="poly", C=1, gamma=1)
glib_esvm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="poly", prep="2e", C=1, verbosity=0)

e_pred <- as.numeric(as.matrix(fitted(e_svm)))
glib_pred <- predict(glib_svm, x)
glight_pred <- predict(glight_svm, x)
glib_epred <- predict(glib_esvm, x)

acc <- svm.accuracy(e_pred, y)
print(sprintf("e1071 acc: %f", acc))
acc <- svm.accuracy(glib_pred, y)
print(sprintf("gmum libsvm acc: %f", acc))
acc <- svm.accuracy(glight_pred, y)
print(sprintf("gmum svmlight acc: %f", acc))
acc <- svm.accuracy(glib_epred, y)
print(sprintf("gmum libsvm 2e acc: %f", acc))

print ("2. rbf kernel:")

e_svm <- e1071::svm(formula, data=ds, type='C-classification', kernel="radial", cost=1, gamma=1)
glib_svm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="rbf", C=1, verbosity=0, gamma=1)
glight_svm <- gmum.r::SVM(formula, ds, lib="svmlight", kernel="rbf", C=1, gamma=1)
glib_esvm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="rbf", prep="2e", C=1, verbosity=0)

e_pred <- as.numeric(as.matrix(fitted(e_svm)))
glib_pred <- predict(glib_svm, x)
glight_pred <- predict(glight_svm, x)
glib_epred <- predict(glib_esvm, x)

acc <- svm.accuracy(e_pred, y)
print(sprintf("e1071 acc: %f", acc))
acc <- svm.accuracy(glib_pred, y)
print(sprintf("gmum libsvm acc: %f", acc))
acc <- svm.accuracy(glight_pred, y)
print(sprintf("gmum svmlight acc: %f", acc))
acc <- svm.accuracy(glib_epred, y)
print(sprintf("gmum libsvm 2e acc: %f", acc))

print ("3. sigmoid kernel:")

e_svm <- e1071::svm(formula, data=ds, type='C-classification', kernel="sigmoid", cost=1, gamma=1)
glib_svm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="sigmoid", C=1, verbosity=0)
glight_svm <- gmum.r::SVM(formula, ds, lib="svmlight", kernel="sigmoid", C=1)
glib_esvm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="sigmoid", prep="2e", C=1, verbosity=0)

e_pred <- as.numeric(as.matrix(fitted(e_svm)))
glib_pred <- predict(glib_svm, x)
glight_pred <- predict(glight_svm, x)
glib_epred <- predict(glib_esvm, x)

acc <- svm.accuracy(e_pred, y)
print(sprintf("e1071 acc: %f", acc))
acc <- svm.accuracy(glib_pred, y)
print(sprintf("gmum libsvm acc: %f", acc))
acc <- svm.accuracy(glight_pred, y)
print(sprintf("gmum svmlight acc: %f", acc))
acc <- svm.accuracy(glib_epred, y)
print(sprintf("gmum libsvm 2e acc: %f", acc))

