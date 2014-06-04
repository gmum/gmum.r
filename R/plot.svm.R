library(ggplot2)

plot2d.svm <- function( object, dim1, dim2) {
  
  d = object$getX()
  target = object$getY()
  
  df = data.frame(d)
  
  x = df[,dim1]
  y = df[,dim2]
  qplot(data=df, x=x, y=y, color=target) + scale_colour_gradientn(colours=rainbow(2),breaks = c(2,4))

}