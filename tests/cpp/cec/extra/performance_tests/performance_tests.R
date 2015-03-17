source('common.R')
source('plot_generator.R')
source('../../../../../R/R_scripts/mouseGaussGenerator.R')
library(gmum.r)
library(CEC)

run_tests <- function(npoints_start, ndatasets, npoints_step, averaging, gmum_cec_function, cran_cec_function) {
    results = list(
        gmum=list(
            standard=list(time=c(), iters=c(), energy=c()), 
            sphere=list(time=c(), iters=c(), energy=c()), 
            diagonal=list(time=c(), iters=c(), energy=c())
        ),
        cran=list(
            all=list(time=c(), iters=c(), energy=c()),
            spherical=list(time=c(), iters=c(), energy=c()),
            diagonal=list(time=c(), iters=c(), energy=c())
        )
    )
    x = seq(npoints_start, npoints_start + ndatasets * npoints_step, by=npoints_step)    
    
    for(i in x) {
        #generate dataset
        size_of_data <- c(i,i,i)
        ear_distance <- 2
        dataset <- mouseGaussGenerator(size_of_data, ear_distance)$data
        
        for(j in 1:nmethod_types) {            
            gmum_method_type_name = method_types$gmum[j]
            cran_method_type_name = method_types$cran[j]
            
            results$gmum[[gmum_method_type_name]] = append_result(gmum_method_type_name, dataset, averaging, results$gmum[[gmum_method_type_name]], gmum_cec_function)
            results$cran[[cran_method_type_name]] = append_result(cran_method_type_name, dataset, averaging, results$cran[[cran_method_type_name]], cran_cec_function)
        }            
    }    
    
    for(j in 1:nmethod_types) {    
        method_type_name = method_types$gmum[j]        
        x_label_ = "Number of points"
        y1_label_ = "gmum"
        y2_label_ = "cran"
        
        plot_name_ = paste(c(method_type_name,' times plot'), collapse='')
        y1_ = results$gmum[[method_types$gmum[j]]]$time
        y2_ = results$cran[[method_types$cran[j]]]$time
        y_label_ = "Time"
        file_name_ = paste(c(method_type_name,'_times_plot.jpg'), collapse='')    
        generate_plot(plot_name_, x, y1_, y2_, x_label_, y_label_, y1_label_, y2_label_, file_name_)       
        
        plot_name_ = paste(c(method_type_name,' energy plot'), collapse='')
        y1_ = results$gmum[[method_types$gmum[j]]]$energy
        y2_ = results$cran[[method_types$cran[j]]]$energy
        y_label_ = "Energy"
        file_name_ = paste(c(method_type_name,'_energy_plot.jpg'), collapse='')
        generate_plot(plot_name_, x, y1_, y2_, x_label_, y_label_, y1_label_, y2_label_, file_name_)
        
        plot_name_ = paste(c(method_type_name,' time/iter plot'), collapse='')
        y1_ = results$gmum[[method_types$gmum[j]]]$time / results$gmum[[method_types$gmum[j]]]$iters 
        y2_ = results$cran[[method_types$cran[j]]]$time / results$cran[[method_types$cran[j]]]$iters
        x_label_ = "Number of points"
        y_label_ = "Time / iter"
        file_name_ = paste(c(method_type_name,'_time_iter_plot.jpg'), collapse='')
        generate_plot(plot_name_, x, y1_, y2_, x_label_, y_label_, y1_label_, y2_label_, file_name_)
    }            
}

perf_gmum_cec <- function(method_type, points) {    
    return (gmum_cec(nclusters = 3, nstart = 1, points = points, init_type = 'random', method_type = method_type, max_iterations = 200, eps = 0.05))
}

perf_cran_cec <- function(method_type, points) {    
    return (cran_cec(nclusters = 3, nstart = 1, points = points, init_type = 'random', method_type = method_type, max_iterations = 200, eps = "5%"))
}

run_tests(npoints_start=200, ndatasets=10, npoints_step=500, averaging=5, gmum_cec_function=perf_gmum_cec, cran_cec_function=perf_cran_cec)
