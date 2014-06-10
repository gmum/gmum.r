loadModule("cec", TRUE)
summary.cec<- NULL
plot.cec <- NULL
show.cec <- NULL
evalqOnLoad({

    print.cec <<- function(x) {
        print(sprintf("Cec clustring, %d clusters with %f entropy",
              length(x$centers()), x$entropy()))
        print("Centers : ")
        print(x$centers())
        print("Covariances : ")
        print(x$cov())
    }
    summary.cec<<- function(object) {
        print.cec(object)
        names = c("x","k","clustering","method.type","method.init","params.r","params.cov","control.nstart","control.eps","control.itmax","log.energy","log.ncluster","log.iters")
        
        Length = c(length(object$clustering()),length(object$centers()),length(object$clustering()),-1,-1,-1,-1,-1,-1,-1,length(object$log.energy()),length(object$log.ncluster()),length(object$log.iters()))
        print(data.frame(names,Length), row.names=FALSE)
    }

    plot.cec <<- function(x,slice = c(), ellipses = FALSE, centers = FALSE) {

        d = x$x()
        if (length(slice)==0){
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
                library("car")
                cov = x$cov()

                for (i in 1:n) {
                    print(t(length(cen[i])))
                    print(class(cen[i]))
                    ellipse(unlist(cen[i][slice]),matrix(unlist(cov[i]),ncol=2)[slice,slice],1.0)
            }
        }

        if(centers) {
            mcenters = do.call(rbind,cen)
            points(mcenters[,slice],col="blue",bg=par("bg"))
         }
        }
    }
    


setMethod("summary", "Rcpp_cec", summary.cec)
setMethod("print","Rcpp_cec", print.cec)
setMethod("plot","Rcpp_cec",plot.cec)

})
