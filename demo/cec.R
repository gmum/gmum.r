dataset <- as.matrix(read.table("inst/data_sets/cec/mouse_1_spherical/input.txt"))

# that is the dataset we want to cluster
plot(dataset)
# run cec with default parameters
c <- CEC(k=3, x=dataset)
plot(c)

# since initial clusterization is random
# it may be a good idea to run cec multiple times
# and choose the best result
c <- CEC(k=3, x=dataset, control.nstart=10)
plot(c)

# better than before, however, we know that
# clusters are spherical; let's inform cec about that
c <- CEC(k=3, x=dataset, control.nstart=10, method.type='sphere')
plot(c)
# ain't that great? :D

# you can learn details of clustering like this
centers(c)
cov(c)

# you can predict cluster which a point would belong to
predictCluster(c, c(1,1))

# or find out what are the probabilities of belonging
# to each cluster
predictClusters(c, c(1,1))

# you can visualise size and shape of clusters
plot(c, ellipses=TRUE)

# try the same with random assignment
c <- CEC(k=3, x=dataset, control.nstart=10, method.type='sphere', method.init='random')
plot(c)
