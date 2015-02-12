source("common.R")
library(devtools)
#load_all(cran_cec_path)
library(CEC)

log_file = cran_cec_log_path
results = list() 
for(i in 1:method_type_num)
    results[[i]] <- matrix(ncol=3, nrow=((max_npoints - min_npoints) / npoints_step) + 1)
result_iter = 1

for(i in seq(min_npoints * 2, max_npoints * 2, by=npoints_step * 2))
{
    filename = paste(i)
    points = as.matrix(read.table(file.path(data_path, filename)))
    for(j in 1:method_type_num)
    {
        method_type = gmum_cran_method_type_map[j,][2]
        t = 0
        l = 0
        e = 0 
        for(i in 1:averaging)
        {
            t = t + as.numeric(system.time(c <- cec(centers=3, nstart=1, x=points, centers.init='random',iter.max=cran_max_iters, type=method_type))[3])
            l = l + c$iterations
            e = e + tail(c$cost, n=1)
        }
        results[[j]][result_iter, ] = c(t / averaging, l / averaging, e / averaging )
    }
    result_iter = result_iter + 1
}

for(i in 1:method_type_num)
{
    file_path = get_cran_result_file_path(i)
    write.table(results[[i]], file=file_path, row.names=FALSE, col.names=FALSE)
}
