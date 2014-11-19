#Growing Neural Gas in R

Easy to install and use package for fast **online** GNG algorithm.

Part of gmum.R project. See gmum research group page [here](http://gmum.ii.uj.edu.pl).

##General
------

This package contains **fast** C++ implementation 
of **online** clustering algorithm Growing Neural Gas wrapped as R package using Rcpp.
It produces topological graph, that you can easily convert to igraph, or you can
dump your model to optimized binary file and load it later on.

This algorithms is widely used for dynamic clustering problem. Package is designed with
focus on **big datasets**. It is already possible to cluster dataset without making its
copy, with different dataset types (bagging, sequential or probability
sampling). In the near future it will be possible to stream dataset from csv file.

<small>Comparision of performance of original implementation and with
improvements suggested by D. Fiser, J. Faigl, M. Kulich </small>
<center><img src="https://raw.github.com/kudkudak/Growing-Neural-Gas/dev/doc/img/plot_speed.png" width="50%"></img></center>

* **SVM**(formula, dataset, ... ) - create svm object.

<small>MNIST dataset clustering (raw images)</small>
<center><img src="https://raw.github.com/kudkudak/Growing-Neural-Gas/dev/doc/img/mnist1.png" width="60%"></img></center>

## Dataset methods

* **dataset.X**(svm) - print unlabeled data.

```Matlab
devtools::install_github("kudkudak/Growing-Neural-Gas")
```

* **svm.dataset.breast_cancer**() - load breast cancer dataset.

* **svm.dataset.2e**() - load example two ellipsoids dataset.

* **read.libsvm**(filename, dimensionality) - load libsvm dataset format. 

## Misc

* **acc**(prediction, target) - calculated accuracy of a prediction.

# Find closest node to vector [1,1,1]
predict(gng, c(1,1,1))

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
                    C = 1,
                    gamma = 0.01,
                    coef0 = 0) 
        
        # You can access the dataset 
        x <- dataset.X(svm)
        y <- dataset.Y(svm)
        
        # Classify your dataset using predict function
        prediction <- predict(svm, x)
        
        # Check models accuracy
        acc <- svm.accuracy(prediction=y, target=prediction)

