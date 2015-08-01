Testthat Tests
==============

This directory contains R code tests.

## Dependencies

* caret package
* e1071 package
* klaR package (and svmlight binaries in path, download from http://svmlight.joachims.org/. For Windows
make sure you add appropriate path to PATH variable or drop them into root folder of package)

### Testthat

In order to run these tests you need to install **testthat** R package.

```
install.packages("testthat")
```

## Run tests

To run all tests use devtools command:

```
devtools::test()
```

