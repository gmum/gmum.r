breast_cancer.path = "./data/svm/breast_cancer.data"

svm.breast_cancer.x <- function() {
  bc = read.libsvm( breast_cancer.path, 10 )
  x = bc[,c(2,3,4,5,6,7,8,9,10,11)]
  return(x)
}

svm.breast_cancer.y <- function() {
  bc = read.libsvm( breast_cancer.path, 10 )
  y = bc[,1]
  return(y)
}