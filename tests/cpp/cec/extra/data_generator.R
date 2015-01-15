data_path = './data/'
for(i in seq(100, 10000, by=100))
{
    data = rnorm(i, mean = 3, sd = 2)
    filename = paste(c(data_path, i,'.RData'), collapse="")
    write.table(data, file=filename, row.names=FALSE, col.names=FALSE)    
}

