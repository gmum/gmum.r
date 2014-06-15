dataset <- as.matrix(read.table("test/data/mouse_1_spherical/input.txt"))

# that is the dataset we want to cluster
plot(dataset)

# run cec with default parameters
args <- list(k=3, x=dataset)
c <- new(cec, args)
plot(c)

# since initial clusterization is random
# it may be a good idea to run cec multiple times
# and choose the best result
args <- list(k=3, x=dataset, params.nstart=10)
c <- new(cec, args)
plot(c)

# better than before, however, we know that
# clusters are spherical; let's inform cec about that

args <- list(k=3, x=dataset, params.nstart=10, method.type='sphere')
c <- new(cec, args)
plot(c)
# ain't that great? :D
