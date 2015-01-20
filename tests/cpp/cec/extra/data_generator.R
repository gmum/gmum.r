source('common.R')
for(i in seq(min_npoints * 2, max_npoints * 2, by=npoints_step * 2))
{
    gauss_1 = rnorm(i / 2, mean = 0, sd = 1)
    gauss_2 = rnorm(i / 2, mean = 2, sd = 0.8)
    data = c(gauss_1, gauss_2)
    filename = paste(c(data_path, i), collapse="")
    write.table(data, file=filename, row.names=FALSE, col.names=FALSE)    
}