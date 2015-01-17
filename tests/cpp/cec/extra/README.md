CEC performance testing
==============

These scripts compares performance of gmum cec with cran cec.
Configuration is in common.R file.
To make this work you have to set correct cran and gmum paths (paths to root directory).
Basic configuration provides generating points in range from 100 to 5000 with step 50 (100, 150, ..., 5000).
To run script execute ./run.sh, it should generate data folder and 2 plots iters_plot.jpg and times_plot.jpg.
To clean generated results run ./clean.sh
