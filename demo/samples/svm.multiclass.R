# Use SVM to run multiclass prediction (OVA or OVR)
library(gmum.r)

data(iris)

# One versus all is solving K subproblems
sv.ova <- SVM(Species ~ ., data=iris, class.type="one.versus.all")
preds <- predict(sv.ova, iris[,1:4])
acc.ova <- sum(diag(table(preds, iris$Species)))/sum(table(preds, iris$Species))  

# One versus one is solving K(K-1)/2 subproblems (one for each pair)
sv.ovo <- SVM(x=iris[,1:4], y=iris[,5], class.type="one.versus.one")
preds <- predict(sv.ovo, iris[,1:4])
acc.ovo <- sum(diag(table(preds, iris$Species)))/sum(table(preds, iris$Species))

plot(sv.ova)
# OVO schemes gets 2 more examples from
# training set classified correctly
plot(sv.ovo)

