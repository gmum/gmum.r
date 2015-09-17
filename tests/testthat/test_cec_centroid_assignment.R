library(testthat)
library('gmum.r')

source('combinations.R')

test_that("EllipseGauss centroid assignment is correct", {
  
  data(cec_ellipse_gauss)
  
  expected_energy <- cec.ellipsegauss.extra[["energy"]]
  dataset_clusters <- cec.ellipsegauss.extra[["cluster"]]
  dataset_points <- cec.ellipsegauss
  
  dataset_clusters <- dataset_clusters - min(dataset_clusters)
  
  t <- 20
  accepted <- 0
  nclusters <- 4
  npoints = dim(dataset_points)[1]
  for(i in 1:t)
  {
    c1 <- CEC(k=nclusters, x=dataset_points, method.init='random', control.itmax=0, control.nstart=1, seed=13371337)
    centers <- c1$centers
    centers_len = length(centers)
    c2 <- CEC(k=centers_len, x=dataset_points, method.init='centroids', params.centroids=centers)
    c3 <- CEC(k=centers_len, x=dataset_points, method.init='centroids', params.centroids=centers)
    expect_that(c2$energy, equals(c3$energy, tolerance = 0))
    actual_energy = c2$energy
    correct_percentage <- correctness(dataset_clusters, c2$clustering, npoints, centers_len)
    if(isTRUE(all.equal(current = actual_energy, target=expected_energy, tolerance=0.2)) | (correct_percentage >= 0.9) ) {
      accepted <- accepted + 1
    }
  }
  expect_that(accepted > t/2.0, is_true())
  
  print("test_centroid_assignment: EllipseGauss centroid assignment is correct")
})
