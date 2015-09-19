library(gmum.r)
ds <- matrix( 
  c(0,1,0,1,0,0,1,1,0,1,1,0),
  ncol=3,
  nrow=4,
  dimnames=list(c(),c("x","y","t")))
x <- ds[,c(1,2)]
y <- as.factor(ds[,3])

library(SparseM)
xs <- as(x, "matrix.csr")


svm <- SVM(xs,y, core="libsvm", kernel="linear", gamma=1, verbosity=6)
print("")
print("svmlight sparse: Alphas, SVs")
print(svm$alpha)
print(svm$SV)
predict(svm, xs)

svm2 <- SVM(x,y, core="svmlight", kernel="linear", gamma=1)
print("")
print("svmlight dense: Alphas, SVs")
print(svm2$alpha)
print(svm2$SV)
