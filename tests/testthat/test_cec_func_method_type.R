library(testthat)
library('gmum.r')

source("cec_function.R")

data(cec.simple1)

expected_energy <- cec.simple1.extra[["energy"]]
dataset_points <- cec.simple1

test_that("Entropy is correct", {  
  c1 <- CEC(k=1, x=dataset_points, method.type='func', params.function=f_standard, seed=13371337)
  c2 <- CEC(k=1, x=dataset_points, method.type='standard', seed=13371337)
	
  plot(c1)
  plot(c2)
  
  actual_energy_func_type <- c1$energy()
  actual_energy_standart_type <- c2$energy()
  
  expect_that(actual_energy_func_type, equals(actual_energy_standart_type, tolerance = 1e-4))
  expect_that(actual_energy_func_type, equals(expected_energy, tolerance = 1e-4))
  print("test_func_method_type: Entropy is correct")
})
