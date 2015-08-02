#' CEC
#' 
#' @name Wrapper function CEC
#' 
#' @rdname cec-classes 
#'
#' @title Cross-Entropy Clustering
#' 
#' @description Create CEC model object
#'
#' @export
#'
#' @docType methods
#'
#' @param x Numeric matrix of data.
#' @param k Initial number of clusters.
#' @param method.type Type of clustering (Gauss family).
#' \enumerate{
#' \item \code{"diagonal"} Gaussians with diagonal covariance. The clustering will try to divide the data into ellipsoid with radiuses parallel to coordinate axes
#' \item \code{"fixed_spherical"} Spherical (radial) Gaussian densities (additional parameter - radius)
#' \item \code{"fixed_covariance"} The clustering will have the tendency to divide the data into clusters resembling the unit circles in the Mahalanobis distance (additional parameter - covaraince matrix required)
#' \item \code{"func"} Own function dependent on m and sigma (additional parameter)
#' \item \code{"standard"} We divide dataset into ellipsoid-like clusters without any preferences (default)
#' \item \code{"spherical"} The clustering will try to divide the data into circles of arbitrary sizes}
#' @param method.init Method to initialize clusters.
#' \enumerate{
#' \item \code{"centroids"}
#' \item \code{"kmeans++"}
#' \item \code{"random"}}
#' @param params.r Radius for spherical family.
#' @param params.cov Covariance matrix for covariance family.
#' @param params.centroids List of centroids.
#' @param params.mix List of cluster with mixed Gaussian types.
#'  
#' @param params.function User energy function
#'
#' @param control.nstart How many times to perform algorithm.
#' @param control.eps What change of value should terminate algorithm.
#' @param control.itmax Maximum number of iterations at each start.
#' @param log.energy Records collected energy of all clusters in each iteration.
#' @param log.ncluster Records number of clusters in each iteration.
#' @param seed User seed
#' 
#' @examples
#' \dontrun{
#' CEC(k=3, x=dataset)
#' CEC(k=3, x=dataset, control.nstart=10, method.type='spherical', control.eps=0.05)
#' CEC(k=2, x=dataset, method.type='spherical', method.init='centroids', params.centroids=list(c(-0.5,0.5),c(0,0)))
#' CEC(k=5, x=dataset, method.type='fixed_spherical', params.r=0.01, control.nstart=10, control.eps=0.07)
#' CEC(k=5, x=dataset, method.type='fixed_covariance', params.cov=matrix(c(0.03,0,0,0.01),2), control.nstart=10, control.eps=0.06)
#' CEC(k=1, x=dataset, method.type='func', params.function='name_of_my_own_function')
#' fixed_spherical_cluster_param = list(method.type = 'fixed_spherical', params.r = 0.001)
#' covariance_cluster_param = list(method.type = 'fixed_covariance', params.cov=matrix(c(0.05, 0, 0, 0.001), 2))
#' CEC(x = dataset, params.mix = list(covariance_cluster_param, fixed_spherical_cluster_param, fixed_spherical_cluster_param, fixed_spherical_cluster_param, fixed_spherical_cluster_param), control.nstart = 10)
#' p1 = list(method.type='spherical', k=3)
#' p2 = list(method.type='diagonal', k=2)
#' CEC(x=dataset, params.mix=list(p1, p2))
#' }
CEC <- NULL

#' @title runAll
#' 
#' @rdname cec-methods 
#' 
#' @description Starts whole algorithm again with same parameters
#'
#' @docType methods
#'
#' @aliases runAll,Rcpp_CecModel-method
#' 
runAll.cec <- NULL

#' @rdname cec-methods
#' 
#' @aliases runOneIteration,Rcpp_CecModel-method
#' 
#' @title runOneIteration
#' 
#' @description runs one iteration of algorithm
#'
#' @docType methods
#'
runOneIteration.cec <- NULL

#' @rdname cec-methods
#' 
#' @aliases energy,Rcpp_CecModel-method
#'
#' @title energy 
#' 
#' @description Print result energy of clustering found
#' 
#' @docType methods
#'
energy.cec <- NULL

#' @rdname cec-methods
#' 
#' @aliases clustering,Rcpp_CecModel-method
#'
#' @title clustering
#' 
#' @description Print labels assigned
#' 
#' @docType methods
#'
clustering.cec <- NULL

#' @rdname cec-methods
#' 
#' @aliases x,Rcpp_CecModel-method
#' 
#' @description Print input dataset 
#' 
#' @docType methods
#'
x.cec <- NULL

#' @rdname cec-methods
#' 
#' @aliases centers,Rcpp_CecModel-method
#'
#' @title centers
#' 
#' @description Print centers of clusters
#'
#' @docType methods
#' 
centers.cec <- NULL

#' @rdname cec-methods
#' 
#' @aliases covMatrix,Rcpp_CecModel-method
#'
#' @title energy 
#' 
#' @title covMatrix
#' 
#' @description Print covariances of clusters
#'
#' @docType methods
#'
covMatrix.cec <- NULL

#' @rdname cec-methods
#' 
#' @aliases predict,Rcpp_CecModel-method
#'
#' @title predict
#' 
#' @description Classify a new point according to the model (returns index of cluster where given point belong to)
#' 
#' @rdname cec-predict-methods
#' 
#' @export
#' 
#' @docType methods
#' 
#' @param object Trained CEC model object.
#' @param x Given point.
#' @param ... other arguments not used by this method.
#' 
predict.cec <- NULL


#' @rdname cec-methods
#' 
#' @aliases logClusters,Rcpp_CecModel-method
#'
#' @title logClusters
#' 
#' @description Print number of clusters that has been recorded at each stage of learning.
#' Data is recorded only if you have chosen to when you created CEC model object.
#' 
#' @docType methods
#'
#' @param c CEC model object.
#' 
logClusters <- NULL

#' @rdname cec-methods
#' 
#' @aliases logEnergy,Rcpp_CecModel-method
#'
#' @title logEnergy
#' 
#' @description Print energy that has been recorded at each stage of learning.
#' Data is recorded only if you have chosen to when you created CEC model object.
#'  
#' @docType methods
#'
#' @param c CEC model object.
#' 
logEnergy <- NULL

#' @rdname cec-methods
#' 
#' @aliases logIterations,Rcpp_CecModel-method
#'
#' @title logIterations
#' 
#' @description Print how many iterations it took to learn CEC model
#'  
#' @docType methods
#'
#' @param c CEC model object.
#' 
logIterations <- NULL

loadModule('cec', TRUE)

CEC <- function(x = NULL,
                 k = 0,
                 method.type = "standard",
                 method.init = "kmeans++",
                 params.r = 0,
                 params.cov = matrix(0),
                 params.centroids = NULL,
                 params.mix = NULL,
                 params.function = NULL,
                 control.nstart = 10,
                 control.eps = 0.05,
                 control.itmax = 25,
                 log.energy = TRUE,
                   log.ncluster= TRUE,
                   seed = NULL){
  
  # check for errors
  call <- match.call(expand.dots = TRUE)
  
  if (is.null(x))
    stop("Dataset is required!");
  
  if(is(x, "data.frame")){
    x = data.matrix(x);
  }
  
  if (is.null(params.mix) && k <= 0)
    stop("Number of clusters should be a positive integer!");
  
  if (control.nstart <= 0)
    stop("Number of starts should be a positive integer!");
  
  npoints <- dim(x)[1]
  if ( (control.eps < 0) || (control.eps > ((npoints - 1) / npoints)) )
    stop("control.eps = ", control.eps, " should be in range [0, (N-1)/N]!");  
  
  if (control.itmax < 0)
    stop("Maximum number of iterations should be a natural number!");
  
  if(is(params.cov, "data.frame")){
    params.cov = data.matrix(params.cov);
  }
  
  config <- new(CecConfiguration)

    if(is.null(seed) == FALSE) {
        config$setSeed(seed)
    }
  config$setDataSet(x)
  config$setEps(control.eps)      
  config$setNrOfClusters(k)
  
  if(is.null(params.mix) == FALSE) {
    config$setMix(params.mix) 
  }
  
  if(is.null(params.function) == FALSE) {
    config$setFunction(params.function)
  }

  config$setLogEnergy(log.energy)
  config$setLogCluster(log.ncluster)      
  config$setNstart(control.nstart)
  config$setCentroids(params.centroids)
  config$setMethodType(method.type)             
  config$setCov(params.cov)
  config$setR(params.r)
  config$setMethodInit(method.init) 
  config$setItmax(control.itmax)
  config$setAlgorithm('hartigan')
  
  model <- new(CecModel, config)
  
  assign("call", call, model)
  model
}

runOneIteration.cec <- function(c) {
  c$runOneIteration()
}

energy.cec <- function(c) {
  c$energy()
}

clustering.cec <- function(c) {
  c$clustering()
}

x.cec <- function(c) {
  c$x()
}

centers.cec <- function(c) {
  c$centers()
}

covMatrix.cec <- function(c) {
  c$covMatrix()
}

logClusters <- function(c) {
  c$log.ncluster()
}

logEnergy <- function(c) {
  c$log.energy()
}

logIterations <- function(c) {
  c$log.iters()
}

predict.cec <- function(object, x, ...) {
  if ( !is(x, "data.frame") && !is(x, "matrix") && !is(x,"numeric")  ) {
    stop("Wrong target class, please provide data.frame, matrix or numeric vector")
  }
  
  if(is(x, "vector")){
    x = matrix(x, nrow=1, byrow=TRUE)
  }
  else if (!is(x, "matrix")) {
    x = data.matrix(x)
  }
  
  if(dim(object$x())[2] != dim(x)[2]){
    stop("Incompatible dimension!")
  }
  
  apply(x, 1, function(row) {
    object$predict(row)
  })
}

#' Class Rcpp_CecModel.
#'
#' Class \code{Rcpp_CecModel} defines a CEC model class. 
#'
#' @name Rcpp_CecModel-class
#' @rdname cec-classes 
#' @exportClass Rcpp_CecModel
setClass(Class = "Rcpp_CecModel")

#' Class Rcpp_CecConfiguration.
#'
#' Class \code{Rcpp_CecConfiguration} defines a CEC model configuration class. 
#'
#' @name Rcpp_CecConfiguration-class
#' @rdname cec-classes 
#' @exportClass Rcpp_CecConfiguration
setClass(Class = "Rcpp_CecConfiguration")

setGeneric("runAll", function(c) standardGeneric("runAll"))
setGeneric("runOneIteration", function(c) standardGeneric("runOneIteration"))
setGeneric("energy", function(c) standardGeneric("energy"))
setGeneric("clustering", function(c) standardGeneric("clustering"))
setGeneric("x", function(c) standardGeneric("x"))
setGeneric("centers", function(c) standardGeneric("centers"))
setGeneric("covMatrix", function(c) standardGeneric("covMatrix"))

evalqOnLoad({
    setMethod("runAll", "Rcpp_CecModel", runAll.cec) 
    setMethod("runOneIteration", "Rcpp_CecModel", runOneIteration.cec)
    setMethod("energy", "Rcpp_CecModel", energy.cec)
    setMethod("clustering", "Rcpp_CecModel", clustering.cec)
    setMethod("x", "Rcpp_CecModel", x.cec)
    setMethod("centers", "Rcpp_CecModel", centers.cec)
    setMethod("covMatrix", "Rcpp_CecModel", covMatrix.cec)
    setMethod("predict", "Rcpp_CecModel", predict.cec)
})
