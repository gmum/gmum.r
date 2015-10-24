# Manual latest version installation

## Linux/MacOS

Installation should be straightforward.

Specify commit, tag, or branch name using **ref** argument. Defaults to "master".

Run in R:

```{R}
devtools::install_github("gmum/gmum.r", ref="dev")
```

Then load package for use:
```{R}
library(gmum.r)
```

## Compile with CMake

Go to project root directory and run following commands 

```{shell}
mkdir build
cd build
cmake ..
make
make install
cd ..
```

Now you can use library from R

```{R}
library(devtools)
devtools::load_all()
...
```

## Windows

Please download appropriate version of [Rtools](http://cran.r-project.org/bin/windows/Rtools/).

Specify commit, tag, or branch name using **ref** argument. Defaults to "master".

Run R matching your CPU architecture (x64 vs x86)

```{R}
devtools::install_github("gmum/gmum.r", ref="dev", args="--no-lock --no-multiarch")
```

To build both x64 and x86 run:

```{R}
devtools::install_github("gmum/gmum.r", ref="dev", args="--no-lock --preclean")
```

Then load package for use:
```{R}
library(gmum.r)
```

### Known issues:

* If you have non-english letters in home path please consider installing from administrator cmd.

```{shell}
R CMD INSTALL . --no-lock --no-multiarch --preclean --library=<path_to_your_R_library_folder>
```

, where path might be `C:\Program Files\R\R-3.1.2\library"`. 

