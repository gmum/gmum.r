library("caret")

gmum.r.svm.radial=c("C", "gamma")
gmum.r.svm.radial.classes=c("double", "double")
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
                   return(gmum.r::SVM(
                     formula = formula("y ~ ."),
                     x.df,
                     C = param$C
                     gamma = gamma$C,
                     ...
                   ))
                 },
                 predict = function(modelFit, newdata, submodels = NULL) {  
                   as.factor(predict(modelFit, newdata))
                 },
                 predict = function(modelFit, newdata, submodels = NULL) {  
                   predict(modelFit, newdata)
                 },
                 varImp = NULL,
                 levels = function(x) levels(x$getY()),
                 sort = function(x) x[order(x[,1]),])