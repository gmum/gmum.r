data_path = './data/'
for(i in seq(100, 10000, by=100))
{
    gauss_1 = rnorm(i / 2, mean = 0, sd = 1)
    gauss_2 = rnorm(i / 2, mean = 2, sd = 0.8)
    data = c(gauss_1, gauss_2)
    filename = paste(c(data_path, i,'.RData'), collapse="")
    write.table(data, file=filename, row.names=FALSE, col.names=FALSE)    
}

