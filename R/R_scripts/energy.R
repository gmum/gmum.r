
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
      covariances[i,,] <- ((length(dataSet[,1])-1)/length(dataSet[,1]))*cov(dataSet[label == i,])
      probab[i] <- length(dataSet[label == i,1])/length(dataSet[,1])
      energyArray[i]  <- energyOneCluster(kind=1, probab=probab[i], covariances=covariances[i,,],dim)
      energy <- energy + energyOneCluster(kind=1, probab=probab[i], covariances=covariances[i,,],dim)
    }
    list(means=means,covariances=covariances,energyArray=energyArray,energy=energy)
}

#dataSet <- matrix(as.numeric(as.matrix(read.table("C:\\Users\\admin\\Dropbox\\CEC_plugin_R\\TESTY\\simple_1\\input.txt"),skipNul=TRUE)),ncol=2);
#label <- as.matrix(read.table("C:\\Users\\admin\\Dropbox\\CEC_plugin_R\\TESTY\\simple_1\\cluster.txt"));

dataSet <- matrix(as.numeric(as.matrix(read.table("C:\\Users\\admin\\Dropbox\\CEC_plugin_R\\TESTY\\mouse_1\\input.txt"),skipNul=TRUE)),ncol=2);
label <- as.matrix(read.table("C:\\Users\\admin\\Dropbox\\CEC_plugin_R\\TESTY\\mouse_1\\cluster.txt"));


CECEnergy(dataSet,label,1)
  
  
#file.path("gmum.r\\test\\data\\simple_1\\input.txt", fsep = .Platform$file.sep)
#plot(dataSet,pch=20,col=label)




