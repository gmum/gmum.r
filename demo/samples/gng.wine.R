#' ---
#' title: "GNG igraph integration on wine dataset"
#' author: ""
#' date: ""
#' output:
#'  html_document:
#'    self_contained: false
#' ---
#' 
library(gmum.r)
library(caret) # For ConfusionMatrix

wine <- get.wine.dataset.X(scale=TRUE)

# Train in an offline manner
gng <- GNG(wine, labels=get.wine.dataset.y(), max.nodes=20, 
           max.iter=10000, min.improvement=1e-1)

# Print number of nodes
numberNodes(gng)

# Convert to igraph directly!
ig = convertToIGraph(gng)

# Print mean degree of the network
mean(degree(ig))

# You can access different attributes of the nodes
V(ig)$error[1]

# Plot using igraph layout
plot(gng, vertex.color="label", layout=igraph::layout.fruchterman.reingold, 
     vertex.size=9)

# Print summary of trained object
summary(gng)

# You can use graph to predict new samples 
# (in a closest neighbour way)
preds <- c()
for(i in 1:nrow(wine)){
  preds <- c(preds,round(node(gng, predict(gng, wine[i,]))$label))
}

# Print prediction statistics
confusionMatrix(table(preds, get.wine.dataset.y()))
