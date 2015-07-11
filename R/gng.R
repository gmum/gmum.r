library(igraph)
library(methods)


gng.plot.color.label <- 'label'
gng.plot.color.fast.cluster <- 'fast.cluster'
gng.plot.color.cluster <- 'cluster'
gng.plot.color.none <- 'none'


gng.plot.layout.v2d <- function(g){
  cbind(V(g)$v0, V(g)$v1)
}
gng.plot.layout.igraph.fruchterman <- function(g){
  layout.fruchterman.reingold(g, niter=10000, area=4*vcount(g)^2)
}
gng.plot.layout.igraph.fruchterman.fast <- layout.fruchterman.reingold
gng.plot.layout.igraph.auto <- layout.auto

gng.plot.2d <- 1
gng.plot.3d <- 2
gng.plot.2d.errors <- 3



gng.type.default <- function(){
	c(2)
}

gng.type.optimized <- function(minimum=0, maximum=10){
  c(0, minimum, maximum)
}

gng.type.utility<- function(k=1.3){
  c(1, k)
}

.gng.dataset.bagging.prob <- 3
.gng.dataset.bagging <- 2
.gng.dataset.sequential <-1

.GNG <- NULL



#' Plot GNG
#'
#' @title plot
#' 
#' @description Plot resulting graph using igraph plotting, or using rgl 3d engine.
#' 
#' @usage
#' plot(gng)
#' 
#' @export
#' 
#' @rdname plot-methods
#' 
#' @docType methods
#'
#' @param mode \code{gng.plot.3d} (3d plot), \code{gng.plot.2d} (igraph plot)
#' \code{gng.plot.2d.errors} (igraph plot with mean error log plot)
#' 
#' @param layout Layout to be used when plotting. Possible values: \code{gng.plot.layour.igraph.v2d} (first two dimensions),
#' \code{gng.plot.layout.igraph.auto} (auto layout from igraph)  \code{gng.plot.layout.igraph.fruchterman.fast} (fast fruchterman reingold layout),or any function accepting igraph graph and returning layout
#' 
#' @param vertex.color How to color vertexes. Possible values: \code{gng.plot.color.cluster} (vertex color is set to fastgreedy.community clustering),
#' \code{gng.plot.color.label} (rounds to integer label if present), \code{list of integers} (colors vertices according to provided list), \code{gng.plot.color.none} (every node is white),
#' 
#' @param vertex.size Size of plotted vertices
#' 
#' @note If you want to "power-use" plotting and plot for instance a subgraph, you might be interested in
#' exporting igraph with convertToGraph function 
#' 
#' @examples
#' 
#' data(wine, package="rattle")
#' scaled.wine <- scale(wine[-1])
#' gng <- GNG(scaled.wine)
#' # Plots igraph using first 2 coordinates and colors according to clusters
#' plot(gng, mode=gng.plot.2d.errors, layout=gng.plot.layout.v2d, vertex.color=gng.plot.color.cluster)
#' 
#' # Plot rgl (make sure you have installed rgl library)
#' plot(gng, mode=gng.plot.3d, layout=gng.plot.layout.v2d, vertex.color=gng.plot.color.cluster)
#' 
#' # For more possibilities see gng.plot.* constants
#' 
plot.gng <- NULL

#' Save model to binary format
#'
#' @title gngSave
#' 
#' @description Writes model to a disk space efficient binary format. 
#' 
#' @usage
#' gngSave(gng, file.name)
#' 
#' @export
#' 
#' @param filename File where binary will be saved
#' 
#' @rdname gngSave-methods
#' 
#' @docType methods
#'
#' @aliases gngSave
#'
gngSave <- NULL



#' Load model from binary format
#'
#' @title gngLoad
#' 
#' @description Writes model to a disk space efficient binary format. 
#' 
#' @usage
#' gngLoad(file.name)
#' 
#' @export
#' 
#' @param filename Binary file location
#' 
#' @rdname gngLoad-methods
#' 
#' @docType methods
#'
#' @aliases gngLoad
#'
gngLoad <- NULL

#' Get centroids
#'
#' @title predictCentroids
#' 
#' @description Using infomap.communities finds communities and for each community pick node with biggest betweenness score
#' 
#' @usage
#' predictCentroids(gng)
#' 
#' @export
#' 
#' @rdname predictCentroids-methods
#' 
#' @docType methods
#'
#' @examples
#' data(wine, package="rattle")
#' scaled.wine <- scale(wine[-1])
#' gng <- GNG(scaled.wine)
#' # Print position of the first centroid
#' print(node(gng, predictCentroids(gng)[1])$pos)
#' 
#' @aliases predictCentroids
#'  
predictCentroids <- NULL

#' Find closest centroid
#'
#' @title findClosest
#' 
#' @description Finds closest node from given list to vector
#' 
#' @usage
#' findClosest(gng, predictCentroids(gng), c(1,1,1))
#' 
#' @export
#' 
#' @rdname findClosest-methods
#' 
#' @docType methods
#'
#' @param node.ids List of indexes of nodes in gng. 
#' 
#' @param x Can be either \code{vector} or \code{data.frame.}
#' 
#' @examples
#' data(wine, package="rattle")
#' scaled.wine <- scale(wine[-1])
#' gng <- GNG(scaled.wine)
#' # Find closest centroid to c(1,1,1)
#' found.centroids <- predictCentroids(gng)
#' # FIXME
#' #findClosest(gng, found.centroids, c(1,1,1))
#' 
#' @aliases predictCentroid
#' 
findClosest <- NULL


#' Find closest component
#'
#' @title predictComponent
#' 
#' @description Finds connected component closest to given vector(s).
#' 
#' @usage
#' predictComponent(gng, c(1,1,1))
#' 
#' @export
#' 
#' @rdname predictComponent-methods
#' 
#' @docType methods
#'
#' @param x Can be either \code{vector} or \code{data.frame}.
#' 
#' @examples
#' data(wine, package="rattle")
#' scaled.wine <- scale(wine[-1])
#' gng <- GNG(scaled.wine)
#' # Find closest component to c(1,1,1)
#' # FIXME: Use suitable vector param
#' #predictComponent(gng,  c(1,1,1))
#' 
#' @aliases predictComponent
predictComponent <- NULL

#' Get GNG node
#'
#' @title node
#' 
#' @description Retrieves node from resulting graph
#' 
#' @usage
#' node(gng, 10)
#' 
#' @export
#' 
#' @rdname node-methods
#' 
#' @docType methods
#'
#' @param gng_id Id of the node to retrieve. This is the id returned by functions like predict, or centroids
#' 
#' @examples
#' data(wine, package="rattle")
#' scaled.wine <- scale(wine[-1])
#' gng <- GNG(scaled.wine)
#' print(node(gng, 10)$pos)
#' 
#' @aliases node
#' 
node.gng <- NULL


#' @title run
#' 
#' @description Run algorithm (in parallel)
#' 
#' @usage
#' run(gng)
#' 
#' @export
#' 
#' @rdname run-methods
#' 
#' @docType methods
#'
#' @examples
#' data(wine, package="rattle")
#' scaled.wine <- scale(wine[-1])
#' gng <- GNG(scaled.wine)
#' # TODO: Better example
#' run(gng)
#' 
#' @aliases run
#'
run.gng <- NULL

#' @title pause
#' 
#' @description Pause algorithm
#' 
#' @usage
#' pause(gng)
#' 
#' @export
#' 
#' @rdname pause-methods
#' 
#' @docType methods
#'
#' @examples
#' data(wine, package="rattle")
#' scaled.wine <- scale(wine[-1])
#' gng <- GNG(scaled.wine)
#' # TODO: Better example
#' pause(gng)
#' 
#' @aliases pause
#'
pause.gng <- NULL

#' @title pause
#' 
#' @description Terminate algorithm
#' 
#' @usage
#' terminate(gng)
#' 
#' @export
#' 
#' @rdname terminate-methods
#' 
#' @docType methods
#'
#' @examples
#' data(wine, package="rattle")
#' scaled.wine <- scale(wine[-1])
#' gng <- GNG(scaled.wine)
#' # TODO: Better example
#' terminate(gng)
#' 
#' @aliases terminate
#'
terminate.gng <- NULL

#' @title meanError
#' 
#' @description Gets mean error of the graph (note: blocks the execution, O(n))
#' 
#' @usage
#' meanError(gng)
#' 
#' @export
#' 
#' @rdname meanError-methods
#' 
#' @docType methods
#'
#' @examples
#' data(wine, package="rattle")
#' scaled.wine <- scale(wine[-1])
#' gng <- GNG(scaled.wine)
#' # TODO: Better example
#' meanError(gng)
#' 
#' @aliases meanError
#'
meanError.gng <- NULL


#' @title errorStatistics
#' 
#' @description Gets vector with errors for every second of execution
#' 
#' @usage
#' errorStatistics(gng)
#' 
#' @export
#' 
#' @rdname errorStatistics-methods
#' 
#' @docType methods
#'
#' 
#' @aliases errorStatistics
#'
errorStatistics.gng <- NULL


#' @title Constructor of Optimized GrowingNeuralGas object. 
#' 
#' @export 
#' 
#' @description Construct simplified and optimized GNG object. Can be used to train offline, or online. Data dimensionality shouldn't be too big, if
#' it is consider using dimensionality reduction techniques.
#'
#' @param beta Decrease the error variables of all node 
#' nodes by this fraction (forgetting rate). Default 0.99
#' 
#' @param alpha Decrease the error variables of the nodes neighboring to 
#' the newly inserted node by this fraction. Default 0.5
#' 
#' @param lambda New vertex is added every lambda iterations. Default 200
#' 
#' @param max.nodes Maximum number of nodes 
#' (after reaching this size it will continue running, but new noes won't be added)
#' 
#' @param eps.n Strength of adaptation of neighbour node. Default \code{0.0006}
#' 
#' @param eps.w Strength of adaptation of winning node. Default \code{0.05}
#' 
#' @param max.iter If training offline will stop if exceedes max.iter iterations. Default \code{200}
#'
#' @param train.online If used will run in online fashion. Default \code{FALSE}
#'
#' @param min.improvement Used for offline (default) training. 
#' Controls stopping criterion, decrease if training stops too early. Default \code{1e-3}
#'
#' @param dim Used for training online, specifies dataset example dimensionality
#'
#' @param value.range All example features should be in this range, required for optimized version of the algorithm. Default \code{(0,1)} 
#' 
#' @examples
#' 
#' # Train online optimizedGNG. All values in this dataset are in the range (-4.3, 4.3)
#' data(wine, package="rattle")
#' # FIXME
#' #gng <- OptimizedGNG(train.online = TRUE), value.range=c(min(scale(wine[-1]),max(scale(wine[-1]))), max.nodes=20)
#' #insertExamples(gng, scale(wine[-1]))
#' #run(gng)
#' #Sys.sleep(10)
#' #pause(gng)
OptimizedGNG <- NULL


#' @title clustering
#' 
#' @description Gets vector with node indexes assigned to examples in the dataset
#' 
#' @usage
#' clustering(gng)
#' 
#' @export
#' 
#' @rdname clustering-methods
#' 
#' @docType methods
#'
#' @aliases clustering
#'
clustering.gng <- NULL

#' @title errorStatistics
#' 
#' @description Gets vector with errors for every second of execution
#' 
#' @usage
#' errorStatistics(gng)
#' 
#' @export
#' 
#' @rdname errorStatistics-methods
#' 
#' @docType methods
#'
#' @examples
#' data(wine, package="rattle")
#' scaled.wine <- scale(wine[-1])
#' gng <- GNG(scaled.wine)
#' # TODO: Better example
#' errorStatistics(gng)
#' 
#' @aliases errorStatistics
#'
errorStatistics.gng <- NULL

#' @title Constructor of GrowingNeuralGas object. 
#' 
#' @export 
#' 
#' @description Construct GNG object. Can be used to train offline, or online.
#' 
#' @param beta Decrease the error variables of all node 
#' nodes by this fraction (forgetting rate). Default 0.99
#' 
#' @param alpha Decrease the error variables of the nodes neighboring to 
#' the newly inserted node by this fraction. Default 0.5
#' 
#' @param lambda Every lambda iteration is added new vertex. Default 200
#' 
#' @param max.nodes Maximum number of nodes 
#' (after reaching this size it will continue running, but won't add new nodes)
#' 
#' @param eps.n How strongly adapt neighbour node. Default \code{0.0006}
#' 
#' @param eps.w How strongly adapt winning node. Default \code{0.05}
#' 
#' @param max.iter Uf training offline will stop if exceedes max.iter iterations. Default \code{200}
#'
#' @param train.online default FALSE. If used will run in online fashion
#'
#' @param min.improvement Used for offline (default) training. 
#' Controls stopping criterion, decrease if training stops too early. Default \code{1e-3}
#'
#' @param dim Used for training online, specifies training example size
#'
#' @param k Utility constant, by default turned off. Good value is 1.3. Constant controlling speed of erasing obsolete nodes, 
#' see \url{http://sund.de/netze/applets/gng/full/tex/DemoGNG/node20.html}
#' 
#'
#' @examples
#' 
#' data(wine, package="rattle")
#' scaled.wine <- scale(wine[-1])
#' # Train in an offline manner
#' gng <- GNG(scaled.wine, labels=wine$Type, max.nodes=20)
#' # Plot
#' plot(gng)
#'
#' # Train in an online manner with utility (erasing obsolete nodes)
#' gng <- GNG(scaled.wine, labels=wine$Type, max.nodes=20, train.online=TRUE, k=1.3)
#' # FIXME
#' #insertExamples(gng, scale(wine[-1])
#' run(gng)
#' Sys.sleep(10)
#' terminate(gng)
#' # Plot
#' plot(gng)
#'
GNG <- NULL

print.gng <- NULL

summary.gng <- NULL

#' @title convertToGraph
#' 
#' @description Converts GNG to igraph object.
#' 
#' @usage
#' convertToGraph(gng)
#' 
#' @param gng Learned GNG object
#' 
#' @export
#' 
#' @rdname convertToGraph-methods
#' 
#' @docType methods
#'
#' @aliases convertToGraph
#'
convertToGraph.gng <- NULL


generateExamples <- NULL

#' @title insertExamples
#' 
#' @description Insert examples with optional labels.
#' 
#' @usage
#' insertExamples(gng, M, L=c())
#' 
#' @export
#' 
#' @param examples \code{matrix} or \code{data.frame} with rows as examples. Note: if training online make sure
#' number of columns matches dim parameter passed to GNG constructor.
#' 
#' @param labels \code{vector} of labels, that will be associated with nodes in the graph. GNG will assign to each
#' node a mean of labels of closest examples.
#' 
#' @rdname insertExamples-methods
#' 
#' @docType methods
#'
#' @examples
#' 
#' data(wine, package="rattle")
#' scaled.wine <- scale(wine[-1])
#' gng <- GNG(scaled.wine)
#' # Add preset examples
#' # FIXME
#' #M = generateExamples(preset=gng.preset.sphere)
#' #insertExamples(gng, M)
#' @aliases insertExamples
#'
insertExamples.Rcpp_GNGServer <- NULL






.GNG <- function(x=NULL, labels=c(),
                  beta=0.99, 
                  alpha=0.5, 
                  max.nodes=100, 
                  eps.n=0.0006, 
                  eps.w= 0.05, 
                  max.edge.age = 200, 
                  type = gng.type.default(),
                  max.iter=200,
                  train.online=FALSE,
                  min.improvement=1e-3,
                  lambda=200,
                  dim=0,
                  verbosity=0,
                  seed=-1
){
  
  
  config <- new(GNGConfiguration)
  
  config$seed = seed
  
  if(is.data.frame(x)){
    warning("Converting data.frame to matrix. Please make sure you pass only numerics to GNG.")
    x <- as.matrix(x)
  }
  
  # Fill in configuration
  if(train.online){
    if(is.null(x)){
      config$dim = dim
    }else{
      config$dim = ncol(x)
    }
    config$max_iter = -1
  }else{
    config$dim = ncol(x)
    config$max_iter = max.iter
  }
  
  if(type[1] == gng.type.optimized()[1]){
    config$.uniformgrid_optimization = TRUE
    config$.lazyheap_optimization = TRUE  
    config$.set_bounding_box(type[2], type[3])
    
    if(!train.online){
      if(!max(x) <= type[3] && !min(x) >= type[2]){
        stop(gmum.error(GMUM_WRONG_PARAMS, "Passed incorrect parameters. The dataset is not in the defined range"))
      }
    }
    
  }else{
    config$.uniformgrid_optimization = FALSE
    config$.lazyheap_optimization = FALSE
  }
  
  if(type[1] == gng.type.utility()[1]){
    config$.experimental_utility_k = type[2]
    config$.experimental_utility_option = 1
  }
  else{
    config$.experimental_utility_option = 0
  }
  
  
  config$.dataset_type=.gng.dataset.bagging
  config$beta = beta
  config$max_edge_age = max.edge.age
  config$alpha = alpha  
  config$max_nodes = max.nodes
  config$eps_n = eps.n
  config$eps_w = eps.w
  
  config$lambda = lambda
  config$verbosity = verbosity
  
  if(!config$.check_correctness()){
    stop(gmum.error(GMUM_WRONG_PARAMS, "Passed incorrect parameters."))
  }
  
  # Construct server
  server = new(GNGServer, config)
  
 
  if(train.online){
    if(!is.null(x)){
      insertExamples(server, x, labels)
      run(server)
    }
  }
  if(!train.online){
    
    print("Training offline")
    if(is.null(x)){
      stop(gmum.error(GMUM_ERROR, "Passed null data and requested training offline"))
    }else{
      insertExamples(server, x, labels)
      run(server)
      
      max_iter = max.iter
      min_relative_dif = min.improvement
      iter = 0
      previous_iter = -1
      best_so_far = 1e10
      initial_patience = 3
      error_index = -1 # always bigger than 0
      patience = initial_patience
      
      tryCatch({
        
        while(server$getCurrentIteration() == 0 || server$isRunning()){}
        
        # max_iter is checked in GNG
        while(iter == 0 || server$isRunning()){
          Sys.sleep(0.1)
          iter = server$getCurrentIteration()
          
          if(previous_iter != iter && iter %% (max_iter/100) == 0){    
            print(paste("Iteration", iter))
          }
          
          
          
          if(length(server$getErrorStatistics()) > 5){
            errors = server$getErrorStatistics()
            
            best_previously = min(errors[(length(errors)-5):length(errors)])
            
            # this is same as (best_so_far-best_previously)/best_so_far < min_relative_di
            # we get minimum of window 5 and look at the history
            if( (error_index - server$.getGNGErrorIndex()) > 4 && 
                  (best_so_far - best_previously) < best_so_far*min_relative_dif){
              patience = patience - 1
              if(patience <= 0){
                print(sprintf("Best error during training: %f", best_so_far))
                print(sprintf("Best error in 5 previous iterations %f", best_previously))
                print(errors[(length(errors)-5):length(errors)])
                print("Patience (which you can control) elapsed, bailing out")
                break
              }
            }else{
              patience = initial_patience
            }
            
            
            error_index = server$.getGNGErrorIndex()
            best_so_far = min(best_previously, best_so_far)
          }
          
        }
        
        print(paste("Iteration", iter))
        previous_iter = iter
        
        
        
        if(server$isRunning()){
          terminate(server)
        }
        
        server$.updateClustering()
        
      }, interrupt=
        function(interrupt){
          if(server$isRunning()){
            terminate(server)
          }
          
        })
      
    }
  }else{
  }
  
  
  
  server
}


GNG <- function(x=NULL, labels=c(),
                 beta=0.99, 
                 alpha=0.5, 
                 max.nodes=1000, 
                 eps.n=0.0006, 
                 eps.w=0.05, 
                 max.edge.age=200,
                 train.online=FALSE,
                 max.iter=200,
                 dim=0,
                 min.improvement=1e-3,
                 lambda=200,
                 verbosity=0,
                 seed=-1,
                 k=NULL
){
  gng <- NULL
  call <- match.call(expand.dots = TRUE)
  if(is.null(k)){
    gng <- .GNG(x=x, labels=labels, beta=beta, alpha=alpha, max.nodes=max.nodes, 
                eps.n=eps.n, eps.w=eps.w, max.edge.age=max.edge.age, type=gng.type.default(), train.online=train.online, max.iter=max.iter, dim=dim, min.improvement=min.improvement, lambda=lambda, verbosity=verbosity, seed=seed)
  }else{
    gng <- .GNG(x=x, labels=labels, beta=beta, alpha=alpha, max.nodes=max.nodes, 
                eps.n=eps.n, eps.w=eps.w, max.edge.age=max.edge.age, type=gng.type.utility(k=k), train.online=train.online, max.iter=max.iter, dim=dim, min.improvement=min.improvement, lambda=lambda, verbosity=verbosity, seed=seed)		
  }
  assign("call", call, gng)
  gng
}

OptimizedGNG <- function(x=NULL, labels=c(),
                          beta=0.99, 
                          alpha=0.5, 
                          max.nodes=1000, 
                          eps.n=0.0006, 
                          eps.w= 0.05, 
                          max.edge.age = 200,
                          train.online=FALSE,
                          max.iter=200,
                          dim=0,
                          min.improvement=1e-3,
                          lambda=200,
                          verbosity=0,
                          seed=-1,
                          value.range=c(0,1)
){
  if(value.range[1] >= value.range[2]){
    stop(gmum.error(GMUM_ERROR, "Incorrect range"))
    return()
  }
  call <- match.call(expand.dots = TRUE)
  gng <- .GNG(x=x, labels=labels, beta=beta, alpha=alpha, max.nodes=max.nodes, 
              eps.n=eps.n, eps.w=eps.w, max.edge.age=max.edge.age, type=gng.type.optimized(min=value.range[1], max=value.range[2]), train.online=train.online, max.iter=max.iter, dim=dim, min.improvement=min.improvement, lambda=lambda, verbosity=verbosity, seed=seed)
  assign("call", call, gng)
  gng
}    

predictComponent <- function(object, x){
  tryCatch(if(is.null(object$components.membership)){
    assign("components.membership", clusters(convertToGraph(object))$membership, object)
  }, error=function(...) 
    assign("components.membership", clusters(convertToGraph(object))$membership, object))
  
  object$components.membership[predict(object, x)]
}

plot.gng <- function(x, vertex.color=gng.plot.color.cluster, 
                      layout=gng.plot.layout.v2d, mode=gng.plot.2d, 
                      vertex.size=3){
  if(vertex.size <= 0){
    stop("Please pass positivie vertex.size")
  }
  
  if(!(is.list(vertex.color) || is.vector(vertex.color) || vertex.color %in% c(gng.plot.color.cluster, 
                           gng.plot.color.fast.cluster, gng.plot.color.label, gng.plot.color.none))){
    stop("Please pass correct vertex.color")
  }
  
  
  if(x$getNumberNodes() > 4000){
    warning("Trying to plot very large graph (>4000 nodes). It might take a while.")
  }
  if(x$getNumberNodes() == 0){
    warning("Empty graph")
    return()
  }
  
  if(mode == gng.plot.3d && !("rgl" %in% rownames(installed.packages()))){
    warning("Please install rgl and reload the package to plot 3d")
    return()
  }
  
  if(mode == gng.plot.3d){
    .gng.plot3d(x)
  }
  else if(mode == gng.plot.2d){
    .gng.plot2d(x, vertex.color, layout, vertex.size=vertex.size)
  }
  else if(mode == gng.plot.2d.errors){
    .gng.plot2d.errors(x, vertex.color, layout, vertex.size=vertex.size)
  }else{
    stop("Unrecognized mode")
  }
}

print.gng <- function(x){
  print(sprintf("Growing Neural Gas, nodes %d with mean error %f", 
                x$getNumberNodes(), x$getMeanError()))
}

summary.gng <- function(object){
  if(object$.getConfiguration()$.uniformgrid_optimization){
    print("(Optimized) Growing Neural Gas")
  }else{
    print("Growing Neural Gas")
  }
  if(exists("object$call")){
    print(object$call)
  }
  if(object$hasStarted()){
    print(sprintf("%d nodes with mean error %f", 
                  object$getNumberNodes(), object$getMeanError()))
    
    print(sprintf("Trained %d iterations", object$getCurrentIteration()))
    print("Mean errors[s]: ")
    errors = object$getErrorStatistics()
    if(length(errors) > 10){
      errors = errors[(length(errors)-10):length(errors)]
    }
    
    print(errors)
  }
}


node.gng <- function(x, gng_id){
  x$getNode(gng_id)
}

run.gng <- function(object){
  # Invalidate components
  assign("components.membership", NULL, object)
  object$run()
}

pause.gng <- function(object){
  object$pause()
  n = 0.0
  sleep = 0.1
  while(object$isRunning()){
    Sys.sleep(sleep)  
    n = n + 1
    if(n > 2/sleep){
      print("Warning: GNG has not paused! Check status with gng$isRunning(). Something is wrong.")
      return()
    }
  }
}

terminate.gng <- function(object){
  object$terminate()
}

meanError.gng <- function(object){
  object$getMeanError()
}  

errorStatistics.gng <- function(object){
  object$getErrorStatistics()
}  

clustering.gng <- function(c){
  c$clustering()
}  

gngSave <- function(object, filename){
  warning("Saving does not preserve training history")
  object$.save(filename)
}

gngLoad <- function(filename){
  warning("Saving does not preserve training history")
  fromFileGNG(filename)
}



predictCentroids  <- function(object, community.detection.algorithm=spinglass.community){
  ig <- convertToGraph(object)
  
  cl = clusters(ig)
  components = lapply(levels(as.factor(cl$membership)), function(x) induced.subgraph(ig, cl$membership==as.numeric(x)))
  
  centroids <- c()
  for(cc in components){
    communities <- community.detection.algorithm(cc)
    for(i in 1:length(communities)){
      #Get subcommunity
      community_graph <- induced.subgraph(cc, which(membership(communities)==i))
      #Get index of centroid (which is ordered by betwenness)
      centroid_index = which(order(betweenness(community_graph))==1)
      # Append
      centroids<- c(centroids, V(community_graph)$index[centroid_index])
    }
  }
  centroids
}


convertToGraph.gng <- function(object, calculate.dist=TRUE){
  was_running = object$isRunning()
  if(was_running){
    pause(object)
  }
  
  if(object$getNumberNodes() == 0){
    return(graph.empty(n=0, directed=FALSE))
  }
  
  #Prepare index map. Rarely there is a difference in indexing
  #due to a hole in memory representation of GNG graph (i.e.
  #indexing in gng can be non-continuous)
  
  # Warning: This is a hack. If there is a bug look for it here
  indexesGNGToIGraph <- 1:(2*object$.getLastNodeIndex()) 
  indexesIGraphToGNG <- 1:object$getNumberNodes()
  
  if(object$.getLastNodeIndex() != object$getNumberNodes()){
    igraph_index = 1
    for(i in (1:object$.getLastNodeIndex())){
      node <- node(object, i)
      if(length(node) != 0){
        indexesGNGToIGraph[i] = igraph_index
        indexesIGraphToGNG[igraph_index] = i
        igraph_index = igraph_index + 1
      }
    }
  }
  
  adjlist<-list()
  for(i in 1:object$.getLastNodeIndex()){
    node <- node(object, i)
    if(length(node) != 0){
      
      igraph_index = indexesGNGToIGraph[i]
      #print(paste(object$.getLastNodeIndex(), length(indexesGNGToIGraph), object$isRunning()))
      #print(paste(igraph_index, node$neighbours))
      neighbours = node$neighbours[node$neighbours > i]
      adjlist[[igraph_index]] <- sapply(neighbours, function(x){ indexesGNGToIGraph[x] })
    } else{
      #print("Empty node")
    }
  }
  
  #print("Creating the graph")
  
  g <- graph.adjlist(adjlist, mode = "all", duplicate=FALSE)
  for(i in 1:object$.getLastNodeIndex()){
    node <- node(object, i)
    if(length(node) != 0){
      igraph_index = indexesGNGToIGraph[i]
      #TODO: it is more efficient to assign whole vectors
      #TODO: refactor in whole code v0 v1 v2 to pos_1 pos_2 pos_3
      V(g)[igraph_index]$v0 <- node$pos[1]
      V(g)[igraph_index]$v1 <- node$pos[2]
      V(g)[igraph_index]$v2 <- node$pos[3]
      V(g)[igraph_index]$label <- node$label
      V(g)[igraph_index]$error <- node$error
      V(g)[igraph_index]$index <- node$index
      if(!is.null(node$utility)){
        V(g)[igraph_index]$utility = node$utility
      }
    } 
  }
  
  if(calculate.dist){
    # Add distance information
    dists <- apply(get.edges(g, E(g)), 1, function(x){ 
      object$nodeDistance(indexesIGraphToGNG[x[1]], indexesIGraphToGNG[x[2]])
    })
    E(g)$dists = dists
  }
  
  if(was_running){
    run(object)
  }
  
  g
}


findClosests <- function(object, node.ids, x){
  .findClosests <- function(object, node.ids, x){
    # Returns all dists from given pos to given nodes
    get_all_dists <- function(pos, nodes, gng){
      sapply(nodes, function(node_index) sqrt(sum((pos-node(gng, node_index)$pos)^2)))
    }
    
    which.min(get_all_dists(x, node.ids, object))
  }
  if( is.vector(x)){
    .findClosests(object, node.ids, x)
  }else{
    if ( !is(x, "data.frame") && !is(x, "matrix") && !is(x,"numeric")  ) {
      stop(gmum.error(GMUM_WRONG_PARAMS, "Wrong target class, please provide data.frame, matrix or numeric vector"))
    }
    
    if (!is(x, "matrix")) {
      x <- data.matrix(x)
    }
    
    y <- rep(NA, nrow(x))
    
    for(i in 1:nrow(x)){
      y[i] <- .findClosests(object, node.ids, x[i,])
    }
    
    y
  }
}

#' Insert examples
#' 
#' @note It copies your examples twice in RAM. You might want to use object$insertExamples, or
#' not to copy at all set_memory_move_examples (when using this function, remember not to modify the matrix
#' and after removing the object delete it aswell)
insertExamples.Rcpp_GNGServer <- function(object, examples, labels=c()){   
  
  
  if(length(labels) == 0){
    object$insertExamples(examples)
  }else if(typeof(labels) == "character"){
    if(typeof(labels) == "list"){
      if(is.null(examples$labels)){
        stop(gmum.error(GMUM_WRONG_PARAMS, "Empty labels column"))
      }else{
        label.column <- examples$labels
        examples$labels <- NULL
        object$insertLabeledExamples(examples, label.column)
      }
    }else{
      stop(gmum.error(GMUM_WRONG_PARAMS, "Please pass data frame"))
    }
  }else{
    object$insertLabeledExamples(examples, labels)
  }    
}


generateExamples <- function(preset, N, r=1.0, center=c(0.5,0.5,0.5)){
  preset(N, center=center, r=r, prob=-1)
}


loadModule('gng_module', TRUE)

setGeneric("node", 
           function(x, gng_id, ...) standardGeneric("node"))

setGeneric("clustering", 
           function(c) standardGeneric("clustering"))


setGeneric("convertToGraph", 
           function(object, ...) standardGeneric("convertToGraph"))



setGeneric("run", 
           function(object, ...) standardGeneric("run"))

setGeneric("pause", 
           function(object, ...) standardGeneric("pause"))

setGeneric("terminate", 
           function(object, ...) standardGeneric("terminate"))


setGeneric("insertExamples", 
           function(object, ...) standardGeneric("insertExamples"))



setGeneric("meanError", 
           function(object, ...) standardGeneric("meanError"))


setGeneric("errorStatistics", 
           function(object, ...) standardGeneric("errorStatistics"))


setGeneric("numberNodes", 
           function(object, ...) standardGeneric("numberNodes"))


# Lazy loading to allow for discovery of all files
evalqOnLoad( {
    
  # Autocompletion fix

  .GlobalEnv$`.DollarNames.C++Object` <- function( x, pattern ){
    grep(pattern, asNamespace("Rcpp")$complete(x), value = TRUE)[! (substr(grep(pattern, asNamespace("Rcpp")$complete(x), value = TRUE),1,1)==".")]
  }
  
  #.GlobalEnv$DollarNamesGmumr <- function( x, pattern ){
  #  asNamespace("Rcpp")$`.DollarNames.C++Object`(x, pattern)[! (substr(asNamespace("Rcpp")$`.DollarNames.C++Object`(x, pattern),1,1)==".")]
  #}
  #environment(.GlobalEnv$DollarNamesGmumr) <- .GlobalEnv
  #setMethod( ".DollarNames", "C++Object", .GlobalEnv$DollarNamesGmumr )

  setMethod("insertExamples", "Rcpp_GNGServer", insertExamples.Rcpp_GNGServer)
  setMethod("plot",  "Rcpp_GNGServer", plot.gng)
  setMethod("print",  "Rcpp_GNGServer", print.gng)
  setMethod("summary", "Rcpp_GNGServer", summary.gng)
  setMethod("show", "Rcpp_GNGServer", summary.gng)
  
  setMethod("node", signature("Rcpp_GNGServer","numeric"), node.gng)
  setMethod("run", "Rcpp_GNGServer", run.gng)
  setMethod("pause", "Rcpp_GNGServer", pause.gng)
  setMethod("terminate", "Rcpp_GNGServer", terminate.gng)
  setMethod("meanError", "Rcpp_GNGServer", meanError.gng) 
  setMethod("errorStatistics", "Rcpp_GNGServer", errorStatistics.gng) 
  
  #'Get number of nodes
  setMethod("numberNodes" ,
            "Rcpp_GNGServer",
            function(object){
              object$getNumberNodes()
            })
  
  
    

  setMethod("convertToGraph" ,
            "Rcpp_GNGServer",
            convertToGraph.gng)


  setMethod("clustering" ,
            "Rcpp_GNGServer",
            clustering.gng)



  setMethod("predict" ,
            "Rcpp_GNGServer",
            function(object, x){
                if( is.vector(x)){
                    object$predict(x)
                }else{
                  if ( !is(x, "data.frame") && !is(x, "matrix") && !is(x,"numeric")  ) {
                    stop(gmum.error(GMUM_WRONG_PARAMS, "Wrong target class, please provide data.frame, matrix or numeric vector"))
                  }
                  
                  if (!is(x, "matrix")) {
                    x <- data.matrix(x)
                  }
                  
                  y <- rep(NA, nrow(x))
                  
                  for(i in 1:nrow(x)){
                    y[i] <- object$predict(x[i,])
                  }
                  
                  y
                }
            })
  
  
  methods = list()
  for(name in names(GNGConfiguration@methods)){
    methods[[name]] = eval(substitute(
      function(...) .CppObject$WHAT(...), list(WHAT = as.name(name)))) 
  }
  
  methods[["initialize"]] <- function(...){
    
  }

})

