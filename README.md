
<!-- README.md is generated from README.Rmd. Please edit that file -->

# dirigible

<!-- badges: start -->

<!-- badges: end -->

A fresh attempt at vapour. GDAL for R using a headers appraoch.

## Installation

TBD but don’t do it this will break your system

## Example

Linux only for now. This merely proves that I am not completely lost.

``` r
library(dirigible)
dirigible:::driver_gdal("../vapour/inst/extdata/tab/list_locality_postcode_meander_valley.tab")
Error in dirigible:::driver_gdal("../vapour/inst/extdata/tab/list_locality_postcode_meander_valley.tab") : 
  Open failed.
dirigible:::register_gdal()  ## vapour did this every time code was run (should be in .onLoad() along with file-path DATA envvars)
[1] TRUE
dirigible:::driver_gdal("../vapour/inst/extdata/tab/list_locality_postcode_meander_valley.tab")
[1] "MapInfo File"
```

-----

## Code of Conduct

Please note that the dirigible project is released with a [Contributor
Code of
Conduct](https://contributor-covenant.org/version/2/0/CODE_OF_CONDUCT.html).
By contributing to this project, you agree to abide by its terms.
