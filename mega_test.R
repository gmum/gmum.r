library(e1071)
library(Sparse)

# Read binary datasets
datasets = lapply(list.files(path="data_local"), function(x){ ds=read.matrix.csr(paste("data_local",x,sep="/")); ds$name=x; ds})





library(gmum.r)
library(caret)

svm.params.defaults = list(
               lib         = "libsvm",             
               kernel      = "linear",
               prep        = "none",
               mclass      = "none",
               C           = 1,
               gamma       = 1,
               coef0       = 0,
               degree      = 3,
               shrinking   = TRUE,
               probability = FALSE,
               cweights    = FALSE,
               example_weights    = FALSE,
               cache_size  = 200,
               tol         = 1e-3,
               verbosity   = 2)

svm.params=c("lib", "kernel", "prep", "mclass", 
             "C", "gamma", "coef0", "degree", 
             "shrinking", "probability", "cweights", 
             "example_weights", "cache_size", "tol", 
             "verbosity")

svm.params.classes=c("character", "character", "character", "character",
        "double", "double", "double", "double", 
        "logical", "logical", 
        "double", "double", "double", "double", "double")


gmumrSVM$parameters

rf = getModelInfo("rf")
rf$rf$predict
gmumrSVM$predict



unlist(lapply(svm.params, function(x) paste(x,"param",sep="_")))

gmumrSVM <- list(label = "gmum.r.svm",
                  library = c("gmum.r"),
                  type = "Classification",
                  parameters = data.frame(parameter = svm.params,
                                          class = svm.params.classes,
                                          label = unlist(lapply(svm.params, function(x) paste(x,"param",sep="_")))),
                  grid = function(x, y, len = NULL) {
                    # We pass tuning grid manually. 
                    expand.grid(svm.params.defaults)
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
                        lib         = as.character(param$lib),
                        kernel      = as.character(param$kernel),
                        prep        = as.character(param$prep),
                        mclass      = as.character(param$mclass),
                        C           = param$C,
                        gamma       = param$gamma,
                        coef0       = param$coef0,
                        degree      = param$degree,
                        shrinking   = as.logical(param$shrinking),
                        probability = as.logical(param$probability),
                        cweights    = param$cweights,
                        example_weights    = param$example_weights,
                        cache_size  = param$cache_size,
                        tol         = param$tol,
                        verbosity   = param$verbosity,
                        ...
                      ))
                  },
                  predict = function(modelFit, newdata, submodels = NULL) {  
                      as.factor(predict(modelFit, newdata))
                  },
                  prob = NULL,
                  varImp = NULL,
                  levels = function(x) levels(x$getY()),
                  sort = function(x) x[order(x[,1]),])

library(mlbench)
data(Sonar)

Sonar$Class = as.factor(as.numeric(Sonar$Class))
Vehicle$Class = as.factor(as.numeric(Vehicle$Class))
library(caret)
set.seed(998)
inTraining <- createDataPartition(Vehicle$Class, p = .75, list = FALSE)
training <- Vehicle[ inTraining,]
testing  <- Vehicle[-inTraining,]

fitControl <- trainControl(method = "cv",
                           ## 10-fold CV...
                           number = 2,
                           ## repeated ten times
                           repeats = 1,
                           verboseIter=TRUE
                           )

C <- 10^(-7:11)
Gamma <- 10^(-10:10)
kernel = 'rbf'

set.seed(825)

copy <- function(x) x
temp = copy(svm.params.defaults)
temp$C = C
temp$gamma = Gamma
temp$kernel = 'rbf'
expand.grid(temp)

model <- train(Class ~ ., data = training,
                   method = gmumrSVM,
                   preProc = c("center", "scale"),
                   tuneLength = 8,
                   tuneGrid = expand.grid(temp),               
                   trControl = fitControl)


max(model$results$Accuracy)


model2 <- train(Class ~ ., data = training,
               method = "svmRadial",
               preProc = c("center", "scale"),
               tuneLength = 8,
               tuneGrid = expand.grid(C=C, sigma=Gamma),               
               trControl = fitControl, scale=FALSE)
max(model2$results$Accuracy)

library(e1071)
x <- svm(x, y)
x$fitted

tuneNames <- as.character(gmumrSVM$parameters$parameter)
goodNames <- all.equal(sort(tuneNames), sort(names(gmumrSVM$grid(NULL, NULL))))


library(gmum.r)
names(ds)

ds <- svm.dataset.breast_cancer()
formula <- X1~ .
svm <- SVM(formula, ds, lib="libsvm", kernel="linear", C=10, example_weights=FALSE, cweights=FALSE)
predict(svm, x)

x <- svm$getX()
y <- svm$getY()
svm <- SVM(NULL, as.data.frame(x), y, lib="libsvm", kernel="linear", C=10)
plot(svm)

x.df <- as.data.frame(x)
x.df$y = y
as.matrix(x.df)
ncol(training)
x = training[,1:60]
y = training[,61]

gmumrSVM$fit(x,y,NULL,param, NULL,NULL,NULL)

param = svm.params.defaults
gmum.r::SVM(
  formula = formula("y ~ ."),
  as.matrix(x.df),
  lib         = as.character(param$lib),
  kernel      = as.character(param$kernel),
  prep        = as.character(param$prep),
  mclass      = as.character(param$mclass),
  C           = param$C,
  gamma       = param$gamma,
  coef0       = param$coef0,
  degree      = param$degree,
  shrinking   = as.logical(param$shrinking),
  probability = as.logical(param$probability),
  cweights    = param$cweights,
  example_weights    = param$example_weights,
  cache_size  = param$cache_size,
  tol         = param$tol,
  verbosity   = param$verbosity)


library(mlbench)
data(Sonar)

library(caret)
set.seed(998)
inTraining <- createDataPartition(Sonar$Class, p = .75, list = FALSE)
training <- Sonar[ inTraining,]
testing  <- Sonar[-inTraining,]
x = training[,1:60]
y = training[,61] + 1
s = gmumrSVM$fit(x,y,NULL,svm.params.defaults, NULL,NULL,NULL)
as.factor(predict(s, x))

names(training)
gbm(Class ~ ., data=training)
k = svm(Class ~ ., training)
library(e1071)
predict(k, training[,1:60])


SVM


names(training)
library(mlbench)
data(Sonar)
library(caret)
library(gmum.r)
set.seed(998)
inTraining <- createDataPartition(Sonar$Class, p = .75, list = FALSE)
training <- Sonar[ inTraining,]
l <- SVM(formula=Class ~ ., x=as.data.frame(training), cweights=FALSE, example_weights=FALSE)
predict(l, training[, 1:60]) 

training
