<<<<<<< HEAD
#Cross Entropy Clustering in R

Part of gmum.R project. See gmum research group page [here](http://gmum.ii.uj.edu.pl) .
=======
#Growing Neural Gas in R

Easy to install and use package for fast **online** GNG algorithm.

Part of gmum.R project. See gmum research group page [here](http://gmum.ii.uj.edu.pl).
>>>>>>> gng/gmum.r

##General
------

<<<<<<< HEAD
CEC aims to efficiently implement Cross Entropy Clustering Algorithm as R extension.
=======
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

Daniel Fiser, Jan Faigl, Miroslav Kulich optimization paper [FGNG: A fast multi-dimensional growing neural gas implementation](http://www.sciencedirect.com/science/article/pii/S0925231213009259)

##Examples
-----

<small>MNIST dataset clustering (raw images)</small>
<center><img src="https://raw.github.com/kudkudak/Growing-Neural-Gas/dev/doc/img/mnist1.png" width="60%"></img></center>

<small>Reconstruction of the Buddha figure from Standford Repositories</small>
<center><img src="https://raw.github.com/kudkudak/Growing-Neural-Gas/dev/doc/img/ex3.png" width="60%"></img></center>

>>>>>>> gng/gmum.r

##Installation
-----------

<<<<<<< HEAD
You can install CEC as an R package, the easiest way for this is using devtools.

To install devtools you need
```R
> install.packages('devtools')
```

If you use Debian you can install some dependencies with
```bash
# apt-get install r-cran-rcpparmadillo r-cran-rcurl
```

Make sure you have boost if you are unix/mac user. It is packaged in most distributions.
For instance in ubuntu you can download it using 
```bash
# apt-get install libboost-all-dev
```

to install CEC
```R
devtools::install_github("gmum/gmum.r/tree/cec")
```

See **Known issues** if your installation fails.

##Usage
-----

For more detailed usage see code in demo folder.

##Testing
-----
There are two sets of tests. You can run them seperately. One require Google C++ Test Framework, the other dectools.

To run tests in Google C++ Test Framework
```bash
$ cd gmum/test/cpp
$ export GTEST='path to google c++ test framework'
$ make
$ ./mouse_1_test #for instance
```

To run R tests type
```
$ cd gmum
$ R
> devtools::test(".")
```

##Contributors
---------
Feel free to contribute to the code. Contributions should be posted as pull requests.

You can get your own copy and compile it like this
```
$ git clone https://github.com/gmum/gmum.r
$ cd gmum
$ R
> library(devtools)
> load_all()
> #you can use CEC from now on
```

##Known issues
---------
=======
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

=======
**Note**: Support for Windows is experimental, I am working on that. 

You can install it as an R package, it is easiest to do it using devtools (make sure you download devtools and RCurl packages in the first place).

```Matlab
devtools::install_github("kudkudak/Growing-Neural-Gas")
```

I recommend using RStudio, especially for plotting.

See **Known issues** if your installation fails. There are few problems with
R/igraph that are platform dependent.



##Usage

For more detailed usage see code in demo folder, or in tests/testthat folder.
You can also refer to R package documentation (pdf version 
[gng.pdf](https://github.com/kudkudak/Growing-Neural-Gas/blob/master/doc/gng.pdf)).

### Adding sphere data and clustering

```Matlab
library("GrowingNeuralGas")

max_nodes <- 600

# Create main GNG object (without optimization) with dataset sampling according
#to probability passed as last coordinate
gng <- GNG(dataset_type=gng.dataset.bagging.prob, max_nodes=max_nodes, dim=3)

# Add examples (note: you can avoid here copy using gng$insert_examples or set_memory_move_examples)
# Note: we are adding preset dataset with all sampling probabilities set to 80%
insert_examples(gng, preset=gng.preset.sphere, N=10000, prob=0.8)

# Run algorithm in parallel
run(gng)

# Wait for the graph to converge
print("Waiting to converge")
while(number_nodes(gng) != max_nodes) 
  Sys.sleep(1.0)

# Find closest node to vector [1,1,1]
predict(gng, c(1,1,1))

# Find mean error
mean_error(gng)

# Plot with first 2 coordinates
plot(gng, mode=gng.plot.2d.errors, vertex.color=gng.plot.color.cluster, 
     layout=gng.plot.layout.v2d)

# Terminate GNG, to free memory you should call rm(gng)
terminate(gng)
```


##List of functions

This is not a full documentation. Please refer to R package documentation (pdf version 
[gng.pdf](https://github.com/kudkudak/Growing-Neural-Gas/blob/master/doc/gng.pdf) ).

* GNG(...) - constructor for GNG object. Parameters:

    *  beta - Decrease the error variables of all node nodes by this fraction. Forgetting rate. Default 0.99

    *  alpha - Alpha coefficient. Decrease the error variables of the nodes neighboring to the newly inserted node by this fraction. Default 0.5

    *  uniformgrid_optimization - TRUE/FALSE. If TRUE please pass bounding box
       parameters also.

    *  lambda - how often to add new vertices. Default is 200
 
    *  lazyheap_optimization - TRUE/FALSE. 

    *  max.node - Maximum number of nodes (after reaching this size it will continue running, but won't add new nodes)

    *  eps_n - Default 0.05

    *  eps_v - Default 0.0006

    *  dataset_type - Dataset type. Possibilities gng.dataset.bagging, gng.dataset.bagging.prob (sampling according to dim+1 coordinate probability), gng.dataset.sequential (loop through examples - default option)

    *  experimental_utility_option - EXPERIMENTAL Utility option (try using it
       for quickly changing distributions). Value: gng.experimental.utility.option.off / gng.experimental.utility.option.basic

    *  experimental_utility_k - EXPERIMENTAL Utility option constant. Default
       to 1.5.

    *  experimental_vertex_extra_data

    *  load_model_filename - Set to path to file from which load serialized model

    *  uniformgrid_boundingbox_sides - Required for uniformgrid_optimization.
      You will need to define bounding box for your data, that will remain the
      same throughout the execution. To change it you should dump model and
      recreate from dumped file
    
    *  uniformgrid_boundingbox_origin - Origin of the bounding box    

    *  max_edge_age - Maximum age of edge, after which it is deleted. Decrease
       if your graph is not following changes of the dataset (you can also try
       experimental utility option)

* run(gng), pause(gng), terminate(gng) - execution control

* node(gng, gng_index) - returns node given index

* dump_model(gng, filename) - dump model to file

* mean_error(gng) - mean error in the graph

* number_nodes(gng) - returns number of nodes

* error_statistics(gng) - vector of errors every second

* plot(gng, mode, layout, vertex.color start_s) - plots gng using one of the presets (gng.plot.rgl3d,
  gng.plot.2d, gng.plot.2derrors). If plotting erros you can specify second from
  it will plot the errors. 

* centroids(gng) - using igraph algorithm GNG will write centroids of found
  clusters (community centers)

* convert_igraph(gng) - converts GNG to igraph

* predict(gng, x) - return gng_index of the closest node in the graph to given
  example

* insert_examples(gng, M) - inefficient adding examples to the graph

* gng$insert_examples() - add examples to gng object. **Note**: this will
  perform a copy to gng object dataset. To set memory pointer to your matrix use
  set_memory_move_examples function (you cannot change it to other memory
pointer later on, only add new examples). You can also use function
insert_examples(gng, ...), but this will perform 2 copies in RAM due to R design.



##Current work
----------

* Adapting Robust GNG strategies (http://www.researchgate.net/publication/8171175_Robust_growing_neural_gas_algorithm_with_application_in_cluster_analysis)

* Windows 32 and 64 bit dll

* Better igraph methods for clustering

* MDL based clustering (as in RGNG)

* Simple metric for clustering quality (PQ?)

* Testing cosine similarity for GNG

##Testing
-----
To run tests you will need Google C++ Test Framework
installed to your search paths.

To run R tests type `devtools::test(".")` in the package folder.

##Contributors
---------
Feel free to contribute to the code. Contributions should be posted as pull requests. 

##Known issues
---------
* Package is not released for Windows yet (however 64 bit should work)

* Due to bug in RStudio you might experience session crashes when creating many
  gng objects.

* Due to bug in R (https://bugs.r-project.org/bugzilla/show_bug.cgi?id=15327)
  on some OS you have to install liblzma-dev additionally. 

* Sometimes after installation of igraph you might have disabled graphml support 
  (http://lists.gnu.org/archive/html/igraph-help/2011-03/msg00101.html). Try
installing libxml2-dev package and reinstalling igraph.
>>>>>>> gng/gmum.r
