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

test_that("corectness works", {
  dataset_clusters <- as.vector(read.table("../cpp/data/EllipseGauss/cluster.txt")[,1])
  dataset_points <- as.matrix(read.table("../cpp/data/EllipseGauss/input.txt"))
  nclusters <- 4
  npoints = dim(dataset_points)[1]
    
  correct_percentage <- correctness(dataset_clusters, dataset_clusters, npoints, nclusters)
  expect_that(correct_percentage, equals(1))
})

test_that("EllipseGauss random assignment is correct", {
  dataset_clusters <- as.vector(read.table("../cpp/data/EllipseGauss/cluster.txt")[,1])
  dataset_points <- as.matrix(read.table("../cpp/data/EllipseGauss/input.txt"))
  expected_energy <- as.numeric(read.table("../cpp/data/EllipseGauss/energy.txt"))
  
  t <- 20
  accepted <- 0
  nclusters <- 4
  npoints = dim(dataset_points)[1]
  for(i in 1:t)
  {
    assignment = sample(0:(nclusters-1), npoints, replace=T)
    args = list(k=nclusters, x=dataset_points, params.centroids = assignment)
    c <- CEC(args)
    correct_percentage <- correctness(dataset_clusters, c$y(), npoints, nclusters)
    if(c$entropy() < (1.5 * expected_energy) | (correct_percentage >= 0.9) ) {
      accepted <- accepted + 1
    }
  }
  expect_that(accepted, is_more_than(t / 2))
})
