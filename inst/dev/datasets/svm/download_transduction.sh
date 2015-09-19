#!/bin/bash

wget http://download.joachims.org/svm_light/examples/example2.tar.gz
gunzip example2.tar.gz
tar -xvf example2.tar

rm -r -f transductive
mkdir transductive
mv example2/* transductive
rm -r -f example2
rm example2.tar
sed '1d' transductive/train_transduction.dat > tmp
mv tmp transductive/train_transduction.dat
sed '1d' transductive/train_induction.dat > tmp
mv tmp transductive/train_induction.dat
sed '1d' transductive/test.dat > tmp
mv tmp transductive/test.dat
