library(gmum.r)
library(caret)

liver <- read.table(system.file("inst", "data_sets", "svm", "liver-disorders", mustWork=TRUE, package="gmum.r"), quote="\"")
x <- liver[,-1]
y <- liver[,1]

set.seed(666)
i <- createDataPartition(y, times=1, list=FALSE)

svm <- SVM(x[i, ], y[i], lib="svmlight", kernel="linear", C=10)
p <- predict(svm, x[-i, ])
print(svm.accuracy(p, y[-i]))

wsvm <- SVM(x[i, ], y[i], lib="svmlight", kernel="linear", C=10, class.weights=c(10,1))
wp <- predict(wsvm, x[-i, ])
print(svm.accuracy(wp, y[-i]))