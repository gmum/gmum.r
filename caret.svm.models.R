library("caret")

copy <- function(x) x

gmum.r.svm.radial.params=c("C", "gamma")
gmum.r.svm.radial.params.classes=c("double", "double")

gmum.r.svm.linear.params=c("C")
gmum.r.svm.linear.params.classes=c("double")

gmum.r.svm.poly.params=c("C", "gamma", "degree", "coef0")
gmum.r.svm.poly.params.classes=c("double", "double", "double", "double")

gmum.r.svm.radial <- list(label = "gmum.r.svmRadial",
                 library = c("gmum.r"),
                 type = "Classification",
                 parameters = data.frame(parameter = gmum.r.svm.radial.params,
                                         class = gmum.r.svm.radial.params.classes,
                                         label = gmum.r.svm.radial.params),
                 grid = function(x, y, len = NULL) {
                   # We pass tuning grid manually. 
                   expand.grid(C=10^(-7:11),
                               gamma= 10^(-10:10))
                 },
                 fit = function(x, y, wts, param, lev, last, classProbs, ...) { 
                   ## First fti the pls model, generate the training set scores,
                   ## then attach what is needed to the random forest object to 
                   ## be used late
                   x.df = as.data.frame(x)
                   x.df$y = as.numeric(y)
                   param$kernel = 'linear'
                   
                   if(is.null(param$gamma)){
                      param$gamma = 1
                   }else{
                      param$kernel = 'rbf'
                   }
                   if(is.null(param$degree)){
                      param$degree = 3
                   }else{
                      param$kernel = 'poly'
                   }
                   if(is.null(param$coef0)){
                      param$coef0 = 0
                   }
                   
                   return(gmum.r::SVM(
                     x,
                     y,
                     C = param$C,
                     gamma = param$gamma,
                     degree = param$degree,
                     coef0 = param$coef0,
                     probability = classProbs,
                     kernel = param$kernel,
                     ...
                   ))
                 },
                 predict = function(modelFit, newdata, submodels = NULL) {  
                   as.factor(predict(modelFit, newdata))
                 },
                 prob = function(modelFit, newdata, submodels = NULL) {  
                   predict(modelFit, newdata)
                 },
                 varImp = NULL,
                 levels = function(x) {levels(x$getY())},
                 sort = function(x) x[order(x[,1]),]
            )

gmum.r.svm.linear <- copy(gmum.r.svm.radial)
gmum.r.svm.poly <- copy(gmum.r.svm.radial)


gmum.r.svm.linear$parameters <- data.frame(parameter = gmum.r.svm.linear.params,
                                                        class = gmum.r.svm.linear.params.classes,
                                                        label = gmum.r.svm.linear.params)

gmum.r.svm.linear$grid <- function(x, y, len = NULL) {
  expand.grid(C=10^(-7:11))
}

gmum.r.svm.poly$grid <- function(x, y, len = NULL) {
  expand.grid(C=10^(-7:11), gamma= 10^(-10:10), coef0=c(0,1,10), degree=c(2,3,4) )
}


gmum.r.svm.poly$parameters <- data.frame(parameter = gmum.r.svm.poly.params,
                                           class = gmum.r.svm.poly.params.classes,
                                           label = gmum.r.svm.poly.params)

