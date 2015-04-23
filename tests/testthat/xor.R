ds <- svm.dataset.xor()
x <- ds[,c(1,2)]
y <- ds[,3]

library(SparseM)
xs <- as(x, "matrix.csr")

svm <- SVM(xs,y, lib="svmlight", kernel="linear", gamma=1)
print("")
print("svmlight sparse: Alphas, SVs")
print(svm$getAlpha())
print(svm$getSV())

svm2 <- SVM(x,y, lib="svmlight", kernel="linear", gamma=1)
print("")
print("svmlight dense: Alphas, SVs")
print(svm2$getAlpha())
print(svm2$getSV())

