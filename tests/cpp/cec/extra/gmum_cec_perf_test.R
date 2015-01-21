source("common.R")
library(devtools)
load_all(gmum_cec_path)

log_file = gmum_cec_log_path
result = matrix(ncol=2, nrow=((max_npoints - min_npoints) / npoints_step) + 1)
result_iter = 1

for(i in seq(min_npoints * 2, max_npoints * 2, by=npoints_step * 2))
{
    filename = paste(i)
    points = as.matrix(read.table(file.path(data_path, filename)))
    t = as.numeric(system.time(c <- CEC(k=3, control.nstart=1, x=points, method.init='random', method.type='sphere',log.energy=1))[3])
    t = t + as.numeric(system.time(c <- CEC(k=3, control.nstart=1, x=points, method.init='random', method.type='sphere',log.energy=1))[3])
    t = t + as.numeric(system.time(c <- CEC(k=3, control.nstart=1, x=points, method.init='random', method.type='sphere',log.energy=1))[3])
    l = length(c$energy())
    result[result_iter, ] = c(t*0.333, l)
    result_iter = result_iter + 1
}

write.table(result, file=log_file, row.names=FALSE, col.names=FALSE)
