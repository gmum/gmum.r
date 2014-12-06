library(testthat)
library('gmum.r')

expected_energy <- as.numeric(read.table("tests/cpp/data/simple_1/energy.txt"))
dataset_clusters <- as.vector(read.table("tests/cpp/data/simple_1/cluster.txt")[,1])
dataset_points <- as.matrix(read.table("tests/cpp/data/simple_1/input.txt"))

test_that("Entropy is correct", {
	c <- CEC(k=1, x=dataset_points)
	actual_energy <- c$entropy()

    expect_that(actual_energy, equals(expected_energy, tolerance = 1e-4))
})
