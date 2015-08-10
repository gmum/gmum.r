library(testthat)
library('gmum.r')

test_that("Entropy is correct", {  
  data(cec_simple_1)
  
  expected_energy <- cec.simple1.extra[["energy"]]
  dataset_points <- cec.simple1
  
  c <- CEC(k=1, x=dataset_points, seed=13371337)
  actual_energy <- c$energy()
  
  expect_that(actual_energy, equals(expected_energy, tolerance = 1e-4))
  print("test_entropy_simple: Entropy is correct")
})
