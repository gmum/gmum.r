source('common.R')
source('../../../../R/R_scripts/mouseGaussGenerator.R')

for(i in seq(min_npoints * 2, max_npoints * 2, by=npoints_step * 2))
{
    size_of_data <- c(i,i,i)
    ear_distance <- 2
    data <- mouseGaussGenerator(size_of_data, ear_distance)$data
    filename = paste(c(data_path, i), collapse="")
    write.table(data, file=filename, row.names=FALSE, col.names=FALSE)    
}
