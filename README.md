
<!-- README.md is generated from README.Rmd. Please edit that file -->

# dirigible

<!-- badges: start -->

<!-- badges: end -->

GDAL headers for R. (A fresh attempt at vapour with a firmer re-use
basis).

## Installation

TBD but don’t do it this will break your system

## Example

Linux only for now.

``` r
library(dirigible)
f <- system.file("extdata/tab/list_locality_postcode_meander_valley.tab", package = "vapour")

## name of driver in use
dirigible:::driver_gdal_cpp(f)
#> [1] "MapInfo File"

## layer feature count
dirigible:::feature_count_gdal_cpp(f, layer = 0)
#> [1] 58

## vector layer names
dirigible:::layer_names_gdal_cpp(f)
#> [1] "list_locality_postcode_meander_valley"

## read fields
as.data.frame(dirigible:::read_fields_gdal_cpp(f, layer = 0, sql = "", limit_n = 1, skip_n = 0, ex = 0, fid_column_name = character(0)))
#>   LOCAL_ID     NAME POSTCODE PLAN_REF   GAZ_DATE NOM_REG_NO
#> 1   100422 Caveside     7304  CPR5322 1970-01-01       947L
#>                                      UFI          CREATED_ON
#> 1 {4a5db4da-ca19-41a0-8dd4-c28a14bbee18} 2016-03-04 10:42:37
#>                                LIST_GUID SHAPE_AREA SHAPE_LEN
#> 1 {839edd46-01a7-4a45-9d97-499962fa952b}      -9999  39785.88
 
## read names

unlist(dirigible:::read_names_gdal_cpp(f, 
                            layer = 0, sql = "", 
                            limit_n = 10, 
                            skip_n = 0, ex = 0))
#>  [1]  1  2  3  4  5  6  7  8  9 10
osm <- system.file("extdata/osm/osm-ways.osm", package = "vapour", mustWork = TRUE)
unlist(dirigible:::read_names_gdal_cpp(osm, 
                            layer = 1,   ## layer 1 is 'lines'
                            sql = "", 
                            limit_n = 0, 
                            skip_n = 0, ex = 0))
#> [1] 100 101 102 103 104

## read proj info
names(dirigible:::projection_info_gdal_cpp(f, layer = 0, sql = ""))
#> Warning in dirigible:::projection_info_gdal_cpp(f, layer = 0, sql = ""): not
#> null
#> [1] "Proj4"      "MICoordSys" "PrettyWkt"  "Wkt"        "EPSG"      
#> [6] "XML"

## read geometry
str(dirigible:::read_geometry_gdal_cpp(f, layer = 0, sql = "",
                         what = "geometry",
                         textformat = "json",
                         limit_n = 1, skip_n = 0, ex = 0))
#> List of 1
#>  $ : raw [1:19485] 01 03 00 00 ...

str(dirigible:::read_geometry_gdal_cpp(f, layer = 0, sql = "",
                         what = "text",
                         textformat = "json",
                         limit_n = 1, skip_n = 0, ex = 0))
#> List of 1
#>  $ : chr "{ \"type\": \"Polygon\", \"coordinates\": [ [ [ 454430.22, 5396950.82 ], [ 454430.86, 5396948.43 ], [ 454432.44"| __truncated__

str(dirigible:::read_geometry_gdal_cpp(f, layer = 0, sql = "",
                         what = "text",
                         textformat = "wkt",
                         limit_n = 1, skip_n = 0, ex = 0))
#> List of 1
#>  $ : chr "POLYGON ((454430.22 5396950.82,454430.86 5396948.43,454432.44 5396943.6,454434.6 5396937.44,454448.18 5396910.5"| __truncated__

str(dirigible:::read_geometry_gdal_cpp(f, layer = 0, sql = "",
                         what = "text",
                         textformat = "gml",
                         limit_n = 1, skip_n = 0, ex = 0))
#> List of 1
#>  $ : chr "<gml:Polygon><gml:outerBoundaryIs><gml:LinearRing><gml:coordinates>454430.22,5396950.82 454430.86,5396948.43 45"| __truncated__

## got to be longlat for kml (just fwiw)
f1 <- system.file("gpkg/nc.gpkg", package = "sf")

str(dirigible:::read_geometry_gdal_cpp(f1, layer = 0, sql = "",
                         what = "text",
                         textformat = "kml",
                         limit_n = 1, skip_n = 0, ex = 0))
#> List of 1
#>  $ : chr "<MultiGeometry><Polygon><outerBoundaryIs><LinearRing><coordinates>-81.4727554321289,36.2343559265137 -81.540840"| __truncated__

## sys
dirigible:::version_gdal_cpp()
#> [1] "GDAL 3.0.4, released 2020/01/28"
str(dirigible:::drivers_list_gdal_cpp())
#> List of 7
#>  $ driver : chr [1:226] "VRT" "DERIVED" "GTiff" "NITF" ...
#>  $ name   : chr [1:226] "Virtual Raster" "Derived datasets using VRT pixel functions" "GeoTIFF" "National Imagery Transmission Format" ...
#>  $ vector : logi [1:226] FALSE FALSE FALSE FALSE FALSE FALSE ...
#>  $ raster : logi [1:226] TRUE TRUE TRUE TRUE TRUE TRUE ...
#>  $ create : logi [1:226] TRUE FALSE TRUE TRUE FALSE FALSE ...
#>  $ copy   : logi [1:226] TRUE FALSE TRUE TRUE FALSE FALSE ...
#>  $ virtual: logi [1:226] TRUE FALSE TRUE TRUE TRUE TRUE ...

dirigible:::proj_to_wkt_gdal_cpp("+proj=laea +datum=WGS84")
#> [1] "PROJCS[\"unknown\",GEOGCS[\"unknown\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9122\"]]],PROJECTION[\"Lambert_Azimuthal_Equal_Area\"],PARAMETER[\"latitude_of_center\",0],PARAMETER[\"longitude_of_center\",0],PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0],UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]],AXIS[\"Easting\",EAST],AXIS[\"Northing\",NORTH]]"

## white zone
dirigible:::dirigible_unload_gdal()
try(dirigible:::layer_names_gdal_cpp(f))
#> Error in dirigible:::layer_names_gdal_cpp(f) : Open failed.
dirigible:::dirigible_load_gdal()
#> [1] TRUE
dirigible:::layer_names_gdal_cpp(f)
#> [1] "list_locality_postcode_meander_valley"
```

-----

## Code of Conduct

Please note that the dirigible project is released with a [Contributor
Code of
Conduct](https://contributor-covenant.org/version/2/0/CODE_OF_CONDUCT.html).
By contributing to this project, you agree to abide by its terms.
