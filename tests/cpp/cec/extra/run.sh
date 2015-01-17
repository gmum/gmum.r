#!/bin/bash

R < data_generator.R --vanilla
R < gmum_cec_perf_test.R --vanilla
R < cran_cec_perf_test.R --vanilla
R < plot_generator.R --vanilla
