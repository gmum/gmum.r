library(rmarkdown)

# Note: rmarkdown has some bugs, so I cannot do here loop, and also
# sometimes you have to terminate R and restart to compile successfuly ;)

highlighting <- "zenburn"

render("svm.2e.R", output_options=c(highlight=highlighting))
render("svm.basic.R", output_options=c(highlight=highlighting))
render("svm.caret.R", output_options=c(highlight=highlighting))
render("svm.multiclass.R", output_options=c(highlight=highlighting))
render("svm.transductive.R", output_options=c(highlight=highlighting))
render("svm.example.weights.R", output_options=c(highlight=highlighting))

render("gng.online.R", output_options=c(highlight=highlighting))
render("gng.wine.R", output_options=c(highlight=highlighting))
render("gng.optimized.R", output_options=c(highlight=highlighting))
render("gng.mouse.R", output_options=c(highlight=highlighting))
