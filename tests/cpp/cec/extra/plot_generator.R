source('common.R')
x = seq(min_npoints, max_npoints, by=npoints_step)

for(i in 1:method_type_num)
{
    method_type_name = gmum_cran_method_type_map[i]

    gmum_result_path = get_gmum_result_file_path(i)
    cran_result_path = get_cran_result_file_path(i)
    gmum_cec_result <- as.matrix(read.table(gmum_result_path))
    cran_cec_result <- as.matrix(read.table(cran_result_path))
    
    gmum_time = gmum_cec_result[,1]
    gmum_iters = gmum_cec_result[,2]
    cran_time = cran_cec_result[,1]
    cran_iters = cran_cec_result[,2]

    min_x = min(x)
    max_x = max(x)

    min_gmum_time = min(gmum_time)
    min_gmum_iters = min(gmum_iters)
    max_gmum_time = max(gmum_time)
    max_gmum_iters = max(gmum_iters)

    min_cran_time = min(cran_time)
    min_cran_iters = min(cran_iters)
    max_cran_time = max(cran_time)
    max_cran_iters = max(cran_iters)

    jpeg(paste(c(method_type_name,'_times_plot.jpg'), collapse=''))
    plot( c(min_x, max_x), c(min(min_gmum_time, min_cran_time), max(max_gmum_time, max_cran_time)), type="n", xlab="Number of points", ylab="Seconds" )
    title(main=paste(c(method_type_name,'time plot'), collapse=' '))
    lines(x, gmum_time, col="green", lwd=2.5)
    lines(x, cran_time, col="red", lwd=2.5)
    par(xpd=TRUE)
    legend("bottom",legend = c("GMUM", "CRAN"), text.width = max(sapply(text, strwidth)),
           col=c("green", "red"), lwd=5, cex=1, horiz = TRUE)
    par(xpd=FALSE)
    dev.off()

    jpeg(paste(c(method_type_name,'_iters_plot.jpg'), collapse=''))
    plot( c(min_x, max_x), c(min(min_gmum_iters, min_cran_iters), max(max_gmum_iters, max_cran_iters)), type="n", xlab="Number of points", ylab="Iterations" )
    title(main=paste(c(method_type_name,'iterations plot'), collapse=' '))
    lines(x, gmum_iters, col="green", lwd=2.5)
    lines(x, cran_iters, col="red", lwd=2.5)
    par(xpd=TRUE)
    legend("bottom",legend = c("GMUM", "CRAN"), text.width = max(sapply(text, strwidth)),
           col=c("green", "red"), lwd=5, cex=1, horiz = TRUE)
    par(xpd=FALSE)
    dev.off()
}


