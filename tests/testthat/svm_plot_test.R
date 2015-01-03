library('gmum.r')

# TO USE: ctrl + a -> ctrl + enter

formula = V3 ~ .


########## two elipsoids plots #########
ds_2e = svm.dataset.2e()

svm_2e <- SVM(formula, ds_2e, lib="libsvm", kernel="linear", prep = "2e", C=10);
svm <- SVM(formula, ds_2e, lib="libsvm", kernel="linear", prep = "none", C=10);

plot(svm, mode="normal") + scale_x_continuous(limits=c(-10, 10)) + scale_y_continuous(limits=c(-10, 10)) + ggtitle("SVM on two elipsoids")
plot(svm_2e, mode="normal") + scale_x_continuous(limits=c(-10, 10)) + scale_y_continuous(limits=c(-10, 10)) + ggtitle("2e SVM on two elipsoids")

plot(svm_2e, mode="test")+ scale_x_continuous(limits=c(-10, 10)) + scale_y_continuous(limits=c(-10, 10)) + ggtitle("Contour + 2eSVM on two elipsoids")

########## circles plots #########
ds_circles = svm.dataset.circles()

svm_2e <- SVM(formula, ds_circles, lib="libsvm", kernel="linear", prep = "2e", C=10);
svm <- SVM(formula, ds_circles, lib="libsvm", kernel="linear", prep = "none", C=10);

plot(svm, mode="normal") + scale_x_continuous(limits=c(-9, 9)) + scale_y_continuous(limits=c(-9, 9)) + ggtitle("SVM on circles")
plot(svm_2e, mode="normal") + scale_x_continuous(limits=c(-9, 9)) + scale_y_continuous(limits=c(-9, 9)) + ggtitle("2e SVM on circles")

plot(svm_2e, mode="test")+ scale_x_continuous(limits=c(-9, 9)) + scale_y_continuous(limits=c(-9, 9)) + ggtitle("Contour + 2eSVM on circles")