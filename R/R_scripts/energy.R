
tr <- function(mat){

    return(sum(diag(mat)))
}
energyOneCluster <- function( kind, probab, covariances,dimOfData){ # Move this to separate function parameter passed 
  energy <- switch(kind,
  { # 1
     
    energy = probab*( -log(probab)+0.5*log(det(covariances) ) +dimOfData*0.5*log(2*pi*exp(1)))
  },      
  { # 2
    N = dimOfData
    print("N")
    print(N)
    print("tr(covariances)")
    print(tr(covariances))
    print("log(tr(covariances) )")
    print(log(tr(covariances) ))
    p = probab
    print("p")
    print(p)
    energy = probab*( -log(probab)+0.5*dimOfData*log(tr(covariances) ) +dimOfData*0.5*log(2*pi*exp(1)/dimOfData))
  })
    print("energy")
    print(energy)
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
      energyArray[i]  <- energyOneCluster(kind=2, probab=probab[i], covariances=covariances[i,,],dim)
      energy <- energy + energyOneCluster(kind=2, probab=probab[i], covariances=covariances[i,,],dim)
    }
    list(means=means,covariances=covariances,energyArray=energyArray,energy=energy)
}

dataPath = file.path("..","..","test","data")
simplePath = file.path(dataPath,"simple_1")
mousePath = file.path(dataPath,"mouse_1")
mousePathS = file.path(dataPath,"mouse_1_spherical")
ellipsePath = file.path(dataPath,"EllipseGauss")
rpath = mousePathS
dataSetPath = file.path(rpath,"input.txt")
labelPathPrzemek = file.path(rpath,"cluster.txt")
labelPathMy = file.path(rpath,"our_clusters.txt")



dataSet <- matrix(as.numeric(as.matrix(read.table(dataSetPath),skipNul=TRUE)),ncol=2);

labelPrzemek <- as.matrix(read.table(labelPathPrzemek));
print(min(labelPrzemek))
labelPrzemek<- labelPrzemek + (1 - min(labelPrzemek))
labelMy <- as.matrix(read.table(labelPathMy));
print (min(labelMy))
labelMy<- labelMy + (1 - min(labelMy))


print(CECEnergy(dataSet,labelPrzemek,1))
print(CECEnergy(dataSet,labelMy,1))
  
  
#file.path("gmum.r\\test\\data\\simple_1\\input.txt", fsep = .Platform$file.sep)
#plot(dataSet,pch=20,col=label)




