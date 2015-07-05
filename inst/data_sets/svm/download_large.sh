#!/bin/bash

rm -r -f large
mkdir large

wget http://gmum.net/files/gmum.r/data/news20_part.RData
mv news20_part.RData large

