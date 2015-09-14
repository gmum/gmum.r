#' ---
#' title: "Optimized version of GNG"
#' author: ""
#' date: ""
#' output:
#'  html_document:
#'    self_contained: false
#' ---

# For advanced used you might want to consider using OptimizedGNG
# , which asymptotically much faster implementation without
# sacrifying quality of the graph

# GNG unique capability is online training

library(gmum.r)

# Construct 10^6 points spherical dataset. Might take a while!
sphere.dataset <- gng.preset.sphere(N=10000)

# If you decide to use this advanced feature, you have to pass a
# range (bounding box) in which all of passed data feature values will reside
gng <- OptimizedGNG(sphere.dataset, max.nodes=1000, max.iter=10000, dim=3, 
                    value.range=c(0,1))

# GNG would much longer, you can check it yourself
# gng <- GNG(sphere.dataset, max.nodes=1000, max.iter=10000, dim=3)

predictCentroid(gng, centroids(gng), sphere.dataset)

# Plot results using spatial coordinates
plot(gng, layout=gng.plot.layout.v2d, vertex.size=6)
