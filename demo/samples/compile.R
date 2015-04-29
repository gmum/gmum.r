library(rmarkdown)

# Note: rmarkdown has some bugs, so I cannot do here loop, and also
# sometimes you have to terminate R and restart to compile successfuly ;)

highlighting <- "zenburn"
options(warn=-1)
render("svm.2e.R", output_options=c(highlight=highlighting))
render("svm.basic.R", output_options=c(highlight=highlighting))
render("svm.caret.R", output_options=c(highlight=highlighting))
render("svm.multiclass.R", output_options=c(highlight=highlighting))
render("svm.transductive.R", output_options=c(highlight=highlighting))
render("svm.example.weights.R", output_options=c(highlight=highlighting))
render("svm.news20.times.R", output_options=c(highlight=highlighting))

render("gng.online.R", output_options=c(highlight=highlighting))
render("gng.wine.R", output_options=c(highlight=highlighting))
render("gng.optimized.R", output_options=c(highlight=highlighting))
render("gng.mouse.R", output_options=c(highlight=highlighting))

# THIS IS REALLY TRICKY - we generate not self contained files
# and then link them to gmum.r FTP server 

scripts <- list.files(".", pattern = glob2rx("*.R"), full.names = FALSE)
# TO RUN FOR PART REPLACE SCRIPTS WITH LIST OF FILES YOU WISH TO UPDATE
run.cmds <- function(script.name){
  file.folder <- paste(substr(script.name, 1, nchar(script.name)-2), "_files", sep="")
  replace_command <-
    paste("sed -i 's/",file.folder,
        "/http:\\/\\/gmum.net\\/files\\/gmum.r\\/sample\\/",file.folder,"/g' *.html", sep="")
  system(replace_command)
  rem_command <- paste("rm -r -f ", file.folder, "/boot*", sep="")
  system(rem_command)
  rem_command <- paste("rm -r -f ", file.folder, "/jquery*", sep="")
  system(rem_command)
}

system.file("sed -i '/.*boot.*/d' *.html")
system.file("sed -i '/.*jquery.*/d' *.html")

for(script in scripts){
  run.cmds(script)
}


