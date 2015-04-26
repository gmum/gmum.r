#!/bin/bash
mkdir data
R < performance_tests.R --vanilla
R < external_dataset_tests.R --vanilla
