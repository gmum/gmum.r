library("multicore")
GNGVisualiseAnimation<-function(sv,step,count){

library("rgl")


iteration<-0


rgl.light()
rgl.clear("all")

counter<-0
while(counter<count){
count=count+1
print(3)
Sys.sleep(step)
print(sv$getNumberNodesOnline())
if(sv$getNumberNodesOnline()!=0){

iteration<-iteration+1
#print(cat("iteration nr ",iteration,sep=""))
tmp<-sv$updateBuffer()

print(cat(":: update of buffer successful, iteration nr ",iteration,sep=""))

if(iteration>1){
rm(x)
rm(y)
rm(z)
rm(x_lines)
rm(y_lines)
rm(z_lines)
rm(nodes)
}
#not that good........
nodes <- sv$getBufferSize()

x_lines <- c(0,0)
y_lines <- c(0,0)

z_lines <- c(0,0)
k<-3
m<-1

x<-c(1:nodes)
y<-c(1:nodes)
z<-c(1:nodes)

for(i in 1:(nodes))
{
	node = sv$getNode(i-1) #from node matrix?
	if(i==1) print(node)
	x[i]=node[2]
	y[i]=node[3]
	z[i]=node[4]

	#print(node)
	#print(cat(length(node)-3," edges"))
	if( length(node)>4){
	
	

 	for(j in 1:(length(node)-4) ){
		
		x_lines[k] = x[i]
		y_lines[k] = y[i]
		z_lines[k] = z[i]
		#print(cat("accessing", node[j+3]+1, " at node ",i,sep=""))
		#print(dim(nodes_matrix))
		
		#idea - find edges later when have all x,y,z read in - better?

		connected_node = (sv$getNode(node[j+4]))

		x_lines[k+1]=connected_node[2]
		y_lines[k+1]=connected_node[3]
		z_lines[k+1]=connected_node[4]
		
		#print(connected_node)
		

		dist = sqrt((x[i] - connected_node[2])^2 + (y[i] - connected_node[3])^2 + (z[i] - connected_node[4])^2)
		#print(dist)

		 k=k+2
		
	 }	

	

	}
	
}
print("::reading was succesful")
print(cat(nodes," of nodes",cat=""))<
print(sv$getAccumulatedError())

zscale <- 1


# clear scene:
#clear3d("all")
 
# setup env:
  clear3d(type="all")
  rgl.bg(color="white")

 
cx <- c(1:(nodes-1))
cy <- c(1:(nodes-1))
cz <- c(1:(nodes-1))

cx <- abs(x)/max(abs(x))
cy <- abs(y)/max(abs(y))
cz <- abs(z)/max(abs(z))

errortext<-cat("Error = ",sv$getAccumulatedError(),cat="")

axes3d(edges="bbox")

rgl.lines(x_lines,y_lines,z_lines,color=rgb(cx,cy,cz))


}
}

}


GNGVisualiseCool<-function(sv,error_scale=1){
sv$pauseServer()
library("rgl")


iteration<-0

clear3d(type="all")
rgl.light()


print(sv$getNumberNodesOnline())
if(sv$getNumberNodesOnline()!=0){

iteration<-iteration+1
#print(cat("iteration nr ",iteration,sep=""))
tmp<-sv$updateBuffer()

if(iteration>1){
rm(x)
rm(y)
rm(z)
rm(x_lines)
rm(y_lines)
rm(z_lines)
rm(nodes)
}
#not that good........
nodes <- sv$getBufferSize()

x_lines <- c(0,0)
y_lines <- c(0,0)

z_lines <- c(0,0)
k<-3
m<-1

x<-c(1:nodes)
y<-c(1:nodes)
z<-c(1:nodes)

errors<-c(0)
for(i in 1:(nodes))
{
	errors[i]=sv$getErrorNode(i-1)
	node = sv$getNode(i-1) #from node matrix?
	if(i==1) print(node)
	x[i]=node[2]
	y[i]=node[3]
	z[i]=node[4]

	#print(node)
	#print(cat(length(node)-3," edges"))
	if( length(node)>4){
	
	

 	for(j in 1:(length(node)-4) ){
		
		x_lines[k] = x[i]
		y_lines[k] = y[i]
		z_lines[k] = z[i]
		#print(cat("accessing", node[j+3]+1, " at node ",i,sep=""))
		#print(dim(nodes_matrix))
		
		#idea - find edges later when have all x,y,z read in - better?

		connected_node = (sv$getNode(node[j+4]))

		x_lines[k+1]=connected_node[2]
		y_lines[k+1]=connected_node[3]
		z_lines[k+1]=connected_node[4]
		
		#print(connected_node)
		

		dist = sqrt((x[i] - connected_node[2])^2 + (y[i] - connected_node[3])^2 + (z[i] - connected_node[4])^2)
		#print(dist)

		 k=k+2
		
	 }	

	

	}
	
}
print("::reading was succesful")

zscale <- 1

  clear3d(type="all")
  rgl.light()
  rgl.bg(color="bisque")

 
cx <- c(1:(nodes-1))
cy <- c(1:(nodes-1))
cz <- c(1:(nodes-1))

print(max(abs(errors)))

cx <- abs(errors)/max(abs(errors)) #abs(x)/max(abs(x))
cy[1:nodes-1] <- 0.1 #abs(y)/max(abs(y)
cz[1:nodes-1] <- 0.1 #abs(z)/max(abs(z))
axes3d(edges="bbox")
rgl.spheres(x,y,z,radius=cx*error_scale, color=rgb(cx,cy,cz))
rgl.lines(x_lines,y_lines,z_lines,color="bisque4")
sv$runServer()


}

}











LearningCurve<-function(client, iterations){
	iteration<-0
	errors<-c(0)
	while(1){
		iteration<-iteration+1
		Sys.sleep(0.5)
		errors[iteration] <- client$getAccumulatedError()
		plot(errors,type="l")
	}
	return(TRUE)
}

GNGLearningCurve<-function(client, iterations){
	parallel(LearningCurve())
}




