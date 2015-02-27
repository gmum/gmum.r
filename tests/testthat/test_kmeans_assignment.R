library(testthat)
library('gmum.r')
source('combinations.R')

test_that("correctness works", {
  data(cec_ellipse_gauss)
  
  dataset_clusters <- cluster[,1]
  dataset_points <- input
  
  nclusters <- 4
  npoints <- dim(dataset_points)[1]
  
  correct_percentage <- correctness(dataset_clusters, dataset_clusters, npoints, nclusters)
  expect_that(correct_percentage, equals(1))
  print("test_kmeans_assignment: correctness works")
})

test_that("mouse_1_spherical kmeans coverage is correct", {
  data(cec_mouse_1_spherical)
  
  expected_energy <- energy_value
  dataset_clusters <- cluster[,1]
  dataset_points <- input
  
  dataset_clusters <- dataset_clusters - min(dataset_clusters)
  
  t <- 100
  accepted <- 0
  nclusters <- 3
  npoints = dim(dataset_points)[1]
  for(i in 1:t)
  {
    c <- CEC(k=3, x=dataset_points, control.nstart=1, method.type='sphere', method.init='kmeans++')
    correct_percentage <- correctness(dataset_clusters, c$y(), npoints, nclusters)
    if(correct_percentage == 1) {
      accepted <- accepted + 1
    }
  }
  expect_that(accepted > t/2.0, is_true())
  print("test_kmeans_assignment: mouse_1_spherical kmeans coverage is correct")
})

test_that("mouse_1_spherical kmeans energy is correct", {
    data(cec_mouse_1_spherical)
    
    expected_energy <- energy_value
    dataset_clusters <- cluster[,1]
    dataset_points <- input
    
    dataset_clusters <- dataset_clusters - min(dataset_clusters)
    
    t <- 100
    accepted <- 0
    nclusters <- 3
    npoints = dim(dataset_points)[1]
    for(i in 1:t)
    {
        c <- CEC(k=3, x=dataset_points, control.nstart=1, method.type='sphere', method.init='kmeans++')
        actual_energy <- c$energy()
        
        if(isTRUE(all.equal(current = actual_energy, target=expected_energy, tolerance=.0001))) {
            accepted <- accepted + 1
        }
    }
    expect_that(accepted > t/2.0, is_true())
    print("test_kmeans_assignment: mouse_1_spherical kmeans energy is correct")
})
