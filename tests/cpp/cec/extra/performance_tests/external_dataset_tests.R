source('common.R')
library(phyclust)
library(nlme)

data_path <- '../../../../../inst/data_sets/cec'
plots_path <- file.path('.', 'plots')
gmum_plot_path <- file.path(plots_path, 'gmum')
cran_plot_path <- file.path(plots_path, 'cran')
dir.create(gmum_plot_path, showWarnings = FALSE, recursive = TRUE)
dir.create(cran_plot_path, showWarnings = FALSE, recursive = TRUE)
test_data = list()

#test_data[['DimSet']] = list(
#    DimSets_32 = load_dataset(data_path = file.path(data_path, "DimSets_32")),
#    DimSets_64 = load_dataset(data_path = file.path(data_path, "DimSets_64")),
#    DimSets_128 = load_dataset(data_path = file.path(data_path, "DimSets_128")),
#    DimSets_256 = load_dataset(data_path = file.path(data_path, "DimSets_256")),
#    DimSets_512 = load_dataset(data_path = file.path(data_path, "DimSets_512"))
#    DimSets_1024 = load_dataset(data_path = file.path(data_path, "DimSets_1024"))
#)

test_data[['UCI']] = list(
    iris = load_dataset(data_path = file.path(data_path, "UCI","iris")),
    glass = load_dataset(data_path = file.path(data_path, "UCI", "glass")),
    vowel = load_dataset(data_path = file.path(data_path, "UCI", "vowel")),
    wine = load_dataset(data_path = file.path(data_path, "UCI", "wine")),
    pendigits = load_dataset(data_path = file.path(data_path, "UCI", "pendigits")),
    poker = load_dataset(data_path = file.path(data_path, "UCI", "poker")),
    connect_4 = load_dataset(data_path = file.path(data_path, "UCI", "connect_4"))
    #covtype = load_dataset(data_path = file.path(data_path, "UCI", "covtype"))
)

gmum_cec_uci <- function(method_type, points, nclusters, output_plot_path = NULL) {
    return (gmum_cec(nstart = 1, init_type = 'kmeans++', max_iterations = 200, method_type = method_type, points = points, nclusters = nclusters, eps = 0.01, output_plot_path))     
}

cran_cec_uci <- function(method_type, points, nclusters, output_plot_path = NULL) {
    return (cran_cec(nstart = 1, init_type = 'kmeans++', max_iterations = 200, method_type = method_type, points = points, nclusters = nclusters, eps = "1%", output_plot_path))
}

for(name in names(test_data)) {    
    for(i in 1:length(test_data[[name]])) {
        item = test_data[[name]][[i]]        
        dataset = item$dataset
        dataset_dim = dim(dataset)

        cat(paste(name, '/', item$name, '(k=',item$k,', rows=', dataset_dim[1], ', cols=', dataset_dim[2], ')\n'))
        for(j in 1:nmethod_types) {
            plot_file_name <- paste(c(name, item$name, method_types$gmum[j], '.jpg'), collapse='_')
            
            gmum <- tryCatch({
                gmum_result <- gmum_cec_uci(method_type = method_types$gmum[j], points = dataset, nclusters = item$k)
                gmum_clustering_df <- data.frame(gmum_result$clustering, item$clustering)
                colnames(gmum_clustering_df) <- c('gmum', 'correct')
                gmum_rand_index <- RRand(prcl = gmum_result$clustering, trcl = item$clustering)$Rand
                gmum_energy = gmum_calculate_energy(points = dataset, centers = gmum_result$centers, method_type = method_types$gmum[j])
                cran_energy = cran_calculate_energy(points = dataset, centers = gmum_result$centers, method_type = method_types$cran[j])
                if(gmum_rand_index < 0) {
                    print('rand index < 0!')
                    print('gmum clustering')
                    print(gmum_result$clustering)
                    print('correct clustering')
                    print(item$clustering)
                }
                gmum_bic <- BIC(lm(gmum ~ correct, data=gmum_clustering_df))
                list(
                    energy = gmum_result$energy,
                    cran_energy = cran_energy,
                    gmum_energy = gmum_energy,
                    iters = gmum_result$iters,
                    time = gmum_result$time,
                    final_nclusters = gmum_result$final_nclusters,
                    rand_index = gmum_rand_index,
                    bic = gmum_bic
                )
            }, error = function(e) {
                cat(paste('gmum: error occured: ', e))
                return(list(
                energy = 'failed',
                iters = 'failed',
                time = 'failed',
                final_nclusters = 'failed',
                rand_index = 'failed',
                bic = 'failed'))     
            })            
            
            cran <- tryCatch({
                cran_result <- cran_cec_uci(method_type = method_types$cran[j], points = dataset, nclusters = item$k)
                cran_clustering_df <- data.frame(cran_result$clustering, item$clustering)
                colnames(cran_clustering_df) <- c('cran', 'correct')
                cran_rand_index <- RRand(prcl = cran_result$clustering, trcl = item$clustering)$Rand   
                gmum_energy = gmum_calculate_energy(points = dataset, centers = cran_result$centers, method_type = method_types$gmum[j])
                cran_energy = cran_calculate_energy(points = dataset, centers = cran_result$centers, method_type = method_types$cran[j])                
                if(cran_rand_index < 0) {
                    print('rand index < 0!')
                    print('cran clustering')
                    print(cran_result$clustering)
                    print('correct clustering')
                    print(item$clustering)
                }
                cran_bic <- BIC(lm(cran ~ correct, data=cran_clustering_df))            
                list(
                    energy = cran_result$energy,
                    cran_energy = cran_energy,
                    gmum_energy = gmum_energy,
                    iters = cran_result$iters,
                    time = cran_result$time,
                    final_nclusters = cran_result$final_nclusters,
                    rand_index = cran_rand_index,
                    bic = cran_bic
                )
            }, error = function(e) {
                cat(paste('cran: error occured: ', e))               
                return(list(
                    energy = 'failed',
                    iters = 'failed',
                    time = 'failed',
                    final_nclusters = 'failed',
                    rand_index = 'failed',
                    bic = 'failed'))     
            })
            
            table_data <- matrix(
                c(gmum$energy, cran$energy,
                  gmum$gmum_energy, cran$gmum_energy,
                  gmum$cran_energy, cran$cran_energy,
                  gmum$iters, cran$iters,
                  gmum$time, cran$time,
                  gmum$final_nclusters, cran$final_nclusters,
                  gmum$rand_index, cran$rand_index,
                  gmum$bic, cran$bic),
                ncol=2, 
                byrow=TRUE
            )
            
            cat('method type: ', method_types$gmum[j], '\n')
            table_data <- as.table(table_data)
            colnames(table_data) <- c('gmum', 'cran')
            rownames(table_data) <- c('energy','gmum energy f','cran energy f' ,'iters', 'time', 'clusters', 'rand index', 'BIC')            
            print(table_data)
            cat('\n')
        }
    }
}
