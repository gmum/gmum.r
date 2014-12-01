#!/bin/bash

if [ "$#" -eq 2 ]; then
    URL="http://pasieka.ii.uj.edu.pl/~data/bigdata/"
    FILE=$1
    DST=$2
elif [ "$#" -eq 3 ]; then
    URL=$1
    FILE=$2
    DST=$3
else
    echo "invalid number of arguments"
    exit
fi

wget $URL/$FILE -O $DST
