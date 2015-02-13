library(testthat)
library(gtools)
library('gmum.r')

correctness <- function(correct_assignment, my_assignment, npoints, nclusters)
{
  perms <- permutations(v=0:(nclusters-1), n=nclusters, r=nclusters, repeats.allowed=F)
  nperms <- dim(perms)[1]
  best_percentage <- 0
  
  for(i in 1:nperms)
  {
    ncorrect <- 0
    for(j in 1:npoints) {
      if(correct_assignment[j] == perms[i, my_assignment[j] + 1]) {
        ncorrect <- ncorrect + 1 
      }
    }
    correct_percentage <- ncorrect / npoints
    best_percentage <- max(correct_percentage, best_percentage)
  }
  return(best_percentage)
}

test_that("correctness works", {
  data(cec_ellipse_gauss)
  
  dataset_clusters <- cluster[,1]
  dataset_points <- input
  
  nclusters <- 4
  npoints <- dim(dataset_points)[1]
  
  correct_percentage <- correctness(dataset_clusters, dataset_clusters, npoints, nclusters)
  expect_that(correct_percentage, equals(1))
  print("test_random_assignment: correctness works")
})

test_that("EllipseGauss random assignment is correct", {
  data(cec_ellipse_gauss)
  
  expected_energy <- energy_value
  dataset_clusters <- cluster[,1]
  dataset_points <- input
  
  dataset_clusters <- dataset_clusters - min(dataset_clusters)
  
  t <- 20
  accepted <- 0
  nclusters <- 4
  npoints = dim(dataset_points)[1]
  for(i in 1:t)
  {
    c <- CEC(k=nclusters, x=dataset_points, method.init='random')
    correct_percentage <- correctness(dataset_clusters, c$y(), npoints, nclusters)
    if(c$.entropy() < (1.5 * expected_energy) | (correct_percentage >= 0.9) ) {
      accepted <- accepted + 1
    }
  }
  expect_that(accepted > t/2.0, is_true())
  print("test_random_assignment: EllipseGauss random assignment is correct")
})

test_that("mouse_1 random assignment is correct", {
  data(cec_mouse_1)
  
  expected_energy <- energy_value
  dataset_clusters <- cluster[,1]
  dataset_points <- input
  
  dataset_clusters <- dataset_clusters - min(dataset_clusters)
  
  t <- 20
  accepted <- 0
  nclusters <- 3
  npoints <- dim(dataset_points)[1]
  for(i in 1:t)
  {
    #CEC(k=nclusters, x=dataset_points, method.init='random')
    c <- CEC(k=nclusters, x=dataset_points, method.init='random', method.type='sphere')
    
    correct_percentage <- correctness(dataset_clusters, c$y(), npoints, nclusters)
    if(c$.entropy() < (1.5 * expected_energy) | (correct_percentage >= 0.9) ) {
      accepted <- accepted + 1
    }
  }
  expect_that(accepted > t/2.0, is_true())
  print("test_random_assignment: mouse_1 random assignment is correct")
})

test_that("mouse_1_spherical random assignment is correct", {
  data(cec_mouse_1_spherical)
  
  expected_energy <- energy_value
  dataset_clusters <- cluster[,1]
  dataset_points <- input 
  
  dataset_clusters <- dataset_clusters - min(dataset_clusters)
  
  t <- 20
  accepted <- 0
  nclusters <- 3
  npoints <- dim(dataset_points)[1]
  for(i in 1:t)
  {
    c <- CEC(k=nclusters, x=dataset_points, method.type='sphere', method.init='random')
    correct_percentage <- correctness(dataset_clusters, c$y(), npoints, nclusters)
    if(c$.entropy() < (1.5 * expected_energy) | (correct_percentage >= 0.9) ) {
      accepted <- accepted + 1
    }
  }
  expect_that(accepted > t/2.0, is_true())
  print("test_random_assignment: mouse_1_spherical random assignment is correct")
})
