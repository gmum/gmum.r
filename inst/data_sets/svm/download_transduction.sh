#!/bin/bash

wget http://download.joachims.org/svm_light/examples/example2.tar.gz
gunzip example2.tar.gz
tar -xvf example2.tar
mv example2 transductive
rm -r -f example2
rm example2.tar
