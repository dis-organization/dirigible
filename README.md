
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
f <- system.file("extdata/tab/list_locality_postcode_meander_valley.tab", package = "vapour")
dirigible:::driver_gdal(f)
#> [1] "MapInfo File"
dirigible:::feature_count_gdal(f, layer = 0, iterate = FALSE)
#> [1] 58

dirigible:::layer_names_gdal(f)
#> [1] "list_locality_postcode_meander_valley"

## white zone
dirigible:::dirigible_unload_gdal()
try(dirigible:::layer_names_gdal(f))
#> Error in dirigible:::layer_names_gdal(f) : Open failed.
dirigible:::dirigible_load_gdal()
#> [1] TRUE
dirigible:::layer_names_gdal(f)
#> [1] "list_locality_postcode_meander_valley"
```

-----

## Code of Conduct

Please note that the dirigible project is released with a [Contributor
Code of
Conduct](https://contributor-covenant.org/version/2/0/CODE_OF_CONDUCT.html).
By contributing to this project, you agree to abide by its terms.
