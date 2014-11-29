C++ Tests
=========

This directory contains C++ code tests.

## Dependencies

### GoogleTest

To compile these tests you need to install **GoogleTest** >= 1.6.0.

Arch Linux package: `gtest`, Debian/Ubuntu package: `libgtest-dev`

### GMUM.R dependencies

C++ **Armadillo** library and its dependencies ( **Lapack**, **Blas**) need to
be installed.

## Build

To build tests `cd` into this directory (`tests/cpp`) and type `make`.

## Run tests

Run compiled executable binary: `./test_all`

## Adding tests

Add new tests as `*.cc` files in subdirectories. You can create a new directory
for your own branch/type of tests. To compile its tests add its name inside
Makefile `TEST_BRANCHES` variable.

