mod <- Module("svm_wrapper")

test_norm <- function(x){
  
  con = mod$SVMConfiguration
  cli = mod$SVMClient
  config = new(con)
  config$data = x
  config$createParams( "poly", "libsvm", "norm", 2, 0.1, 0.0 )
  
  client = new(cli, config)
  client$run()
  result = client$getResult()
  return(result)
}