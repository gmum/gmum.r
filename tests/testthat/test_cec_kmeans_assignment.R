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
  print("test_kmeans_assignment: correctness works")
})

test_that("mouse_1_spherical kmeans coverage is correct", {
  data(cec_mouse_1_spherical)
  
  expected_energy <- cec_mouse_1_spherical.energy
  dataset_clusters <- cec_mouse_1_spherical.cluster[,1]
  dataset_points <- cec_mouse_1_spherical.input
  
  dataset_clusters <- dataset_clusters - min(dataset_clusters)
  
  t <- 100
  accepted <- 0
  nclusters <- 3
  npoints = dim(dataset_points)[1]
  for(i in 1:t)
  {
    c <- CEC(k=3, x=dataset_points, control.nstart=1, method.type='sphere', method.init='kmeans++')
    plot(c)
    correct_percentage <- correctness(dataset_clusters, c$clustering(), npoints, nclusters)
    if(correct_percentage == 1) {
      accepted <- accepted + 1
    }
  }
  expect_that(accepted > t/2.0, is_true())
  print("test_kmeans_assignment: mouse_1_spherical kmeans coverage is correct")
})

test_that("mouse_1_spherical kmeans energy is correct", {
    data(cec_mouse_1_spherical)
    
    expected_energy <- cec_mouse_1_spherical.energy
    dataset_clusters <- cec_mouse_1_spherical.cluster[,1]
    dataset_points <- cec_mouse_1_spherical.input
    
    dataset_clusters <- dataset_clusters - min(dataset_clusters)
    
    t <- 100
    accepted <- 0
    nclusters <- 3
    npoints = dim(dataset_points)[1]
    for(i in 1:t)
    {
        c <- CEC(k=3, x=dataset_points, control.nstart=1, method.type='sphere', method.init='kmeans++')
        plot(c)
        actual_energy <- c$energy()
        
        if(isTRUE(all.equal(current = actual_energy, target=expected_energy, tolerance=.0001))) {
            accepted <- accepted + 1
        }
    }
    expect_that(accepted > t/2.0, is_true())
    print("test_kmeans_assignment: mouse_1_spherical kmeans energy is correct")
})
