library(testthat)
library('gmum.r')

test_that("mouse_1_spherical kmeans test control eps boundary values", {
          data(cec_mouse_1_spherical)
          dataset_points <- input
          nclusters <- 3
          npoints = dim(dataset_points)[1]
          c <- CEC(k=nclusters, x=dataset_points, control.nstart=1, method.init='kmeans++', control.eps=((npoints - 1) / npoints))
          final_nclusters = tail(c$log.ncluster( ), n=1)
          expect_that(final_nclusters == 1, is_true())
          print("mouse_1_spherical kmeans test control eps boundary values is correct")
})

test_that("mouse_1_spherical random test control eps boundary values", {
          data(cec_mouse_1_spherical)
          dataset_points <- input
          nclusters <- 3
          npoints = dim(dataset_points)[1]
          c <- CEC(k=nclusters, x=dataset_points, control.nstart=1, method.init='random', control.eps=((npoints - 1) / npoints))
          final_nclusters = tail(c$log.ncluster( ), n=1)
          expect_that(final_nclusters == 1, is_true())
          print("mouse_1_spherical random test control eps boundary values is correct")
})

test_that("EllipseGauss kmeans test control eps boundary values", {
          data(cec_ellipse_gauss)
          dataset_points <- input
          nclusters <- 4
          npoints = dim(dataset_points)[1]
          c <- CEC(k=nclusters, x=dataset_points, control.nstart=1, method.init='kmeans++', control.eps=((npoints - 1) / npoints))
          final_nclusters = tail(c$log.ncluster( ), n=1)
          expect_that(final_nclusters == 1, is_true())
          print("EllipseGauss kmeans test control eps boundary values is correct")
})

test_that("EllipseGauss random test control eps boundary values", {
          data(cec_ellipse_gauss)
          dataset_points <- input
          nclusters <- 4
          npoints = dim(dataset_points)[1]
          c <- CEC(k=nclusters, x=dataset_points, control.nstart=1,  method.init='random', control.eps=((npoints - 1) / npoints))
          final_nclusters = tail(c$log.ncluster( ), n=1)
          expect_that(final_nclusters == 1, is_true())
          print("EllipseGauss random test control eps boundary values is correct")
})

test_that("mouse_1 kmeans test control eps boundary values", {
          data(cec_mouse_1)
          dataset_points <- input
          nclusters <- 3
          npoints = dim(dataset_points)[1]
          c <- CEC(k=nclusters, x=dataset_points, control.nstart=1, method.init='kmeans++', control.eps=((npoints - 1) / npoints))
          final_nclusters = tail(c$log.ncluster( ), n=1)
          expect_that(final_nclusters == 1, is_true())
          print("mouse_1 kmeans test control eps boundary values is correct")
})

test_that("mouse_1 random test control eps boundary values", {
          data(cec_mouse_1)
          dataset_points <- input
          nclusters <- 3
          npoints = dim(dataset_points)[1]
          c <- CEC(k=nclusters, x=dataset_points, control.nstart=1, method.init='random', control.eps=((npoints - 1) / npoints))
          final_nclusters = tail(c$log.ncluster( ), n=1)
          expect_that(final_nclusters == 1, is_true())
          print("mouse_1 random test control eps boundary values is correct")
})
