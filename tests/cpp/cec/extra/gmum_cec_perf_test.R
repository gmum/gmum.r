library(devtools)
load_all('../../../../')
#options(scipen=999)

data_path = './data/'
log_file = './gmum_cec.log'

cat("points\ttime(s)\titerations\n", file=log_file, append=FALSE)

for(i in seq(100, 10000, by=100))
{
    filename = paste(c(i, '.RData'), collapse='')
    points = as.matrix(read.table(file.path(data_path, filename)))
    t = as.numeric(system.time(c <- CEC(k=3, control.nstart=1, x=points, method.init='random', method.type='sphere',log.energy=1))[3])
    l = length(c$energy())
    #cat(i/2, t, l, '\n', file=log_file, append=TRUE, sep='\t')
    cat(t, l, '\n', file=log_file, append=TRUE, sep='\t')
}
