#Cross Entropy Clustering in R

Part of gmum.R project. See gmum research group page [here](http://gmum.ii.uj.edu.pl) .

##General
------

##Examples
-----


##Installation
-----------

You can install it as an R package, it is easiest to do it using devtools (of
course make sure you download devtools package in the first place).

Make sure you have boost if you are unix/mac user. It is packaged in most distributions.
For instance in ubuntu you can download it using `sudo apt-get install
libboost-all-dev`

Make sure you run R in sudo mode when on unix (required for Rcpp linking).

```Matlab
devtools::install_github("gmum/gmum.r/tree/cec")
```

We recommend using RStudio, especially for plotting (RStudio is doing plots in
double buffer, which results in much faster igraph plots).

See **Known issues** if your installation fails.

##Usage

For more detailed usage see code in demo folder.

##List of functions

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
