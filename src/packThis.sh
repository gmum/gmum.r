#!/bin/bash

# TODO: add checking for that
echo "Remember to run this script from root folder"

echo $#
if [ "$#" -ne 1 ]; then
    destination="pkg"
else
    destination=$1
fi

echo "Packing to ", $destination

echo "Creating folder structure"
mkdir -p $destination

echo "Copy necessary files"
rsync -rP --exclude=".*" --exclude="demo/samples/*" --exclude="inst/data_sets/*" --exclude="cmake/*" --exclude="build/*" --exclude="doc/*" --exclude="libs/*" --exclude="tests/*" --exclude="..Rcheck/*" --include="*.cpp" --include="*.h" --include="*.hpp" --include="*.Rd" --include="*.c" --include="*.R" --include="*/" --exclude="*" --exclude="pkg/*" --exclude="pkg" . $destination

# Copy datasets
mkdir $destination/data
cp data/cec.mouse1.spherical.RData $destination/data
cp data/cec.tset.RData $destination/data
cp data/cec.ellipsegauss.RData $destination/data
cp data/svm.transduction.RData $destination/data
cp data/svm_breast_cancer_dataset.RData $destination/data

# Some individual files
cp DESCRIPTION $destination
cp LICENSE $destination
cp NAMESPACE $destination
cp demo/00Index $destination/demo
cp src/Makevars $destination/src
cp src/Makevars.win $destination/src
cp inst/include/svmlight/Makefile $destination/inst/include/svmlight/
cp inst/include/svmlight/LICENSE.txt $destination/inst/include/svmlight/

# TODO: this is scary, but rsync refuses to stop copying hidden files
# NOTE: be super, super scared of modyfing next line, and don't add -f by no means
# we all remember Steam bug, lol
rm -r $destination/..Rcheck
rm -r $destination/.git
rm -r $destination/.idea
rm -r pkg/pkg
rm -r pkg/doc
rm -r pkg/cmake
rm -r pkg/libs
