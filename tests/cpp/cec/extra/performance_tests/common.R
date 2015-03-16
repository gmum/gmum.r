method_types = list(gmum=c('standard', 'sphere', 'diagonal'), cran=c('all', 'spherical', 'diagonal'))
nmethod_types = length(method_types$gmum)

append_gmum_result <- function(method_type, points, times, max_iter, gmum_results) {
    t = 0
    it = 0
    e = 0
    for(i in 1:times)
    {
        t = t + as.numeric(system.time(c <- CEC(k=3, control.nstart=1, x=points, method.init='random', method.type=method_type, control.itmax=max_iter))[3])
        it = it + c$log.iters()
        e = e + c$energy()
    }
    gmum_results$time = c(gmum_results$time, t)
    gmum_results$iters = c(gmum_results$iters, it)
    gmum_results$energy = c(gmum_results$energy, e)
    return (gmum_results)
}

append_cran_result <- function(method_type, points, times, max_iter, cran_results) {
    t = 0
    it = 0
    e = 0
    for(i in 1:times)
    {
        t = t + as.numeric(system.time(c <- cec(centers=3, nstart=1, x=points, centers.init='random', type=method_type, iter.max=max_iter))[3])
        it = it + c$iterations
        e = e + tail(c$cost, n=1)
    }
    
    cran_results$time = c(cran_results$time, t)
    cran_results$iters = c(cran_results$iters, it)
    cran_results$energy = c(cran_results$energy, e)
    return (cran_results)
}