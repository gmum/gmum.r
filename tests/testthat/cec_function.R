f_standard <- function(m, sigma){
  return (m * log(2 * pi * exp(1)) / 2 + log(det(sigma)) / 2)
}