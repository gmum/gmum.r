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
render("svm.news20.times.R", output_options=c(highlight=highlighting))

render("gng.online.R", output_options=c(highlight=highlighting))
render("gng.wine.R", output_options=c( highlight=highlighting))
render("gng.optimized.R", output_options=c(highlight=highlighting))
render("gng.mouse.R", output_options=c(highlight=highlighting))

# THIS IS REALLY TRICKY - we generate not self contained files
# and then link them to gmum.r FTP server 

scripts <- list.files(".", pattern = glob2rx("*.R"), full.names = FALSE)
# TO RUN FOR PART REPLACE SCRIPTS WITH LIST OF FILES YOU WISH TO UPDATE

create.sed.cmd <- function(script.name){
  file.folder <- substr(script.name, 1, nchar(script.name)-2)
  replace_command = 
    paste("sed -i 's/",file.folder,
        "/http:\\/\\/gmum.net\\/files\\/gmum.r\\/",file.folder,"/g' *.html", sep="")
}
cmds <- lapply(scripts, create.sed.cmd)
for(cmd in cmds){
  system(cmd)
}


