#' ---
#' title: "GNG igraph integration on wine dataset"
#' author: ""
#' date: ""
#' ---
#' 
library(gmum.r)
library(caret) # For ConfusionMatrix
library(rattle) # For dataset

# Prepare data
scaled.wine <- as.matrix(scale(wine[-1]))

# Train in an offline manner
gng <- GNG(scaled.wine, labels=as.integer(wine$Type), max.nodes=20, 
           max.iter=10000, min.improvement=1e-1)

# Print number of nodes
numberNodes(gng)

# Convert to igraph directly!
ig = convertToGraph(gng)

# Print mean degree of the network
mean(degree(ig))

# You can access different attributes of the nodes
V(ig)$error[1]

# Plot using igraph layout
plot(gng, mode = gng.plot.2d, 
     vertex.color=gng.plot.color.label, layout=igraph::layout.fruchterman.reingold, 
     vertex.size=9)

# Print summary of trained object
summary(gng)

# You can use graph to predict new samples 
# (in a closest neighbour way)
preds <- c()
for(i in 1:nrow(scaled.wine)){
  preds <- c(preds,round(node(gng, predict(gng, scaled.wine[i,]))$label))
}

# Print prediction statistics
confusionMatrix(table(preds, wine$Type))
