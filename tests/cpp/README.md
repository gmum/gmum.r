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

Run compiled executable binary: `./run_tests`

To run tests individually, use helper `./*.sh` scripts or refer to
`./run_tests -h` from GTest:

```
Test Selection:
  --gtest_list_tests
      List the names of all tests instead of running them. The name of
      TEST(Foo, Bar) is "Foo.Bar".
  --gtest_filter=POSTIVE_PATTERNS[-NEGATIVE_PATTERNS]
      Run only the tests whose name matches one of the positive patterns but
      none of the negative patterns. '?' matches any single character; '*'
      matches any substring; ':' separates two patterns.
```

Examples:

`./run_tests --gtest_filter=TwoESVMTest.*` runs all tests from TwoESVMTest.

`./run_tests --gtest_filter=TwoESVMTest.TestPreprocessor` runs only one test.

## Adding tests

Add new tests as `*.cc` files in subdirectories. You can create a new directory
for your own branch/type of tests. To compile new directory, add its name to
Makefile `TEST_BRANCHES` variable.

## Disabling tests

To disable single test case, prefix it with a `DISABLE_` string.

To disable file, rename its extension. For example from `*.cc` to `*.xcc` is a
common convention.

We want to disable tests when they are for example no longer compatible with
current architecture or in any other way are blockers for
continous integration. Disabled tests can be fixed later, while continous
integration still goes good.

