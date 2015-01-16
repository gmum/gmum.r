library("gmum.r")

data(wine, package="rattle")
scaled.wine <- scale(wine[-1])

# Train in an offline manner
gng <- GNG(scaled.wine, labels=wine$Type, max.nodes=20, 
           training=gng.train.offline(max.iter=10000, min.improvement=1e-1))



environment(DollarNamesGmumr)




setMethod( ".DollarNames", "C++Object",
           function( x, pattern ){
             envir = asNamespace("Rcpp")
             DollarNames.Rcpp = envir$`.DollarNames.C++Object`
             DollarNames.Rcpp(x, pattern)[! (substr(.DollarNames.Rcpp(x, pattern),1,1)==".")]
           } , where=.GlobalEnv)


library(gmum.r)
setMethod( ".DollarNames", "C++Object", function( x, pattern ){
  grep(pattern, asNamespace("Rcpp")$complete(x), value = TRUE)[! (substr(grep(pattern, asNamespace("Rcpp")$complete(x), value = TRUE),1,1)==".")]
} )


`.DollarNames.C++Object` <- function( x, pattern ){
  grep(pattern, asNamespace("Rcpp")$complete(x), value = TRUE)[! (substr(grep(pattern, asNamespace("Rcpp")$complete(x), value = TRUE),1,1)==".")]
}

gng$

.DollarNames(gng, ".export")
g
m(gng, "^g")

environment(m) = asNamespace("Rcpp")

asNamespace("Rcpp")$`.DollarNames.C++Object`

library(Rcpp)
asNamespace("Rcpp")$complete


asNamespace("Rcpp")$`.DollarNames.C++Object`

h <- function(x){
  print(complete)
}
environment(h) <- asNamespace("Rcpp")
h("1")

g = DollarNamesGmumr

DollarNamesGmumr

f <- function(x){
  
}

.GlobalEnv$g = function(x){
  
}

environment(g)
environment(f)
environment(DollarNamesGmumr)

asNamespace("gmum.r")$`.DollarNames.C++Object`

asNamespace("gmum.r")$`.DollarNames.C++Object`


asNamespace("gmum.r")$.DollarNames.Rcpp

# Print number of nodes
numberNodes(gng)

# Print mean degree of the network
ig = convertToGraph(gng)
mean(degree(ig))

# Plot using igraph layout
plot(gng, mode = gng.plot.2d, 
     vertex.color=gng.plot.color.label, layout=igraph::layout.fruchterman.reingold)

# Print summary of trained object
print(summary(gng))

# Print prediction accuracy
labels = as.vector(wine[,c("Type")], mode="double") 
preds <- c()
for(i in 1:nrow(scaled.wine)){
  preds <- c(preds,round(node(gng,  round(predict(gng, as.vector(scaled.wine[i,], mode="double")))+1)$label))
}
print(table(preds, labels))