library(testthat)
library('gmum.r')

expected_energy <- as.numeric(read.table("../cpp/data/simple_1/energy.txt"))
dataset_clusters <- as.vector(read.table("../cpp/data/simple_1/cluster.txt")[,1])
dataset_points <- as.matrix(read.table("../cpp/data/simple_1/input.txt"))

test_that("Entropy is correct", {
	args = list(k=1, x=dataset_points)
	c <- CEC(args)
	actual_energy <- c$entropy()

    expect_that(actual_energy, equals(expected_energy, tolerance = 1e-4))
})

test_that("Entropy is correct with initial centroids", {
    centroids = dataset_clusters - min(dataset_clusters)
    args = list(k=1, x=dataset_points, params.init='centroids', params.centroids = centroids)
    c <- CEC(args)
   	actual_energy <- c$entropy()
	
	expect_that(actual_energy, equals(expected_energy, tolerance = 1e-4))
})
