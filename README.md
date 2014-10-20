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

## Example

### R CMD

        devtools::load_all()
        svm <- SVM()
        x <- svm$getX()
        target <- svm$getY()
        train(svm)
        result <- predict(svm, x)
        acc <- accuracy.svm(y=result, t=target)
        print(acc)
    
### Script

        library('gmum.r')
        
        # Create new SVM object
        svm <- SVM(lib = "libsvm",
                   kernel = "linear",
                   prep = "none",
                   C = 1,
                   gamma = 0.01,
                   coef0 = 0) 
        
        # Load a dataset, here we have provided an example 
        ds <- read.libsvm( breast_cancer.path, 10 )
        
        # All functions are generic, meaning you nedd to provide your model as a first argument
        load_dataset(svm, ds)
        
        # You can access the dataset 
        x <- dataset.X(svm)
        y <- dataset.Y(svm)
        
        # You can change the dataset, but that requires class methods
        svm$setX(x)
        svm$setY(y)
        
        # You can change model paramaters using params function for specific parameter
        params.C(svm, 100)
        
        # or change multiple parameters at once
        params(svm, C=1, gamma=0.01)
        
        # To view specific parameter you can use the same function without a values, or print a summary for your model
        params.C(svm)
        print(svm)
        
        # Use train(object) function to train your model on provided dataset
        train(svm)
        
        # Classify your dataset using predict function
        prediction <- predict(svm, x)

        # Check models accuracy
        acc <- accuracy.svm(y=y, t=prediction)

