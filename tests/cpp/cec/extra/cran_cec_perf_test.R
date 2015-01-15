library(devtools)
load_all('../../../../../cran-cec')
#options(scipen=999)

data_path = './data/'
log_file = './cran_cec.log'

cat("points\ttime(s)\titerations\n", file=log_file, append=FALSE)

for(i in seq(100, 10000, by=100))
{
    filename = paste(c(i, '.RData'), collapse='')
    points = as.matrix(read.table(file.path(data_path, filename)))
    t = as.numeric(system.time(c <- cec(centers=3, nstart=1, x=points, centers.init='random', type='spherical'))[3])
    #cat(i/2, t, c$iterations, '\n', file=log_file, append=TRUE, sep='\t')
    cat(t, c$iterations, '\n', file=log_file, append=TRUE, sep='\t')
}
