library('gmum.r')

# TO USE: ctrl + a -> ctrl + enter

formula = V3 ~ .


########## two elipsoids plots #########
ds_2e = svm.dataset.2e()

svm_2e <- SVM(formula, ds_2e, lib="libsvm", kernel="linear", prep = "2e", C=10);
svm <- SVM(formula, ds_2e, lib="libsvm", kernel="linear", prep = "none", C=10);

plot(svm, mode="normal")
plot(svm_2e, mode="normal")
plot(svm_2e, mode="contour")

########## circles plots #########
ds_circles = svm.dataset.circles()

svm_2e <- SVM(formula, ds_circles, lib="libsvm", kernel="linear", prep = "2e", C=10);
svm <- SVM(formula, ds_circles, lib="libsvm", kernel="linear", prep = "none", C=10);

plot(svm, mode="normal") 
plot(svm_2e, mode="normal")
plot(svm_2e, mode="contour")