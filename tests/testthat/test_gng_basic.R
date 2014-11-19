library(devtools)
for(k in 1:1){
  
  library(testthat)
  library(gmum.r)
  library(igraph)
  
  

  print("her")  
  max_nodes <- 600
  
  # Construct gng object
  gng <- GNG(dataset_type=gng.dataset.bagging.prob, max_nodes=max_nodes, dim=3,
             uniformgrid_optimization=FALSE,  lazyheap_optimization=FALSE, verbosity=10)
             
  
  
  # Construct examples, here we will use a sphere
  ex <- gng.preset.sphere(N=90000)
  print("her")
  print(dim(ex))
  print("her")
  ex_binded <- cbind(ex, rep(0.6, nrow(ex) ))
  
  
  # Set examples pointer, note: this is the most
  # efficient way to pass examples, however you can use it
  # only once (it sets hard link to memory inside)
  print("here")

#  gng$insert_examples(ex_binded)
  
  # Run algorithm in parallel
 # run(gng)
  
  # Wait for the graph to converge
 # n <- 0
 # print("Waiting to converge")
 # while(number_nodes(gng) != gng$get_configuration()$max_nodes && n < 100) {
 #   Sys.sleep(1.0)
 #   n <- n + 1
#  }
}

