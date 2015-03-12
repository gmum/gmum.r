# Manual latest version installation

## Linux/MacOS

Installation should be straightforward. Run in R:

```{R}
devtools::install_github("gmum/gmum.r")
```

## Windows

Please download appropriate version of (http://cran.r-project.org/bin/windows/Rtools/)[http://cran.r-project.org/bin/windows/Rtools/].
Run R matching your cpu architecture (x64 vs x86)

```{R}
devtools::install_github("gmum/gmum.r", args="--no-lock --no-multiarch")
```

To build both x64 and x86 run

```{R}
devtools::install_github("gmum/gmum.r", args="--no-lock --preclean")
```

### Known issues:

* If you have non-english letters in home path please consider installing from administrator cmd.

```{shell}
R CMD INSTALL . --no-lock --no-multiarch --preclean --library=<path_to_your_R_library_folder>
```

,where path might be `C:\Program Files\R\R-3.1.2\library"`. 
