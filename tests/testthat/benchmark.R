library(e1071)
library(klaR)
library(gmum.r)

data(svm_breast_cancer_dataset)
ds <- svm.breastcancer.dataset
x <- subset(ds, select = -X1)
y <- as.matrix(ds['X1'])
formula <- X1 ~ .

# Linear kernel

e_svm <- e1071::svm(formula, data=ds, type='C-classification', kernel="linear", cost=1, gamma=1, scale=FALSE)
glib_svm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="linear", C=1, verbosity=0)
glight_svm <- gmum.r::SVM(formula, ds, lib="svmlight", kernel="linear", C=1)
klar_svmlight <- klaR::svmlight(formula, data=ds, type="C", svm.options="-v 0")
glib_esvm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="linear", prep="2e", C=1, verbosity=0)
e_pred <- as.numeric(as.matrix(fitted(e_svm)))
glib_pred <- predict(glib_svm, x)
glight_pred <- predict(glight_svm, x)
# NOTE: No idea how to mute this... (tried: invisible, capture.output, sink)
klar_svmlight_pred <- as.numeric(as.matrix(predict(klar_svmlight, x)$class))
glib_epred <- predict(glib_esvm, x)

print("0. linear kernel:")

acc <- svm.accuracy(e_pred, y)
print(sprintf("e1071 acc: %f", acc))
acc <- svm.accuracy(glib_pred, y)
print(sprintf("gmum libsvm acc: %f", acc))
acc <- svm.accuracy(klar_svmlight_pred, y)
print(sprintf("klaR svmlight acc: %f", acc))
acc <- svm.accuracy(glight_pred, y)
print(sprintf("gmum svmlight acc: %f", acc))
acc <- svm.accuracy(glib_epred, y)
print(sprintf("gmum libsvm 2e acc: %f", acc))

# Poly kernel

e_svm <- e1071::svm(formula, data=ds, type='C-classification', kernel="poly", cost=1, gamma=1, scale=FALSE)
glib_svm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="poly", C=1, verbosity=0, gamma=1)
glight_svm <- gmum.r::SVM(formula, ds, lib="svmlight", kernel="poly", C=1, gamma=1, coef0=1)
klar_svmlight <- klaR::svmlight(formula, data=ds, type="C", svm.options="-v 0 -t 1 -s 1 -r 1")
glib_esvm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="poly", prep="2e", C=1, verbosity=0)

e_pred <- as.numeric(as.matrix(fitted(e_svm)))
glib_pred <- predict(glib_svm, x)
glight_pred <- predict(glight_svm, x)
klar_svmlight_pred <- as.numeric(as.matrix(predict(klar_svmlight, x)$class))
glib_epred <- predict(glib_esvm, x)

print ("1. poly kernel:")

acc <- svm.accuracy(e_pred, y)
print(sprintf("e1071 acc: %f", acc))
acc <- svm.accuracy(glib_pred, y)
print(sprintf("gmum libsvm acc: %f", acc))
acc <- svm.accuracy(klar_svmlight_pred, y)
print(sprintf("klaR svmlight acc: %f", acc))
acc <- svm.accuracy(glight_pred, y)
print(sprintf("gmum svmlight acc: %f", acc))
acc <- svm.accuracy(glib_epred, y)
print(sprintf("gmum libsvm 2e acc: %f", acc))

# RBF kernel

e_svm <- e1071::svm(formula, data=ds, type='C-classification', kernel="radial", cost=1, gamma=1, scale=FALSE)
glib_svm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="rbf", C=1, verbosity=0, gamma=1)
glight_svm <- gmum.r::SVM(formula, ds, lib="svmlight", kernel="rbf", C=1, gamma=1)
klar_svmlight <- klaR::svmlight(formula, data=ds, type="C", svm.options="-v 0 -t 2 -s 1 -r 1")
glib_esvm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="rbf", prep="2e", C=1, verbosity=0)

e_pred <- as.numeric(as.matrix(fitted(e_svm)))
glib_pred <- predict(glib_svm, x)
glight_pred <- predict(glight_svm, x)
klar_svmlight_pred <- as.numeric(as.matrix(predict(klar_svmlight, x)$class))
glib_epred <- predict(glib_esvm, x)

print ("2. rbf kernel:")

acc <- svm.accuracy(e_pred, y)
print(sprintf("e1071 acc: %f", acc))
acc <- svm.accuracy(glib_pred, y)
print(sprintf("gmum libsvm acc: %f", acc))
acc <- svm.accuracy(klar_svmlight_pred, y)
print(sprintf("klaR svmlight acc: %f", acc))
acc <- svm.accuracy(glight_pred, y)
print(sprintf("gmum svmlight acc: %f", acc))
acc <- svm.accuracy(glib_epred, y)
print(sprintf("gmum libsvm 2e acc: %f", acc))

# Sigmoid kernel

e_svm <- e1071::svm(formula, data=ds, type='C-classification', kernel="sigmoid", cost=1, gamma=1, scale=FALSE)
glib_svm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="sigmoid", C=1, verbosity=0)
glight_svm <- gmum.r::SVM(formula, ds, lib="svmlight", kernel="sigmoid", C=1)
# -r and -s are sigmoid kernel parameters. 1/dimensions = 1/11 = 0.09090909
klar_svmlight <- klaR::svmlight(formula, data=ds, type="C", svm.options="-v 0 -t 3 -r -1 -s 0.09090909")
glib_esvm <- gmum.r::SVM(formula, ds, lib="libsvm", kernel="sigmoid", prep="2e", C=1, verbosity=0)

e_pred <- as.numeric(as.matrix(fitted(e_svm)))
glib_pred <- predict(glib_svm, x)
glight_pred <- predict(glight_svm, x)
klar_svmlight_pred <- as.numeric(as.matrix(predict(klar_svmlight, x)$class))
glib_epred <- predict(glib_esvm, x)

print ("3. sigmoid kernel:")

acc <- svm.accuracy(e_pred, y)
print(sprintf("e1071 acc: %f", acc))
acc <- svm.accuracy(glib_pred, y)
print(sprintf("gmum libsvm acc: %f", acc))
acc <- svm.accuracy(klar_svmlight_pred, y)
print(sprintf("klaR svmlight acc: %f", acc))
acc <- svm.accuracy(glight_pred, y)
print(sprintf("gmum svmlight acc: %f", acc))
acc <- svm.accuracy(glib_epred, y)
print(sprintf("gmum libsvm 2e acc: %f", acc))

