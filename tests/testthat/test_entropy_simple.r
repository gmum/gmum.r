library(testthat)
library('gmum.r')

dataset_energy <- as.matrix(read.table("../cpp/data/simple_1/energy.txt"))
dataset_points <- as.matrix(read.table("../cpp/data/simple_1/input.txt"))

args = list(k=1, x=dataset_points)
c <- CEC(args)
actual_energy <- c$entropy()
expected_energy <- unname(dataset_energy[1,1])

expect_that(actual_energy, equals(expected_energy, tolerance = 1e-4))