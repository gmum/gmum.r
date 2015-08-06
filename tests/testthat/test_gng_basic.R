library(testthat)

#TODO: add test for checking GNGConfiguration serialization

test_that("Basic saving/loading works", {
  g <- GNG(train.online=TRUE, dim=3, verbosity=-1); 
  insertExamples(g, gng.preset.sphere(300))
  Sys.sleep(1)
  pause(g)
  
  gngSave(g, file='mygraph.bin')
  
  g2 <- gngLoad("mygraph.bin")
  # Check basic deserialization
  expect_that(g2$.getConfiguration()$alpha == g$.getConfiguration()$alpha &&
                g2$.getConfiguration()$eps_n == g$.getConfiguration()$eps_n, is_true())

  # Check basic equivalency (TODO: check something deeper)
  for(i in 1:100){
    point <- runif(3)
    expect_that(g$predict(point) == g2$predict(point), is_true())
  }
  
  file.remove("mygraph.bin")
})

test_that("predictCluster returns sensible results", {
  data(cec.mouse1.spherical)
  g <- GNG(cec.mouse1.spherical, max.nodes=50)
  mouse_centr <- predictCentroids(g)
  
  m = as.data.frame(cec.mouse1.spherical)
  colnames(m) = c("x", "y")
  
  x_col <- cec.mouse1.spherical[,1]
  y_col <- cec.mouse1.spherical[,2]
  
  x_max <- max(x_col)
  x_min <- min(x_col) 
  y_max <- max(y_col)
  y_min <- min(y_col)
  
  x_axis <- seq(from=x_min, to=x_max, length.out=30)
  y_axis <- seq(from=y_min, to=y_max, length.out=30)
  grid <- data.frame(x_axis,y_axis)
  grid <- expand.grid(x=x_axis,y=y_axis)
  target <- findClosests(g, node.ids=mouse_centr, x=grid)
  target_loopy <- apply(grid, 1, function(x) findClosests(g, node.ids=mouse_centr, x=x))
  
  grid["target"] <- target
  library(ggplot2)
  
  pl <- ggplot()+ 
    geom_tile(data=grid, aes(x=x,y=y, fill=factor(target))) + theme(legend.position="none") +
    geom_point(data=m, aes(x,y), color='white') + scale_size_continuous(range = c(3, 6))
  plot(pl)
  
  # Equivalent
  expect_that(all(target==target_loopy), is_true())
  
  # More or less balanced (if fails - might be needed to change threshold)
  expect_that(all(sapply(table(target), function(x) x>40)), is_true())
  
  # At least catches most important clusters
  expect_that(length(predictCentroids(g)) > 3, is_true())
  
})

test_that("GNG converges on simple cases", {

    online_converged <- function(gng){
        n <- 0
        print("Waiting to converge")
        while(numberNodes(gng) != gng$.getConfiguration()$max_nodes && n < 100) {
          Sys.sleep(1.0)
          n <- n + 1
        }
        test_that("GNG has reached expected number of vertexes", {
          expect_that(n < 100, is_true() )
        })
    }
    
    sanity_check <- function(gng){
      # Find closest node
      predict(gng, c(1,1,1))
      
      # # Get igraph
      ig <- convertToGraph(gng)
      
      # # Running testthat unit tests (almost)
      test_that("GNG has not isolated vertexes", {
        expect_that(any(degree(ig)==0), is_false()) 
      })
      
      test_that("GNG has converged", {
        error_before = meanError(gng)
        expect_that(error_before  < 50.0/max_nodes, is_true() )
      })
      
      # Test memory
      terminate(gng)
      print("Saving in sanity check:")
      gngSave(gng, "graph.bin")
    }


    max_nodes <- 600
    ex <- gng.preset.sphere(N=90000)
    gng <- GNG(max.nodes=max_nodes, train.online=TRUE, dim=3, verbosity=3) 
    insertExamples(gng, ex)
    run(gng)
    online_converged(gng)
    sanity_check(gng) 

    gng <- GNG(ex, max.nodes=max_nodes, train.online=TRUE, dim=3, verbosity=3)
    sanity_check(gng) 


    gng <- OptimizedGNG(max.nodes=max_nodes, train.online=TRUE, dim=3, verbosity=3, value.range=c(-2,2))    
    insertExamples(gng, ex)
    run(gng)
    online_converged(gng)
    sanity_check(gng) 
})

test_that("GNG is working on mouse dataset", {
    data(cec.mouse1.spherical)
    dataset = cec.mouse1.spherical
    gng <- GNG(dataset, seed=778)
  expect_that(gng$getMeanError() < 0.1, is_true())
  expect_that(all(gng$clustering() == predict(gng,dataset)), is_true())
  gng.refit <- GNG(dataset, seed=778)
  # Seeding works => error statistics are the same
  expect_that(all(abs(errorStatistics(gng.refit) - errorStatistics(gng)) < 1e-2), is_true() )
})

test_that("GNG clustering and predict are returning the same", {
  X <- replicate(10, rnorm(20))
  gng <- GNG(X)
  expect_that(all(gng$clustering() == predict(gng,X)), is_true())
})

test_that("GNG errorStatistics and node retrieval work", {
  X <- replicate(10, rnorm(20))
  gng <- GNG(X)
  expect_that(length(errorStatistics(gng)) > 1, is_true())
  node(gng, 1)
})

test_that("GNG synchronization looks ok", {
  
  data(cec.mouse1.spherical)
  dataset = cec.mouse1.spherical
      synchronization_test <- function(){
        gng <- GNG(dataset, verbosity=3, max.nodes=20)
        gng$.updateClustering()
        sum_1 = (sum( gng$clustering() != predict(gng, dataset)))

        gng <- GNG(train.online=TRUE, dim=2, verbosity=3, max.nodes=20)
        gng$insertExamples(dataset) 
        gng$pause()
        gng$.updateClustering()

        sum_2 = (sum( gng$clustering() != predict(gng, dataset)))
        
        expect_that(sum_1 == 0 && sum_2 == 0, is_true())
    }
    for(i in 1:3){
        synchronization_test()
    }
})
