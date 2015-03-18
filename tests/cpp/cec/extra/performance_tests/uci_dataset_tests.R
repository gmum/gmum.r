source('common.R')
library(fossil)
library(nlme)
library(gmum.r)
library(CEC)

test_files = list(
#    Birch_Sets=list(
#        list(url='http://cs.joensuu.fi/sipu/datasets/birch1.txt', M=100, name='birch1'),
#        list(url='http://cs.joensuu.fi/sipu/datasets/birch2.txt', M=100, name='birch2'),
#        list(url='http://cs.joensuu.fi/sipu/datasets/birch3.txt', M=100, name='birch3')
#    ),
#    S_Sets=list(
#        list(url='http://cs.joensuu.fi/sipu/datasets/s1.txt', M=15, name='s1'),
#        list(url='http://cs.joensuu.fi/sipu/datasets/s2.txt', M=15, name='s2'),
#        list(url='http://cs.joensuu.fi/sipu/datasets/s3.txt', M=15, name='s3'),
#        list(url='http://cs.joensuu.fi/sipu/datasets/s4.txt', M=15, name='s4')
#    ),
#    A_Sets=list(
#        list(url='http://cs.joensuu.fi/sipu/datasets/a1.txt', M=20, name='a1'),
#        list(url='http://cs.joensuu.fi/sipu/datasets/a2.txt', M=35, name='a2'),
#        list(url='http://cs.joensuu.fi/sipu/datasets/a3.txt', M=50, name='a3')
#    ),
    UCI_Sets=list(
        list(url='http://cs.joensuu.fi/sipu/datasets/thyroid.txt', M=2, name='thyroid'),
        list(url='http://cs.joensuu.fi/sipu/datasets/wine.txt', M=3, name='wine'),
        list(url='http://cs.joensuu.fi/sipu/datasets/yeast.txt', M=10, name='yeast'),
        list(url='http://cs.joensuu.fi/sipu/datasets/breast.txt', M=2, name='breast'),
        list(url='http://cs.joensuu.fi/sipu/datasets/iris.txt', M=3, name='iris'),
        list(url='http://cs.joensuu.fi/sipu/datasets/glass.txt', M=7, name='glass')        
    )
)

gmum_cec_uci <- function(method_type, points, nclusters, output_plot_path = NULL) {
    return (gmum_cec(nstart = 5, init_type = 'kmeans++', max_iterations = 200, method_type = method_type, points = points, nclusters = nclusters, eps = 0.01, output_plot_path))     
}

cran_cec_uci <- function(method_type, points, nclusters, output_plot_path = NULL) {
    return (cran_cec(nstart = 5, init_type = 'kmeans++', max_iterations = 200, method_type = method_type, points = points, nclusters = nclusters, eps = 1, output_plot_path))
}

data_path <- file.path('.', 'data')
plots_path <- file.path('.', 'plots')
gmum_plot_path <- file.path(plots_path, 'gmum')
cran_plot_path <- file.path(plots_path, 'cran')

dir.create(data_path, showWarnings = FALSE, recursive = TRUE)
dir.create(gmum_plot_path, showWarnings = FALSE, recursive = TRUE)
dir.create(cran_plot_path, showWarnings = FALSE, recursive = TRUE)

for(name in names(test_files)) {
    d = file.path('.', 'data', name)
    dir.create(d, showWarnings = FALSE)
    
    for(i in 1:length(test_files[[name]])) {
        item = test_files[[name]][[i]]
        fpath = file.path(d, item$name)
        
        if(file.exists(fpath) == FALSE) {
            download.file(url = item$url, destfile = fpath)
        }
        
        dataset = as.matrix(read.table(file = fpath, colClasses='numeric'))
        
        cat(name, '/', item$name, '( M =',item$M,')' ,'\n')
        for(j in 1:nmethod_types) {
            plot_file_name <- paste(c(name, item$name, method_types$gmum[j], '.jpg'), collapse='_')
            gmum_result <- gmum_cec_uci(method_type = method_types$gmum[j], points = dataset, nclusters = item$M, file.path(gmum_plot_path, plot_file_name))
            cran_result <- cran_cec_uci(method_type = method_types$cran[j], points = dataset, nclusters = item$M)
            
            clustering_df <- data.frame(gmum_result$clustering, cran_result$clustering)
            colnames(clustering_df) <- c('gmum', 'cran')
            
            table_data <- matrix(
                c(gmum_result$energy, cran_result$energy,
                  gmum_result$iters, cran_result$iters,
                  gmum_result$time, cran_result$time,
                  length(gmum_result$centers), length(cran_result$centers),
                  rand.index(gmum_result$clustering, cran_result$clustering), '',
                  BIC(lm(gmum ~ cran, data=clustering_df)), ''),
                ncol=2, 
                byrow=TRUE
            )
            
            cat('method type: ', method_types$gmum[j], '\n')
            table_data <- as.table(table_data)
            colnames(table_data) <- c('gmum', 'cran')
            rownames(table_data) <- c('energy', 'iters', 'time', 'clusters', 'rand index', 'BIC')            
            print(table_data)
            cat('\n')
        }
    }
}
