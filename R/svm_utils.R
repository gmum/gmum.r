svm.dataset.breast_cancer <- function() {
  data(svm_breast_cancer_dataset)
  return (svm.breastcancer.dataset)
}

svm.dataset.2e <- function() {
  data(svm_two_ellipsoids_dataset)
  return(svm.twoellipsoids.dataset)
}

svm.dataset.circles <- function() {
  data(svm_two_circles_dataset)
  return(svm.twocircles.dataset)
}

svm.dataset.xor <- function() {
  matrix( 
    c(0,1,0,1,0,0,1,1,0,1,1,0),
    ncol=3,
    nrow=4,
    dimnames=list(c(),c("x","y","t")))
}

svm.accuracy <- function(prediction, target) {
  if ( length(target) != length(prediction)) {
    stop("Prediction's and target's length don't match!")
  }
  len <- length(target)
  
  diff = target-prediction
  acc <- sum(diff == 0) / len
  return(acc) 
}