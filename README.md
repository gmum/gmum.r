#Cross Entropy Clustering in R

Part of gmum.R project. See gmum research group page [here](http://gmum.ii.uj.edu.pl) .

##General
------

CEC aims to efficiently implement Cross Entropy Clustering Algorithm as R extension.

##Installation
-----------

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
