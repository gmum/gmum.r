library(testthat)
library('gmum.r')

data(cec_simple_1)

expected_energy <- energy_value
dataset_points <- input

test_that("Entropy is correct", {  
  f_standard <- function(m, sigma){
    return (m * log(2 * pi * exp(1)) / 2 + log(det(sigma)) / 2)
  }
  
  c1 <- CEC(k=1, x=dataset_points, method.type='func', params.function='f_standard')
  c2 <- CEC(k=1, x=dataset_points, method.type='standard')
	
  actual_energy_func_type <- c1$.entropy()
  actual_energy_standart_type <- c2$.entropy()
  
  expect_that(actual_energy_func_type, equals(actual_energy_standart_type, tolerance = 1e-4))
  expect_that(actual_energy_func_type, equals(expected_energy, tolerance = 1e-4))
  print("test_func_method_type: Entropy is correct")
})
