#!/bin/bash

URL="http://pasieka.ii.uj.edu.pl/~data/bigdata"
DATA_DIR="../data"
DIRS=`cat test_data_dirs_list`
FILES=`cat test_data_list`

# mkdir if not exist
for d in $DIRS
do
    mkdir $DATA_DIR/$d
done

# Download data
for f in $FILES
do
    wget $URL/$f -O $DATA_DIR/$f
done
