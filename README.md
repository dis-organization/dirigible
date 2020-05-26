
<!-- README.md is generated from README.Rmd. Please edit that file -->

# dirigible

<!-- badges: start -->

<!-- badges: end -->

A fresh attempt at vapour. GDAL for R using a headers appraoch.

## Installation

TBD but don’t do it this will break your system

## Example

Linux only for now.

``` r
library(dirigible)
f <- system.file("extdata/tab/list_locality_postcode_meander_valley.tab", package = "vapour")

## name of driver in use
dirigible:::driver_gdal(f)
#> [1] "MapInfo File"

## layer feature count
dirigible:::feature_count_gdal(f, layer = 0, iterate = FALSE)
#> [1] 58

## vector layer names
dirigible:::layer_names_gdal(f)
#> [1] "list_locality_postcode_meander_valley"

## read fields
as.data.frame(dirigible:::read_fields_gdal(f, layer = 0, sql = "", limit_n = 1, skip_n = 0, ex = 0, fid_column_name = character(0)))
#>   LOCAL_ID     NAME POSTCODE PLAN_REF   GAZ_DATE NOM_REG_NO
#> 1   100422 Caveside     7304  CPR5322 1970-01-01       947L
#>                                      UFI          CREATED_ON
#> 1 {4a5db4da-ca19-41a0-8dd4-c28a14bbee18} 2016-03-04 10:42:37
#>                                LIST_GUID SHAPE_AREA SHAPE_LEN
#> 1 {839edd46-01a7-4a45-9d97-499962fa952b}      -9999  39785.88
 


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
