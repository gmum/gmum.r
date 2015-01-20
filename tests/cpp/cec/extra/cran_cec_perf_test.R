source("common.R")
library(devtools)
#load_all(cran_cec_path)
library(CEC)

log_file = cran_cec_log_path
result = matrix(ncol=2, nrow=((max_npoints - min_npoints) / npoints_step) + 1)
result_iter = 1

for(i in seq(min_npoints * 2, max_npoints * 2, by=npoints_step * 2))
{
    filename = paste(i)
    points = as.matrix(read.table(file.path(data_path, filename)))
    t = as.numeric(system.time(c <- cec(centers=3, nstart=1, x=points, centers.init='random',iter.max=cran_max_iters, type='spherical'))[3])
    result[result_iter, ] = c(t, c$iterations)
    result_iter = result_iter + 1
}
write.table(result, file=log_file, row.names=FALSE, col.names=FALSE)
