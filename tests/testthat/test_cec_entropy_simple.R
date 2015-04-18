library(testthat)
library('gmum.r')

data(cec_simple_1)

expected_energy <- cec_simple_1.energy
dataset_clusters <- cec_simple_1.cluster[,1]
dataset_points <- cec_simple_1.input

test_that("Entropy is correct", {
  c <- CEC(k=1, x=dataset_points)
  actual_energy <- c$energy()
  
  expect_that(actual_energy, equals(expected_energy, tolerance = 1e-4))
  print("test_entropy_simple: Entropy is correct")
})
