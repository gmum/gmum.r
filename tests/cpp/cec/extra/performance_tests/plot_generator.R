generate_plot <- function(plot_name, x, y1, y2, x_label, y_label, y1_label, y2_label, file_name) {
    print(file_name)
    min_x = min(x)
    max_x = max(x)
    min_y = min(min(y1), min(y2))
    max_y = max(max(y1), max(y2))
    
    jpeg(file_name)
    plot( c(min_x, max_x), c(min_y, max_y), type="n", xlab=x_label, ylab=y_label )
    title(main=plot_name)
    lines(x, y1, col="green", lwd=2.5)
    lines(x, y2, col="red", lwd=2.5)
    par(xpd=TRUE)
    legend("bottom", legend = c(y1_label, y2_label), text.width = max(sapply(text, strwidth)),
           col=c("green", "red"), lwd=5, cex=1, horiz = TRUE)
    par(xpd=FALSE)
    dev.off()    
}
