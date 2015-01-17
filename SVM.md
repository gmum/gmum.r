gmum.r - SVM Wrapper
======

SVM Wrapper is a part of the gmum.R project which provides a popular Support Vector Machine inplementations wrapped in the R package.

## Prerelease version

Right now the only avaiable SVM implementation is the libSVM. We also provide [2e SVM pre-/postprocess algorithm](http://gmum.ii.uj.edu.pl/gmum.r/SVM/2eSVM.pdf).

## Two Ellipsoids SVM example plots

![2e Iris example](./doc/svm/img/2e.png "2e Iris example")

## Main methods

* **SVM**(formula, dataset, ... ) - create svm object.

* **predict**(svm, x) - predict class of given examples using a SVM object.

## Dataset methods

* **dataset.X**(svm) - print unlabeled data.

* **dataset.Y**(svm) - print labels.

* **svm.dataset.breast_cancer**() - load breast cancer dataset.

* **svm.dataset.2e**() - load example two ellipsoids dataset.

* **read.libsvm**(filename, dimensionality) - load libsvm dataset format. 

## Misc

* **acc**(prediction, target) - calculated accuracy of a prediction.

For detailed documentation please refer to provided documentation.

## Example usage

        library('gmum.r')

        # Load a dataset, here we have provided an example 
        ds <- svm.dataset.breast_cancer()
        
        # Create new SVM object
        svm <- SVM( formula = X1~. ,
                    data = ds,
                    lib = "libsvm",
                    kernel = "linear",
                    prep = "none",
                    C = 10) 
        
        # You can access the dataset 
        x <- dataset.X(svm)
        y <- dataset.Y(svm)
        
        # Classify your dataset using predict function
        prediction <- predict(svm, x)
        
        # Check models accuracy
        acc <- svm.accuracy(prediction=y, target=prediction)
