#' CEC
#' 
#' @title CEC
#' 
#' @description Create CEC model object
#'
#' @export
#'
#' @rdname cec-methods
#'
#' @docType methods
#'
#' @param x Numeric matrix of data.
#' @param k Initial number of clusters.
#' @param method.type Type of clustering (Gauss family).
#' \enumerate{
#' \item diagonal Gaussians with diagonal covariance. The clustering will try to divide the data into ellipsoid with radiuses parallel to coordinate axes
#' \item fsphere Spherical (radial) Gaussian densities (additional parameter - radius)
#' \item full The clustering will have the tendency to divide the data into clusters resembling the unit circles in the Mahalanobis distance (additional parameter - covaraince matrix required)
#' \item func Own function dependent on m and sigma (additional parameter)
#' \item mix Mix of others Gaussian types.
#' \item standard We divide dataset into ellipsoid-like clusters without any preferences (default)
#' \item sphere The clustering will try to divide the data into circles of arbitrary sizes}
#' @param method.init Method to initialize clusters.
#' \enumerate{
#' \item centroids
#' \item kmeans++
#' \item random}
#' @param params.r Radius for spherical family.
#' @param params.cov Covariance matrix for covariance family.
#' @param params.centroids List of centroids.
#' @param control.nstart How many times to perform algorithm.
#' @param control.eps What change of value should terminate algorithm.
#' @param control.itmax Maximum number of iterations at each start.
#' @param log.energy Records collected energy of all clusters in each iteration.
#' @param log.ncluster Records number of clusters in each iteration.
#' @param log.iters Records number of iterations.
#' 
#' @usage CEC(k=3, x=dataset)
#' @usage CEC(k=3, x=dataset, control.nstart=10, method.type='sphere', control.eps=0.05)
#' @usage CEC(k=2, x=dataset, method.type='sphere', method.init='centroids', params.centroids=list(c(-0.5,0.5),c(0,0)))
#' @usage CEC(k=5, x=dataset, method.type='fsphere', params.r=0.01, control.nstart=10, control.eps=0.07)
#' @usage CEC(k=5, x=dataset, method.type='full', params.cov=matrix(c(0.03,0,0,0.01),2), control.nstart=10, control.eps=0.06)
#' @usage CEC(k=1, x=dataset_points, method.type='func', params.function='name_of_my_own_function')
#' 
CEC <- NULL

#' @title runAll
#' 
#' @description Starts whole algorithm again with same parameters
#'
#' @docType methods
#'
#' @param c CEC model object.
#' 
runAll.cec <- NULL

#' @title runOneIteration
#' 
#' @description runs one iteration of algorithm
#'
#' @docType methods
#'
#' @param c CEC model object.
#' 
runOneIteration.cec <- NULL

#' @title energy 
#' 
#' @description Print result energy of clustering found
#' 
#' @docType methods
#'
#' @param c CEC model object.
#' 
energy.cec <- NULL

#' @title y
#' 
#' @description Print labels assigned
#' 
#' @docType methods
#'
#' @param c CEC model object.
#' 
y.cec <- NULL

#' @title x
#' 
#' @description Print input dataset 
#' 
#' @docType methods
#'
#' @param c CEC model object.
#' 
x.cec <- NULL

#' @title centers
#' 
#' @description Print centers of clusters
#'
#' @docType methods
#' 
#' @param c CEC model object.
#' 
centers.cec <- NULL

#' @title covMatrix
#' 
#' @description Print covariances of clusters
#'
#' @docType methods
#'
#' @param c CEC model object.
#' 
covMatrix.cec <- NULL

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
#' 
predict.cec <- NULL

#' @title log.ncluster.cec
#' 
#' @description Print number of clusters that has been recorded at each stage of learning.
#' Data is recorded only if you have chosen to when you created CEC model object.
#' 
#' @docType methods
#'
#' @param c CEC model object.
#' 
log.ncluster.cec <- NULL

#' @title log.energy.cec
#' 
#' @description Print energy that has been recorded at each stage of learning.
#' Data is recorded only if you have chosen to when you created CEC model object.
#'  
#' @docType methods
#'
#' @param c CEC model object.
#' 
log.energy.cec <- NULL

#' @title log.iters.cec
#' 
#' @description Print how many iterations it took to learn CEC model
#'  
#' @docType methods
#'
#' @param c CEC model object.
#' 
log.iters.cec <- NULL

#' @title nstart
#' 
#' @description Print number of times CEC algorithm is to be performed. The best outcome
#' is chosen as result.
#'  
#' @docType methods
#'
#' @param c CEC model object.
#' 
nstart.cec <- NULL

loadModule('cec', TRUE)

evalqOnLoad({
  CEC <<- function(x = NULL,
                   k = 0,
                   method.type = "standard",
                   method.init = "kmeans++",
                   params.r = 0,
                   params.cov = matrix(0),
                   params.centroids = NULL,
                   params.mix = NULL,
                   params.function = "",
                   control.nstart = 1,
                   control.eps = 1e-4,
                   control.itmax = 25,
                   log.energy = FALSE,
                   log.ncluster= FALSE){
    
    # check for errors
    call <- match.call(expand.dots = TRUE)
    
    if (is.null(x))
      stop("Dataset is required!");
    
    if(is(x, "data.frame")){
      x = data.matrix(x);
    }
    
    if (k <= 0)
      stop("Number of clusters should be a positive integer!");
    
    if (control.nstart <= 0)
      stop("Number of starts should be a positive integer!");
    
    if (control.eps > 1.0 / k)
      stop("killThreshold " + contorl.eps + " is too high!");  
    
    if (control.itmax <= 0)
      stop("Maximum number of iterations should be a positive integer!");
    
    if(is(params.cov, "data.frame")){
     params.cov = data.matrix(params.cov);
    }
    
    config <- new(CecConfiguration)
    config$setDataSet(x)
    config$setEps(control.eps)      
    config$setMix(params.mix) 
    config$setNrOfClusters(k)
    config$setLogEnergy(log.energy)
    config$setLogCluster(log.ncluster)      
    config$setNstart(control.nstart)
    config$setCentroids(params.centroids)
    config$setMethodInit(method.init)              
    config$setMethodType(method.type)
    config$setCov(params.cov)
    config$setR(params.r)
    config$setFunction(params.function)
    config$setItmax(control.itmax)
    
    model <- new(CecModel, config)

    assign("call", call, model)
    model
  }
  
  runAll.cec <<- function(c) {
    c$runAll()
  }
  
  runOneIteration.cec <<- function(c) {
    c$runOneIteration()
  }

  energy.cec <<- function(c) {
    c$energy()
  }
  
  y.cec <<- function(c) {
    c$y()
  }
  
  x.cec <<- function(c) {
    c$x()
  }

  centers.cec <<- function(c) {
    c$centers()
  }
  
  covMatrix.cec <<- function(c) {
    c$covMatrix()
  }

  log.ncluster <<- function(c) {
    c$log.ncluster()
  }

  log.energy.cec <<- function(c) {
    c$log.energy()
  }

  log.iters.cec <<- function(c) {
    c$log.iters()
  }
    
    setGeneric("runAll", function(c) standardGeneric("runAll"))
    setGeneric("runOneIteration", function(c) standardGeneric("runOneIteration"))
    setGeneric("energy", function(c) standardGeneric("energy"))
    setGeneric("y", function(c) standardGeneric("y"))
    setGeneric("x", function(c) standardGeneric("x"))
    setGeneric("centers", function(c) standardGeneric("centers"))
    setGeneric("covMatrix", function(c) standardGeneric("covMatrix"))
    setGeneric("log.ncluster", function(c) standardGeneric("log.ncluster"))
    setGeneric("log.energy", function(c) standardGeneric("log.energy"))
    setGeneric("log.iters", function(c) standardGeneric("log.iters"))
    
    setMethod("runAll", "Rcpp_CecModel", runAll.cec)
    setMethod("runOneIteration", "Rcpp_CecModel", runOneIteration.cec)
    setMethod("energy", "Rcpp_CecModel", energy.cec)
    setMethod("y", "Rcpp_CecModel", y.cec)
    setMethod("x", "Rcpp_CecModel", x.cec)
    setMethod("centers", "Rcpp_CecModel", centers.cec)
    setMethod("covMatrix", "Rcpp_CecModel", covMatrix.cec)
    setMethod("log.ncluster", "Rcpp_CecModel", log.ncluster.cec)
    setMethod("log.energy", "Rcpp_CecModel", log.energy.cec)
    setMethod("log.iters", "Rcpp_CecModel", log.iters.cec)
  
    setMethod("predict", "Rcpp_CecModel", function(object, x) {
      if ( !is(x, "data.frame") && !is(x, "matrix") && !is(x,"numeric")  ) {
        stop("Wrong target class, please provide data.frame, matrix or numeric vector")
      }
      
      if (!is(x, "matrix")) {
        x = data.matrix(x)
      }
      object$predict(x)
    })
})
