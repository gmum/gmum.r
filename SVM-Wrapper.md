gmum.r - SVM Wrapper
======

SVM Wrapper is a part of the gmum.R project which provides a popular Support Vector Machine inplementations wrapped in the R package.

## Prerelease version

Right now the only avaiable SVM implementation is the libSVM. We also provide [2e SVM pre-/postprocess algorithm](http://gmum.ii.uj.edu.pl/gmum.r/SVM/2eSVM.pdf).

## Two Ellipsoids SVM example plots

![2e Iris example](./doc/svm/img/2e.png "2e Iris example")

## Main methods

* **SVM**(...) - create svm object.

* **train**(svm) - train a SVM object.

* **predict**(svm, x) - predict class of given examples using a SVM object.

* **params**(svm, ...) - change parameters of a SVM object.


## Dataset methods

* **load.dataset**(svm, dataset) - load a dataset to SVM object.

* **dataset.X**(svm) - print unlabeled data.

* **dataset.Y**(svm) - print labels.


## Parameters method

Each method act as a setter when given an argument, and as a getter when used without one.

* **params.kernel**(svm, ... )

* **params.preprocess**(svm, ... )

* **params.C**(svm, ... )

* **params.gamma**(svm, ... )

* **params.coef0**(svm, ... )

* **params.degree**(svm, ... )

* **params.shrinkin**g(svm, ... )

* **params.probability**(svm, ... )

* **params.cache_size**(svm, ... )


For detailed documentation please refer to provided documentation.
