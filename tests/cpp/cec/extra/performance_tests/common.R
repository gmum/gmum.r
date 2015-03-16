method_types = list(gmum=c('standard', 'sphere', 'diagonal'), cran=c('all', 'spherical', 'diagonal'))
nmethod_types = length(method_types$gmum)

append_result <- function(method_type, points, times, max_iter, results, cec_function) {
    t = 0
    it = 0
    e = 0
    for(i in 1:times)
    {
        c = cec_function(method_type, points, max_iter)
        t = t + c$time
        it = it + c$iters
        e = e + c$energy
    }
    results$time = c(results$time, t)
    results$iters = c(results$iters, it)
    results$energy = c(results$energy, e)
    return (results)
}