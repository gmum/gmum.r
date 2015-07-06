library(caret)
library(gmum.r)

K = 10

data(svm_breast_cancer_dataset)

ds = svm.breastcancer.dataset
folds = createFolds(ds$V1, k=K)
formula = V1 ~ .

for (c in seq(-6,5,1) ){
  
  mean_acc = 0
  mean_twoe_acc = 0
  
  for ( i in seq(1,K,1) ) {
    
    train = ds[-folds[[i]],]
    test = ds[folds[[i]],]
    
    svm <- SVM(formula, train, core="libsvm", kernel="linear", prep = "none", C=10^c);
    twoe_svm <- SVM(formula, train, core="libsvm", kernel="linear", prep = "2e", C=10^c);
    
    print("----------")
    
    test_x = subset(test, select = -c(V1))
    target = test[,"V1"]
    
    pred <- predict(svm, test_x)
    twoe_pred <- predict(twoe_svm, test_x)
    
    acc <- svm.accuracy(prediction=pred, target=target)
    twoe_acc <- svm.accuracy(prediction=twoe_pred, target=target)
    
    mean_acc = mean_acc + acc
    mean_twoe_acc = mean_twoe_acc + twoe_acc
    
  }
  
  print(sprintf("mean SVM acc with C=%f after %i folds: %f ", c, K, mean_acc/K))
  print(sprintf("mean 2eSVM acc with C=%f after %i folds: %f ", c, K, mean_twoe_acc/K))
}

