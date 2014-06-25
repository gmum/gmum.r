# Just a snippet to get you started on slidify

# Install required libraries (and less required)
require(devtools)
install_github(c('slidify', 'slidifyLibraries'), 'ramnathv', ref = 'dev')
install_github('rCharts', 'ramnathv')
install_github('shiny', 'rstudio')
library('slidify')
library('slidifyLibraries')

# Create skeleton. Do not run if already existing
author("test")

# Generate presentation
slidify("test/index.Rmd")

