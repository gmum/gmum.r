#' ---
#' title: "Online training in GNG"
#' author: ""
#' date: ""
#' output:
#'  html_document:
#'    self_contained: false
#' ---
library(gmum.r)

# If you decide to use this advanced feature, pass dimensionality of the data
gng <- GNG(max.nodes=100, train.online=TRUE, dim=3, verbosity=3)

# Construct exemplary spherical dataset with labels
ex <- gng.preset.sphere(N=10000)
labels <- round(runif(10000)*3)

# Insert dataset with labels
insertExamples(gng, ex, labels)


# Run algorithm in parallel 
# (GNG will be running in separate thread!)
run(gng)

# Wait for it to converge
Sys.sleep(5.0)

# It is easy to plot dataset with error curve!
plot(gng, mode=gng.plot.2d.errors, vertex.size=6)

# We can add another dataset
box.dataset <- gng.preset.box(N=10000, r=1.0, center=c(1.0,1.0,1.0))
insertExamples(gng, box.dataset)

# Wait for it to converge
Sys.sleep(5.0)

# Plot again.
plot(gng, mode=gng.plot.2d.errors, vertex.size=6)

# Terminate algorithm
terminate(gng)