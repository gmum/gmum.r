library(testthat)
library('gmum.r')

test_that("spherical works", {
  data(cec_t_set)
  c <- CEC(x = Tset, k = 10, method.type = 'sphere', control.nstart = 100, control.eps = 0.07)
  plot(c, ellipses = TRUE, centers = TRUE)
  
  expected_energy = -0.9294208
  
  expect_that(all.equal(current = c$energy(), target = expected_energy, tolerance = 0.2), is_true())
  print("test_t_set: spherical works")
})

test_that("fsphere works", {
  data(cec_t_set)
  c <- CEC(x = Tset, k = 10, method.type = 'fsphere', params.r = 0.01, control.nstart = 100, control.eps = 0.07)
  plot(c, ellipses = TRUE, centers = TRUE)
  
  expected_energy = -0.6100412
  
  expect_that(all.equal(current = c$energy(), target = expected_energy, tolerance = 0.2), is_true())
  print("test_t_set: fsphere works")
})

test_that("diagonal works", {
  data(cec_t_set)
  c <- CEC(x = Tset, k = 10, method.type = 'diagonal', control.nstart = 100, control.eps = 0.1)
  plot(c, ellipses = TRUE, centers = TRUE)
  
  expected_energy = -0.9852
  
  expect_that(all.equal(current = c$energy(), target = expected_energy, tolerance = 0.2), is_true())
  print("test_t_set: diagonal works")
})

test_that("covariance works", {
  data(cec_t_set)
  c <- CEC(x = Tset, k = 10, method.type = 'full', params.cov=matrix(c(0.04, 0, 0, 0.01), 2), control.nstart = 100, control.eps = 0.07)
  plot(c, ellipses = TRUE, centers = TRUE)
  
  expected_energy = -0.2362521
  
  expect_that(all.equal(current = c$energy(), target = expected_energy, tolerance = 0.2), is_true())
  print("test_t_set: covariance works")
})

#test_that("mix works", {
#  data(cec_t_set)
#  fsphere_cluster_param = list(method.type = 'fsphere', params.r = 0.001)
#  full_cluster_param = list(method.type = 'full', params.cov=matrix(c(0.05, 0, 0, 0.001), 2))
#  c <- CEC(x = Tset, k = 5, params.mix = list(full_cluster_param, fsphere_cluster_param, fsphere_cluster_param, fsphere_cluster_param, fsphere_cluster_param), control.nstart = 10)
#  plot(c, ellipses = TRUE, centers = TRUE)
#  
#  expected_energy = 0.6424162
#  
#  expect_that(all.equal(current = c$energy(), target = expected_energy, tolerance = 0.2), is_true())
#  print("test_t_set: mix works")
#})
