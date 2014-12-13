library('gmum.r')

formula = V3 ~ .


########## two elipsoids plots #########
ds_2e = svm.dataset.2e()

svm_2e <- SVM(formula, ds_2e, lib="libsvm", kernel="linear", prep = "2e", C=10);
svm <- SVM(formula, ds_2e, lib="libsvm", kernel="linear", prep = "none", C=10);

plot(plot(svm, pca=FALSE), main = "SVM on two elipsoids")
plot(plot(svm_2e, pca=FALSE), main = "2e SVM on two elipsoids")


########## circles plots #########
ds_circles = svm.dataset.circles()

svm_2e <- SVM(formula, ds_circles, lib="libsvm", kernel="linear", prep = "2e", C=10);
svm <- SVM(formula, ds_circles, lib="libsvm", kernel="linear", prep = "none", C=10);

plot(plot(svm, pca=FALSE), main = "SVM on two circles")
plot(plot(svm_2e, pca=FALSE), main = "2e SVM on two circles")
