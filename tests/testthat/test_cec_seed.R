library(testthat)
library('gmum.r')

source('combinations.R')

test_that("Seed works", {
  data(cec_ellipse_gauss)
  dataset_points <- cec.ellipsegauss

  c1 <- CEC(k=4, x=dataset_points, method.init='random', seed=1)
  c2 <- CEC(k=4, x=dataset_points, method.init='random', seed=2)
  expect_that(all(c1$clustering() == c2$clustering()), is_false())
  expect_that(c1$energy() == c2$energy(), is_false())

  c1 <- CEC(k=4, x=dataset_points, method.init='random', seed=1)
  c2 <- CEC(k=4, x=dataset_points, method.init='random', seed=1)
  expect_that(all(c1$clustering() == c2$clustering()), is_true())
  expect_that(c1$energy() == c2$energy(), is_true())

  print("test_cec_seed: seed works")
})
