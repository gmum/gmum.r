library("methods")

setGenericVerif <- function(x,y){if(!isGeneric(x)){setGeneric(x,y)}else{}}

dyn.load("RcppInterface.so")


GNGClient_method <- function(name){
	paste("GNGClient",name,sep="__");
}
setClass("GNGClient",representation(pointer="externalptr", server_pid="list"))




setMethod("initialize",
"GNGClient",function(.Object, server_descriptor) {
    
	.Object@pointer <- .Call(GNGClient_method("new"), server_descriptor[["server_id"]])
    .Object@server_pid = server_descriptor[["server_pid"]]
	.Object
}
)


planePoint<-function(r,center){
	if(!hasArg(r)) r<-1.0
	if(!hasArg(center)) center<-c(0,0,0)
	
	point<-center
	point[1]<-point[1]+r*runif(1.0)
	point[2]<-point[2]+r*runif(1.0)
	point[3]<-point[3]

	return(point)
}

spherePoint<-function(r,center){
	if(!hasArg(r)) r<-1.0
	if(!hasArg(center)) center<-c(0,0,0)

	alpha<-runif(1)*2*pi
	beta<-runif(1)*pi
	
	point<-center
	point[1]<-point[1]+r*cos(alpha)*sin(beta)
	point[2]<-point[2]+r*sin(alpha)*sin(beta)
	point[3]<-point[3]+r*cos(beta)

	return(point)
}

GNGAddExamples<-function(x,preset,preset_size,...){
	.Call(GNGClient_method("addExamples"),x@pointer,...)			
	#if(hasArg(preset)){
	#	examples<-matrix(0,preset_size,3)		
	#	if(preset=="sphere"){
	#		for(i in 1:preset_size) examples[i,]=spherePoint()		
	#	}		
	#	else if(preset=="plane"){
	#		for(i in 1:preset_size) examples[i,]=planePoint()		
	#	}
	#	.Call(GNGClient_method("addExamples"),x@pointer,examples)	
	#}
	#else{
	#	.Call(GNGClient_method("addExamples"),x@pointer,...)	
	#}
}

setMethod( "$", "GNGClient", function(x, name ) { 

	#if(name=="addExamples") function(...) GNGAddExamples(x,...)
    if(name=="terminateServer"){
        function(...){
	        .Call( GNGClient_method(name) , x@pointer , ... )
            if(length(x@server_pid) == 0){
                print("WARNING!! : not provided server_pid in constructor. Leaving zombie thread")
            }else{
                # Terminate child process
                # TODO: test behaviour with 2 clients in different threads?
                # TODO: should check if he is parent of this server
                # TODO: do it the other way : server is created in the main thread
                # and it creates clients? 
                #print("Terminating GNGServer")
                #print(x@server_pid[[1]])
                # TODO: I honestly have no idea why I have to call it twice..
                # TODO: Find fork mechanism for windows AND unix !
                y = collect(x@server_pid[[1]], wait=FALSE, timeout=1)
                y = collect(x@server_pid[[1]],wait=FALSE, timeout =1)
                y = collect(x@server_pid[[1]],wait=FALSE, timeout=1)
            }
        }
    }
    else{    
	    function(...) .Call( GNGClient_method(name) , x@pointer , ... )
    }
} 
)


createGNGServer <- function(... ){
	.Call("GNGRunServer",...)
	return(0)
}

tmp<-function(...){
	.Call("GNGRunServer",...)
	rm(list=ls(all=TRUE))
}

GNGCreateServer <- function(... ){
    server_pid<-parallel(tmp(...))	
	return(list(server_pid = list(server_pid), server_id = .Call("GNGGetServerID")))
}

GNGVisualiseStepAnimation <- function(step,pause,... ){
	iteration<-1
	source("VisualisePointsSingle.r")
	return(0)
}
GNGVisualise <- function(... ){
	iteration<-1
	source("VisualisePointsSingle.r")
	return(0)
}

GNG.Utility.None = 0
GNG.Utility.BasicUtility = 1

GNGSetParams<- function(max_nodes,orig,axis,dim,uniformgrid,lazyheap,debug_level,database_type,
	max_age=200, alpha=0.95, betha=0.9995, lambda=200,
		eps_v=0.05, eps_n=0.0006, memory_bound = 500000, utility_option = GNG.Utility.None, utility_k = 1){

    if(database_type == 2){
        print("You are using DatabaseProbabilistic. Please remember to add column with probabilities of rejecting sample")
    }
    else{
        print("You are using unsupported database type, please change to DatabaseProbabilistic (2)")
    }

	if(hasArg(database_type)) .Call("GNGSet__database_type",database_type)
	if(hasArg(max_nodes)) .Call("GNGSet__max_nodes",max_nodes)
	if(hasArg(dim)) .Call("GNGSet__dim",dim)
	if(hasArg(orig) & hasArg(axis)) .Call("GNGSet__boundingbox",orig,axis)
	if(hasArg(uniformgrid)) .Call("GNGSet__uniformgrid",uniformgrid)
	if(hasArg(lazyheap)) .Call("GNGSet__lazyheap",lazyheap)
	if(hasArg(debug_level)) .Call("GNGSet__debug_level",debug_level) 
	if(hasArg(memory_bound)) .Call("GNGSet__memory_bound",memory_bound)
	
	if(hasArg(utility_option)) .Call("GNGSet__utility_option",utility_option)
	if(hasArg(utility_k)) .Call("GNGSet__utility_k", utility_k)
    .Call("GNGSet__max_age",max_age)
	.Call("GNGSet__alpha",alpha)
	.Call("GNGSet__betha",betha)
	.Call("GNGSet__lambda",lambda)	
	.Call("GNGSET__eps_v",eps_v)
	.Call("GNGSet__eps_n",eps_n)
	return(0)
}


