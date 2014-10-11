loadModule("cec", TRUE)

#' plot()
#' @title plot()
#' plot clustering found
#'
#' @docType methods
#'
#' @param x CEC model object
#' @param slice list of dimentions chosen for display since plot is 2d
#' @param ellipses outline clusters
#' @param centers mark center of every cluster
plot.cec <- NULL

evalqOnLoad({

    plot.cec <- function(x,slice = c(), ellipses = FALSE, centers = FALSE) {

        d = x$x()
        if (length(slice)==0) {
            slice = c(1:(dim(d)[2]))
        }
        if (length(slice)==2) {
            plot(d[,slice], col = (x$y() + 1))
        }
        else{
            pairs(d[,slice],col = (x$y()+1))
        }

        if (ellipses || centers) {
            cen = x$centers()
            n = length(cen)
            if (ellipses && length(slice) <= 2){
                                        #library("car")
                cov = x$cov()

                for (i in 1:n) {
                    data = unlist(cov[i])
                    covMat = matrix(data,ncol=sqrt(length(data)))[slice,slice]
                    m = unlist(cen[i][slice])
                    print(covMat)
                    eigenValuesAndVectors = eigen(covMat)
                    veE <- eigenValuesAndVectors$vectors
                    l <- eigenValuesAndVectors$values
                    r <- seq(-pi, pi, by = 0.001)
                    len <- length(r)
                    Xa <- 2*sqrt(l[1])*cos(r)
                    Ya <- 2*sqrt(l[2])*sin(r)
                    mm <- c(rep(m[1], len),rep(m[2],len))
                    meansMultiply <- matrix(mm, ncol = 2)
                    line1 = cbind(Xa,Ya)
                    lineAll = rbind(line1)
                    ddd <- (lineAll%*%t(veE)) + meansMultiply
                    points(ddd,col = "black", type = "l", lwd = 2)
                                        #dataEllipse(d[x$y() == (i-1),],plot.points=FALSE,add = TRUE, levels = c(0.9))
                }

                
            }

            if(centers) {
                mcenters = do.call(rbind,cen)
                points(mcenters[,slice],col="blue",bg=par("bg"))
            }
        }
    }
    
    setMethod("plot", signature(x="Rcpp_cec"), plot.cec)
})
