
energyOneCluster <- function( kind, probab, covariances,dimOfData){ # Move this to separate function parameter passed 
  energy <- switch(kind,
  { # 1
    energy = probab*( -log(probab)+0.5*log(det(covariances) ) +dimOfData*0.5*log(2*pi*exp(1)))
  },      
  { # 2
    energy = 1
  })
  return(energy)
}

CECEnergy<-function(dataSet,label,kind){
  k<-length(unique(label))
  dim<-ncol(dataSet)  
    means <- matrix(0,k,dim)
    probab<- matrix(0,k,1)
    energyArray<- matrix(0,k,1)
    covariances <- array(c(1:k*dim*dim),c(k,dim,dim))
    energy<-0
    for (i in 1:k){
      means[i,] <- apply(dataSet[label == i,],2,mean)
      covariances[i,,] <- cov(dataSet[label == i,])
      probab[i] <- length(dataSet[label == i,1])/length(dataSet[,1])
      energyArray[i]  <- energyOneCluster(kind=1, probab=probab[i], covariances=covariances[i,,],dim)
      energy <- energy + energyOneCluster(kind=1, probab=probab[i], covariances=covariances[i,,],dim)
    }
    list(means=means,covariances=covariances,energyArray=energyArray,energy=energy)
}

dataSet <- matrix(as.numeric(as.matrix(read.table("C:\\Users\\admin\\Google Dysk_new\\CEC_curve\\R_symulacje\\EllipseGauss.txt"),skipNul=TRUE)),ncol=2);
label <- as.matrix(read.table("C:\\Users\\admin\\Google Dysk_new\\CEC_curve\\R_symulacje\\EllipseGauss_cluster_start.txt"))+1;

CECEnergy(dataSet,label,1)
  
  
  
  #plot(dataSet,pch=20,col=label)




means
covariances
probab

energy

energyArray




