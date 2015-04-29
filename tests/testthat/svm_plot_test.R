library(gmum.r)

# TO USE: ctrl + a -> ctrl + enter

data(svm_two_ellipsoids_dataset)
data(svm_two_circles_dataset)

formula <- V3 ~ .
########## two elipsoids plots #########
ds_2e <- svm.twoellipsoids.dataset

svm_2e <- SVM(formula, ds_2e, core="libsvm", kernel="linear", prep = "2e", C=10);
svm <- SVM(formula, ds_2e, core="libsvm", kernel="linear", prep = "none", C=10);

plot(svm, mode="normal")
plot(svm_2e, mode="normal")
plot(svm_2e, mode="contour")

########## circles plots #########
ds_circles <- svm.twocircles.dataset

svm_2e <- SVM(formula, ds_circles, core="libsvm", kernel="linear", prep = "2e", C=10);
svm <- SVM(formula, ds_circles, core="libsvm", kernel="linear", prep = "none", C=10);

plot(svm, mode="normal") 
plot(svm_2e, mode="normal")
plot(svm_2e, mode="contour")


# multiclass plot

set.seed(666)

centers <- list(c(0,0),  
                c(10,0), 
                c(0,10),
                c(3,3))

sigmas <- list(matrix(c(1, 0, 0, 1), nrow=2), 
               matrix(c(1, 0, 0, 1), nrow=2),  
               matrix(c(1, 0, 0, 1), nrow=2),
               matrix(c(1, 0, 1, 1), nrow=2))  

sizes <- list(100, 100, 100, 101)

n <- length(centers)  
df <- c()
for(i in 1:n){
  df <- rbind(df, cbind(mvrnorm(n=sizes[[i]], mu=centers[[i]], Sigma=sigmas[[i]]), rep(i,sizes[[i]])))
}
df <- data.frame(df)
colnames(df) <- c("x1","x2", "y")

df[,3] <- as.factor(df[,3])

svm <- SVM(x=df[,1:2], y=df[,3], class.type="one.versus.all")

plot(svm)