#!/bin/bash

URL="http://pasieka.ii.uj.edu.pl/~data/bigdata/"
DATA_DIR="../data"
FILES=`cat test_data_list`
for f in $FILES
do
    wget $URL/$f -O $DATA_DIR/$f
done
