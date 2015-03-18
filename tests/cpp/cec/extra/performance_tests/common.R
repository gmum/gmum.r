method_types = list(gmum=c('standard', 'sphere', 'diagonal'), cran=c('all', 'spherical', 'diagonal'))
nmethod_types = length(method_types$gmum)

normalize_clustering <- function(clustering) {
    return (clustering - min(clustering))
}

gmum_cec <- function(nclusters, nstart, points, init_type, method_type, max_iterations, eps, output_plot_path = NULL) {        
    t = as.numeric(system.time(c <- CEC(k=nclusters, control.nstart=nstart, x=points, method.init=init_type, method.type=method_type, control.itmax=max_iterations, control.eps=eps))[3])
    
    if( !is.null(output_plot_path) ) {
        jpeg(output_plot_path)
        plot(c)
        dev.off()
    }    
    
    return (list(
        time=t, 
        iters=c$log.iters(), 
        energy=c$energy(), 
        clustering=normalize_clustering(c$y()), 
        centers=c$centers()))
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
        energy=tail(c$cost, n=1),
        clustering=normalize_clustering(c$cluster),
        centers=c$centers))
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