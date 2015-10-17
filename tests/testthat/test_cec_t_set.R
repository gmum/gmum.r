library(testthat)
library('gmum.r')

test_that("spherical works", {
  data(cec.tset)
  c <- CEC(x = Tset, k = 10, method.type = 'spherical', control.nstart = 100, control.eps = 0.07, seed=13371337)
  plot(c, ellipses = TRUE, centers = TRUE)
  
  expected_energy = -0.9294208
  
  expect_that(all.equal(current = c$energy, target = expected_energy, tolerance = 0.2), is_true())
  print("test_t_set: spherical works")
})

test_that("fixed_spherical works", {
  data(cec.tset)
  c <- CEC(x = Tset, k = 10, method.type = 'fixed_spherical', params.r = 0.01, control.nstart = 100, control.eps = 0.07, seed=13371337)
  plot(c, ellipses = TRUE, centers = TRUE)
  
  expected_energy = -0.6100412
  
  expect_that(all.equal(current = c$energy, target = expected_energy, tolerance = 0.2), is_true())
  print("test_t_set: fixed_spherical works")
})

test_that("diagonal works", {
  data(cec.tset)
  c <- CEC(x = Tset, k = 10, method.type = 'diagonal', control.nstart = 100, control.eps = 0.1, seed=13371337)
  plot(c, ellipses = TRUE, centers = TRUE)
  
  expected_energy = -0.9852
  
  expect_that(all.equal(current = c$energy, target = expected_energy, tolerance = 0.2), is_true())
  print("test_t_set: diagonal works")
})

test_that("fixed_covariance works", {
  data(cec.tset)
  expected_energy = -0.2362521
  c <- CEC(x = Tset, k = 10, method.type = 'fixed_covariance', params.cov=matrix(c(0.04, 0, 0, 0.01), 2), control.nstart = 100, control.eps = 0.07, seed=2)
  plot(c, ellipses = TRUE, centers = TRUE)
  expect_that(all.equal(current = c$energy, target = expected_energy, tolerance = 0.2), is_true())
  print("test_t_set: fixed_covariance works")
})


test_that("mix works", {
  data(cec.tset)
  fixed_spherical_cluster = list(k = 3, method.type = 'fixed_spherical', params.r = 0.001)
  fixed_covariance_cluster = list(k = 1, method.type = 'fixed_covariance', params.cov=matrix(c(0.05, 0, 0, 0.001), 2))
  c <- CEC(x = Tset, params.mix = list(fixed_covariance_cluster, fixed_spherical_cluster), control.nstart = 100, control.eps=0.09, seed=13371337)
  plot(c, ellipses = TRUE, centers = TRUE)

  expected_energy = 2.459158
  
  expect_that(all.equal(current = c$energy, target = expected_energy, tolerance = 0.2), is_true())
  print("test_t_set: mix works")
})
