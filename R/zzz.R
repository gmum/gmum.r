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
    


setMethod("summary", "Rcpp_cec", summary.cec)
setMethod("print","Rcpp_cec", print.cec)

})
