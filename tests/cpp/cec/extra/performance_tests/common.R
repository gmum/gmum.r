library(gmum.r)
library(CEC)
source('../../../../../R/R_scripts/energy.R')
method_types = list(gmum=c('standard', 'spherical', 'diagonal'), cran=c('all', 'spherical', 'diagonal'))
entropy_func_map = list(standard_entropy, sphere_entropy, diagonal_entropy)
nmethod_types = length(method_types$gmum)

load_dataset <- function(data_path) {
    points_file = file.path(data_path, 'input.txt')
    clusters_file = file.path(data_path, 'cluster.txt')
    
    if(!file.exists(points_file)) {
        stop(points_file, ": File does not exist")
    }
    
    if(!file.exists(clusters_file)) {
        stop(clusters_file, ": File does not exist")
    }
    
    dataset <- as.matrix(read.table(file=points_file, colClasses='numeric'))
    clusters <- normalize_clustering(as.vector(as.matrix(read.table(file=clusters_file))))
    k <- length(unique(clusters))
    name <- basename(data_path) 
    return(list(name=name, k=k, clustering=clusters, dataset=dataset))
}

normalize_clustering <- function(clustering) {
    return( (clustering - min(clustering)) + 1 )
}

gmum_cec <- function(nclusters, nstart, points, init_type, method_type, max_iterations, eps, output_plot_path = NULL) {        
    t = as.numeric(system.time(c <- CEC(k=nclusters, control.nstart=nstart, x=points, method.init=init_type, method.type=method_type, control.itmax=max_iterations, control.eps=eps, log.ncluster=TRUE))[3])
    if( !is.null(output_plot_path) ) {
        jpeg(output_plot_path)
        plot(c)
        dev.off()
    }    
    
    time_ = t
    iters_ = c$iterations
    energy_ = c$energy
    clustering_ = normalize_clustering(c$clustering)
    centers_ = c$centers
    final_nclusters_ = tail(c$logNumberOfClusters, n=1)    
    
    return (list(
        time=time_, 
        iters=iters_, 
        energy=energy_, 
        clustering=clustering_, 
        centers=centers_,
        final_nclusters=final_nclusters_))
}

cran_cec <- function(nclusters, nstart, points, init_type, method_type, max_iterations, eps, output_plot_path = NULL) {    
    t = as.numeric(system.time(c <- cec(centers=nclusters, nstart=nstart, x=points, centers.init=init_type, type=method_type, iter.max=max_iterations, card.min=eps))[3])
    
    if( !is.null(output_plot_path) ) {
        jpeg(output_plot_path)
        plot(c)
        dev.off()
    } 
    
    return (list(
        time=t, 
        iters=c$iterations,
        energy=c$final.cost,
        clustering=normalize_clustering(c$cluster),
        centers=c$centers,
        final_nclusters=c$final.nclusters))
}

append_result <- function(method_type, points, times, results, cec_function) {
    t = 0
    it = 0
    e = 0
    for(i in 1:times)
    {
        c = cec_function(method_type, points)
        t = t + c$time
        it = it + c$iters
        e = e + c$energy
    }
    results$time = c(results$time, t)
    results$iters = c(results$iters, it)
    results$energy = c(results$energy, e)
    return (results)
}
