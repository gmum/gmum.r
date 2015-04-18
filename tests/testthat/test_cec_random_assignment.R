library(testthat)
library('gmum.r')

source('combinations.R')

test_that("correctness works", {
  data(cec_ellipse_gauss)
  
  dataset_clusters <- cec_ellipse_gauss.cluster[,1]
  dataset_points <- cec_ellipse_gauss.input
  
  nclusters <- 4
  npoints <- dim(dataset_points)[1]
  
  correct_percentage <- correctness(dataset_clusters, dataset_clusters, npoints, nclusters)
  expect_that(correct_percentage, equals(1))
  print("test_random_assignment: correctness works")
})

test_that("EllipseGauss random assignment is correct", {
  data(cec_ellipse_gauss)
  
  expected_energy <- cec_ellipse_gauss.energy
  dataset_clusters <- cec_ellipse_gauss.cluster[,1]
  dataset_points <- cec_ellipse_gauss.input
  
  dataset_clusters <- dataset_clusters - min(dataset_clusters)
  
  t <- 20
  accepted <- 0
  nclusters <- 4
  npoints = dim(dataset_points)[1]
  for(i in 1:t)
  {
    c <- CEC(k=nclusters, x=dataset_points, method.init='random')
    correct_percentage <- correctness(dataset_clusters, c$clustering(), npoints, nclusters)
    if(isTRUE(all.equal(current = c$energy(), target=expected_energy, tolerance=0.25)) | (correct_percentage >= 0.9) ) {
      accepted <- accepted + 1
    }
  }
  expect_that(accepted > t/2.0, is_true())
  print("test_random_assignment: EllipseGauss random assignment is correct")
})

test_that("mouse_1 random assignment is correct", {
  data(cec_mouse_1)
  
  expected_energy <- cec_mouse_1.energy
  dataset_clusters <- cec_mouse_1.cluster[,1]
  dataset_points <- cec_mouse_1.input
  
  dataset_clusters <- dataset_clusters - min(dataset_clusters)
  
  t <- 20
  accepted <- 0
  nclusters <- 3
  npoints <- dim(dataset_points)[1]
  for(i in 1:t)
  {
    c <-CEC(k=nclusters, x=dataset_points, method.init='random')
    plot(c)
    correct_percentage <- correctness(dataset_clusters, c$clustering(), npoints, nclusters)
    if(isTRUE(all.equal(current = c$energy(), target=expected_energy, tolerance=0.2)) | (correct_percentage >= 0.9) ) {
      accepted <- accepted + 1
    }
  }
  expect_that(accepted > t/2.0, is_true())
  print("test_random_assignment: mouse_1 random assignment is correct")
})

test_that("mouse_1_spherical random assignment is correct", {
  data(cec_mouse_1_spherical)
  
  expected_energy <- cec_mouse_1_spherical.energy
  dataset_clusters <- cec_mouse_1_spherical.cluster[,1]
  dataset_points <- cec_mouse_1_spherical.input 
  
  dataset_clusters <- dataset_clusters - min(dataset_clusters)
  
  t <- 20
  accepted <- 0
  nclusters <- 3
  npoints <- dim(dataset_points)[1]
  for(i in 1:t)
  {
    c <- CEC(k=nclusters, x=dataset_points, method.type='sphere', method.init='random')
    plot(c)
    correct_percentage <- correctness(dataset_clusters, c$clustering(), npoints, nclusters)
    if(isTRUE(all.equal(current = c$energy(), target=expected_energy, tolerance=0.2))  | (correct_percentage >= 0.9) ) {
      accepted <- accepted + 1
    }
  }
  expect_that(accepted > t/2.0, is_true())
  print("test_random_assignment: mouse_1_spherical random assignment is correct")
})
