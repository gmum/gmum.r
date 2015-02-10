source("common.R")
library(devtools)
load_all(gmum_cec_path)

log_file = gmum_cec_log_path
results = list()
for(i in 1:method_type_num)
    results[[i]] <- matrix(ncol=2, nrow=((max_npoints - min_npoints) / npoints_step) + 1)
result_iter = 1

for(i in seq(min_npoints * 2, max_npoints * 2, by=npoints_step * 2))
{
    filename = paste(i)
    points = as.matrix(read.table(file.path(data_path, filename)))
    for(j in 1:method_type_num)
    {
        method_type = gmum_cran_method_type_map[j,][1]
        t = 0
        for(i in 1:averaging)
        {
            t = t + as.numeric(system.time(c <- CEC(k=3, control.nstart=1, x=points, method.init='random', method.type=method_type, log.energy=1))[3])
        }
        l = length(c$energy())
        results[[j]][result_iter, ] = c(t / averaging, l)
    } 
    result_iter = result_iter + 1
}

for(i in 1:method_type_num)
{
    file_path = get_gmum_result_file_path(i) 
    write.table(results[[i]], file=file_path, row.names=FALSE, col.names=FALSE)
} 
