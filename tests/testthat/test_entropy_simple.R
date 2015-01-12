library(testthat)
library('gmum.r')

tmpEnv <- new.env()
load(system.file("data_sets", "cec", "simple_1", "energy.RData", package="gmum.r"), envir=tmpEnv)
expected_energy <- tmpEnv$energy

load(system.file("data_sets", "cec", "simple_1", "cluster.RData", package="gmum.r"), envir=tmpEnv)
dataset_clusters <- tmpEnv$cluster[,1]

load(system.file("data_sets", "cec", "simple_1", "input.RData", package="gmum.r"), envir=tmpEnv)
dataset_points <- tmpEnv$input

test_that("Entropy is correct", {
  c <- CEC(k=1, x=dataset_points)
  actual_energy <- c$entropy()
  
  expect_that(actual_energy, equals(expected_energy, tolerance = 1e-4))
  print("test_entropy_simple: Entropy is correct")
})