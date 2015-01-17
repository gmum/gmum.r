library(testthat)
library('gmum.r')

expected_energy <- as.numeric(read.table(system.file("data_sets", "cec", "simple_1","energy.txt", package="gmum.r")))
dataset_clusters <- as.vector(read.table(system.file("data_sets", "cec", "simple_1","cluster.txt", package="gmum.r"))[,1])
dataset_points <- as.matrix(read.table(system.file("data_sets", "cec", "simple_1","input.txt", package="gmum.r")))

test_that("Entropy is correct", {
	c <- CEC(k=1, x=dataset_points)
	actual_energy <- c$entropy()

    expect_that(actual_energy, equals(expected_energy, tolerance = 1e-4))
    print("test_entropy_simple: Entropy is correct")
})
