
<!-- README.md is generated from README.Rmd. Please edit that file -->

# dirigible

<!-- badges: start -->

[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://www.tidyverse.org/lifecycle/#experimental)
<!-- badges: end -->

GDAL headers for R. (A fresh attempt at vapour with a firmer re-use
basis).

The underlying `gdalheaders` namespace is now feature complete with the
C++ level of the [vapour
package](https://CRAN.r-project.org/package=vapour). The main thing to
do is establish this namespace as a headers-only package, then migrate
vapour over to use that. To do that we need Windows and MacOS support,
which includes making sure the rwinlib tools/ stuff works and that the
metadata files are set up correctly on package load, and cleaned up with
unload.

## Installation

TBD but don’t do it this will break your system

## Example

Linux only for now.

``` r
library(dirigible)
f <- system.file("extdata/tab/list_locality_postcode_meander_valley.tab", package = "vapour")
l <- "list_locality_postcode_meander_valley"
## name of driver in use
dirigible:::driver_gdal_cpp(f)
#> [1] "MapInfo File"

## layer feature count
dirigible:::feature_count_gdal_cpp(f, layer = 0, sql = "", ex = 0)
#> [1] 58

## vector layer names
dirigible:::layer_names_gdal_cpp(f)
#> [1] "list_locality_postcode_meander_valley"

## report fields
dirigible:::report_fields_gdal_cpp(f, 0, sql = "")
#>     LOCAL_ID         NAME     POSTCODE     PLAN_REF     GAZ_DATE   NOM_REG_NO 
#>    "OFTReal"  "OFTString" "OFTInteger"  "OFTString"    "OFTDate"  "OFTString" 
#>          UFI   CREATED_ON    LIST_GUID   SHAPE_AREA    SHAPE_LEN 
#>  "OFTString"  "OFTString"  "OFTString"    "OFTReal"    "OFTReal"

## report fields
dirigible:::report_fields_gdal_cpp(f, 0, 
                sql = "SELECT FID, NAME FROM list_locality_postcode_meander_valley")
#>          FID         NAME 
#> "OFTInteger"  "OFTString"


## read fields
as.data.frame(dirigible:::read_fields_gdal_cpp(f, layer = 0, sql = "", limit_n = 1, skip_n = 0, ex = 0, fid_column_name = character(0)))
#>   LOCAL_ID     NAME POSTCODE PLAN_REF   GAZ_DATE NOM_REG_NO
#> 1   100422 Caveside     7304  CPR5322 1970-01-01       947L
#>                                      UFI          CREATED_ON
#> 1 {4a5db4da-ca19-41a0-8dd4-c28a14bbee18} 2016-03-04 10:42:37
#>                                LIST_GUID SHAPE_AREA SHAPE_LEN
#> 1 {839edd46-01a7-4a45-9d97-499962fa952b}      -9999  39785.88
 
## VSI list
u <- "/vsizip//vsicurl/http://dapds00.nci.org.au/thredds/fileServer/rr2/national_geophysical_compilations/http/radmap_v3_2015_filtered_dose/radmap_v3_2015_filtered_dose.ers.zip"
(paths <- dirigible:::vsi_list_gdal_cpp(u))
#> [1] "radmap_v3_2015_filtered_dose"     "radmap_v3_2015_filtered_dose.ers"
#> [3] "radmap_v3_2015_filtered_dose.isi" "radmap_v3_2015_filtered_dose.txt"

## sds
sdsfile <- system.file("extdata/gdal/sds.nc", package = "vapour", mustWork = TRUE)
dirigible:::sds_list_gdal_cpp(sdsfile)
#> [1] "SUBDATASET_1_NAME=NETCDF:\"/usr/local/lib/R/site-library/vapour/extdata/gdal/sds.nc\":vv" 
#> [2] "SUBDATASET_1_DESC=[61x87] vv (64-bit floating-point)"                                     
#> [3] "SUBDATASET_2_NAME=NETCDF:\"/usr/local/lib/R/site-library/vapour/extdata/gdal/sds.nc\":vv2"
#> [4] "SUBDATASET_2_DESC=[61x87] vv2 (64-bit floating-point)"


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
dirigible:::read_fields_gdal_cpp(osm, layer = 1, sql = "", limit_n = 0, skip_n = 0, ex = 0, fid_column_name = "")
#> $osm_id
#> [1] "100" "101" "102" "103" "104"
#> 
#> $name
#> [1] ""             "blue"         "River Thames" ""             ""            
#> 
#> $highway
#> [1] "footway"  "maybe"    ""         "cycleway" "cycleway"
#> 
#> $waterway
#> [1] "" "" "" "" ""
#> 
#> $aerialway
#> [1] "" "" "" "" ""
#> 
#> $barrier
#> [1] "" "" "" "" ""
#> 
#> $man_made
#> [1] "" "" "" "" ""
#> 
#> $z_order
#> [1] 0 0 0 0 0
#> 
#> $other_tags
#> [1] "\"layer\"=>\"0\""  ""                  "\"boat\"=>\"yes\""
#> [4] "\"foot\"=>\"yes\"" ""                 
#> 
#> [[10]]
#> NULL
## read proj info
names(dirigible:::projection_info_gdal_cpp(f, layer = 0, sql = ""))
#> [1] "Proj4"      "MICoordSys" "PrettyWkt"  "Wkt"        "EPSG"      
#> [6] "XML"

## read gcp
gcpfile <- system.file("extdata/gcps/volcano_gcp.tif", package = "vapour", mustWork = TRUE)
dirigible:::raster_gcp_gdal_cpp(gcpfile)
#> $Pixel
#> [1]  0  5 20
#> 
#> $Line
#> [1]  0  5 15
#> 
#> $X
#> [1] 100 200 300
#> 
#> $Y
#> [1] 100 200 300
#> 
#> $Z
#> [1] 0 0 0
#> 
#> $CRS
#> [1] ""

## raster info
dirigible:::raster_info_gdal_cpp(gcpfile, FALSE)
#> $geotransform
#> [1] 0 1 0 0 0 1
#> 
#> $dimXY
#> [1] 20 15
#> 
#> $minmax
#> [1] NA NA
#> 
#> $tilesXY
#> [1] 20 15
#> 
#> $projection
#> [1] ""
#> 
#> $bands
#> [1] 1
#> 
#> $proj4
#> [1] ""
#> 
#> $nodata_value
#> [1] -3.4e+38
#> 
#> $overviews
#> integer(0)
#> 
#> $filelist
#> [1] "/usr/local/lib/R/site-library/vapour/extdata/gcps/volcano_gcp.tif"
dirigible:::raster_info_gdal_cpp(gcpfile, TRUE)$minmax
#> [1] 100 163

## an error, but ok
try(dirigible:::raster_info_gdal_cpp(sdsfile, FALSE))
#> Error in dirigible:::raster_info_gdal_cpp(sdsfile, FALSE) : 
#>   no rasters found in dataset

## read raster
sstif <- system.file("extdata", "sst.tif", package = "vapour", mustWork = TRUE)
## a 5*5 window from a 10*10 region
dirigible:::raster_io_gdal_cpp(sstif, window = c(0, 0, 10, 10, 5, 5), band = 1, resample = "nearestneighbour")
#> [[1]]
#>  [1] 285.743 285.786 285.999 286.064 286.038 285.763 285.698 285.831 286.265
#> [10] 286.134 286.070 285.931 286.092 286.450 286.501 286.264 286.076 285.915
#> [19] 286.335 286.702 285.870 285.940 286.252 286.549 287.142
dirigible:::raster_io_gdal_cpp(sstif, window = c(0, 0, 10, 10, 5, 5), band = 1,  resample = "lanczos")
#> [[1]]
#>  [1] 285.7112 285.7088 285.9000 286.0153 285.9586 285.7220 285.7106 285.7733
#>  [9] 286.1091 286.2422 286.0036 285.8829 285.9075 286.3403 286.4126 286.2443
#> [17] 286.1346 285.9894 286.3176 286.6353 286.0227 285.9557 286.0615 286.3574
#> [25] 286.8593

## overviews
otif <- system.file("extdata", "volcano_overview.tif", package = "vapour", mustWork = TRUE)
matrix(dirigible:::raster_info_gdal_cpp(otif, FALSE)$overviews, ncol = 2, byrow = TRUE)
#>      [,1] [,2]
#> [1,]   31   44
#> [2,]   16   22
#> [3,]    8   11
#> [4,]    4    6

## there are none
matrix(dirigible:::raster_info_gdal_cpp(sstif, FALSE)$overviews, ncol = 2, byrow = TRUE)
#>      [,1] [,2]

## compression
dirigible:::raster_info_gdal_cpp(otif, FALSE)
#> $geotransform
#> [1]  0  1  0 87  0 -1
#> 
#> $dimXY
#> [1] 61 87
#> 
#> $minmax
#> [1] NA NA
#> 
#> $tilesXY
#> [1] 61 16
#> 
#> $projection
#> [1] ""
#> 
#> $bands
#> [1] 1
#> 
#> $proj4
#> [1] ""
#> 
#> $nodata_value
#> [1] -1.7e+308
#> 
#> $overviews
#> [1] 31 44 16 22  8 11  4  6
#> 
#> $filelist
#> [1] "/usr/local/lib/R/site-library/vapour/extdata/volcano_overview.tif"

## name of driver in use
dirigible:::driver_gdal_cpp(sstif)
#> [1] "GTiff"

# read geometry
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



f <- system.file("extdata/tab/list_locality_postcode_meander_valley.tab", package = "vapour")

dirigible:::geometry_cpp_limit_skip(f, 0, "", 0, "geometry", limit_n = 2, skip_n = 0)
#> [[1]]
#>     [1] 01 03 00 00 00 01 00 00 00 c1 04 00 00 14 ae 47 e1 78 bc 1b 41 48 e1 7a
#>    [25] b4 75 96 54 41 0a d7 a3 70 7b bc 1b 41 b8 1e 85 1b 75 96 54 41 29 5c 8f
#>    [49] c2 81 bc 1b 41 66 66 66 e6 73 96 54 41 66 66 66 66 8a bc 1b 41 c3 f5 28
#>    [73] 5c 72 96 54 41 85 eb 51 b8 c0 bc 1b 41 52 b8 1e a5 6b 96 54 41 8f c2 f5
#>    [97] 28 ce bc 1b 41 ae 47 e1 4a 69 96 54 41 5c 8f c2 f5 d2 bc 1b 41 85 eb 51
#>   [121] 98 68 96 54 41 ae 47 e1 7a d9 bc 1b 41 d7 a3 70 cd 67 96 54 41 7b 14 ae
#>   [145] 47 c8 c2 1b 41 f6 28 5c 0f 23 96 54 41 e1 7a 14 ae 3a ca 1b 41 9a 99 99
#>   [169] f9 ca 95 54 41 8f c2 f5 28 62 ca 1b 41 14 ae 47 31 c9 95 54 41 71 3d 0a
#>   [193] d7 7b ca 1b 41 00 00 00 00 c8 95 54 41 f6 28 5c 8f 84 ca 1b 41 85 eb 51
#>   [217] 98 c7 95 54 41 8f c2 f5 28 96 cc 1b 41 1f 85 eb 01 af 95 54 41 c3 f5 28
#>   [241] 5c b1 d4 1b 41 8f c2 f5 c8 4f 95 54 41 3d 0a d7 a3 5b d8 1b 41 48 e1 7a
#>   [265] c4 24 95 54 41 9a 99 99 99 de da 1b 41 85 eb 51 58 07 95 54 41 3d 0a d7
#>   [289] a3 d3 de 1b 41 9a 99 99 59 d0 94 54 41 5c 8f c2 f5 ae e2 1b 41 c3 f5 28
#>   [313] 3c a1 94 54 41 ae 47 e1 7a a6 e4 1b 41 a4 70 3d fa 8a 94 54 41 cd cc cc
#>   [337] cc 8e ef 1b 41 33 33 33 c3 0f 94 54 41 c3 f5 28 5c a7 ef 1b 41 00 00 00
#>   [361] b0 0e 94 54 41 c3 f5 28 5c 4f ef 1b 41 00 00 00 b0 f8 93 54 41 f6 28 5c
#>   [385] 8f 24 ef 1b 41 f6 28 5c 2f ee 93 54 41 5c 8f c2 f5 00 ef 1b 41 f6 28 5c
#>   [409] 0f e8 93 54 41 9a 99 99 99 be ee 1b 41 f6 28 5c cf dc 93 54 41 e1 7a 14
#>   [433] ae 65 ee 1b 41 1f 85 eb d1 cc 93 54 41 ec 51 b8 1e 62 ee 1b 41 52 b8 1e
#>   [457] 85 cb 93 54 41 14 ae 47 e1 54 ee 1b 41 33 33 33 13 c8 93 54 41 ec 51 b8
#>   [481] 1e 45 ee 1b 41 66 66 66 d6 c6 93 54 41 48 e1 7a 14 29 ee 1b 41 d7 a3 70
#>   [505] ad c3 93 54 41 5c 8f c2 f5 10 ee 1b 41 33 33 33 13 c1 93 54 41 8f c2 f5
#>   [529] 28 0e ee 1b 41 b8 1e 85 bb c0 93 54 41 85 eb 51 b8 04 ee 1b 41 52 b8 1e
#>   [553] 95 bf 93 54 41 a4 70 3d 0a fb ed 1b 41 b8 1e 85 3b be 93 54 41 85 eb 51
#>   [577] b8 f5 ed 1b 41 66 66 66 d6 bc 93 54 41 14 ae 47 e1 f2 ed 1b 41 52 b8 1e
#>   [601] 75 ba 93 54 41 52 b8 1e 85 f2 ed 1b 41 7b 14 ae 57 ba 93 54 41 71 3d 0a
#>   [625] d7 e4 ed 1b 41 71 3d 0a 07 b6 93 54 41 0a d7 a3 70 db ed 1b 41 33 33 33
#>   [649] 13 b4 93 54 41 71 3d 0a d7 d4 ed 1b 41 7b 14 ae 37 b3 93 54 41 c3 f5 28
#>   [673] 5c b9 ed 1b 41 c3 f5 28 2c b0 93 54 41 1f 85 eb 51 7f ed 1b 41 29 5c 8f
#>   [697] c2 aa 93 54 41 ae 47 e1 7a 79 ed 1b 41 29 5c 8f e2 a9 93 54 41 1f 85 eb
#>   [721] 51 6f ed 1b 41 e1 7a 14 ce a7 93 54 41 3d 0a d7 a3 67 ed 1b 41 85 eb 51
#>   [745] 58 a5 93 54 41 71 3d 0a d7 63 ed 1b 41 e1 7a 14 ae a3 93 54 41 29 5c 8f
#>   [769] c2 63 ed 1b 41 14 ae 47 a1 9c 93 54 41 7b 14 ae 47 68 ed 1b 41 ec 51 b8
#>   [793] fe 98 93 54 41 71 3d 0a d7 64 ed 1b 41 ec 51 b8 2e 93 93 54 41 14 ae 47
#>   [817] e1 63 ed 1b 41 0a d7 a3 90 91 93 54 41 14 ae 47 e1 64 ed 1b 41 00 00 00
#>   [841] 50 8f 93 54 41 b8 1e 85 eb 64 ed 1b 41 f6 28 5c 4f 8b 93 54 41 0a d7 a3
#>   [865] 70 7d ed 1b 41 f6 28 5c 2f 86 93 54 41 52 b8 1e 85 78 ed 1b 41 f6 28 5c
#>   [889] ef 7d 93 54 41 a4 70 3d 0a 96 ed 1b 41 ec 51 b8 ee 73 93 54 41 33 33 33
#>   [913] 33 81 ed 1b 41 f6 28 5c 8f 6d 93 54 41 cd cc cc cc f2 ec 1b 41 f6 28 5c
#>   [937] af 60 93 54 41 a4 70 3d 0a 92 ec 1b 41 f6 28 5c 4f 51 93 54 41 f6 28 5c
#>   [961] 8f 8f ec 1b 41 f6 28 5c 6f 48 93 54 41 b8 1e 85 eb 8f ec 1b 41 f6 28 5c
#>   [985] af 45 93 54 41 b8 1e 85 eb 91 ec 1b 41 f6 28 5c ef 34 93 54 41 f6 28 5c
#>  [1009] 8f 8f ec 1b 41 f6 28 5c cf 31 93 54 41 48 e1 7a 14 90 ec 1b 41 7b 14 ae
#>  [1033] a7 27 93 54 41 d7 a3 70 3d 8f ec 1b 41 71 3d 0a c7 1e 93 54 41 00 00 00
#>  [1057] 00 8f ec 1b 41 7b 14 ae e7 1d 93 54 41 d7 a3 70 3d 78 ec 1b 41 29 5c 8f
#>  [1081] b2 12 93 54 41 c3 f5 28 5c 73 ec 1b 41 3d 0a d7 13 0a 93 54 41 b8 1e 85
#>  [1105] eb 6e ec 1b 41 14 ae 47 51 02 93 54 41 00 00 00 00 68 ec 1b 41 5c 8f c2
#>  [1129] a5 fd 92 54 41 a4 70 3d 0a 61 ec 1b 41 00 00 00 40 f3 92 54 41 ec 51 b8
#>  [1153] 1e 59 ec 1b 41 8f c2 f5 68 ef 92 54 41 29 5c 8f c2 54 ec 1b 41 ec 51 b8
#>  [1177] ee ed 92 54 41 29 5c 8f c2 2a ec 1b 41 48 e1 7a 54 e5 92 54 41 33 33 33
#>  [1201] 33 27 ec 1b 41 8f c2 f5 78 e4 92 54 41 8f c2 f5 28 26 ec 1b 41 33 33 33
#>  [1225] 03 e4 92 54 41 7b 14 ae 47 21 ec 1b 41 48 e1 7a 54 de 92 54 41 ae 47 e1
#>  [1249] 7a 1e ec 1b 41 48 e1 7a 64 dd 92 54 41 66 66 66 66 0e ec 1b 41 c3 f5 28
#>  [1273] 4c d9 92 54 41 ec 51 b8 1e 0b ec 1b 41 b8 1e 85 3b d8 92 54 41 9a 99 99
#>  [1297] 99 0a ec 1b 41 85 eb 51 68 d7 92 54 41 7b 14 ae 47 0d ec 1b 41 b8 1e 85
#>  [1321] 2b d0 92 54 41 3d 0a d7 a3 14 ec 1b 41 ec 51 b8 0e ce 92 54 41 d7 a3 70
#>  [1345] 3d 1a ec 1b 41 1f 85 eb d1 cc 92 54 41 c3 f5 28 5c 23 ec 1b 41 d7 a3 70
#>  [1369] bd cb 92 54 41 85 eb 51 b8 39 ec 1b 41 b8 1e 85 5b c9 92 54 41 3d 0a d7
#>  [1393] a3 44 ec 1b 41 00 00 00 90 c8 92 54 41 7b 14 ae 47 5f ec 1b 41 e1 7a 14
#>  [1417] 1e c7 92 54 41 8f c2 f5 28 c4 ec 1b 41 29 5c 8f 72 c1 92 54 41 85 eb 51
#>  [1441] b8 c9 ec 1b 41 3d 0a d7 c3 c0 92 54 41 ae 47 e1 7a da ec 1b 41 e1 7a 14
#>  [1465] 5e bf 92 54 41 c3 f5 28 5c e2 ec 1b 41 71 3d 0a 97 be 92 54 41 e1 7a 14
#>  [1489] ae f7 ec 1b 41 b8 1e 85 cb bb 92 54 41 7b 14 ae 47 ff ec 1b 41 7b 14 ae
#>  [1513] e7 ba 92 54 41 f6 28 5c 8f 31 ed 1b 41 33 33 33 c3 b6 92 54 41 8f c2 f5
#>  [1537] 28 37 ed 1b 41 85 eb 51 18 b6 92 54 41 a4 70 3d 0a 42 ed 1b 41 5c 8f c2
#>  [1561] 85 b3 92 54 41 cd cc cc cc 42 ed 1b 41 9a 99 99 89 b2 92 54 41 a4 70 3d
#>  [1585] 0a 41 ed 1b 41 e1 7a 14 8e b1 92 54 41 f6 28 5c 8f 35 ed 1b 41 0a d7 a3
#>  [1609] 50 ae 92 54 41 d7 a3 70 3d 33 ed 1b 41 5c 8f c2 75 ad 92 54 41 71 3d 0a
#>  [1633] d7 2d ed 1b 41 c3 f5 28 3c a7 92 54 41 52 b8 1e 85 29 ed 1b 41 7b 14 ae
#>  [1657] 17 a6 92 54 41 9a 99 99 99 f7 ec 1b 41 cd cc cc 4c 9c 92 54 41 7b 14 ae
#>  [1681] 47 f5 ec 1b 41 52 b8 1e 75 9b 92 54 41 85 eb 51 b8 f1 ec 1b 41 5c 8f c2
#>  [1705] a5 98 92 54 41 d7 a3 70 3d f3 ec 1b 41 33 33 33 f3 97 92 54 41 ec 51 b8
#>  [1729] 1e fe ec 1b 41 85 eb 51 58 96 92 54 41 5c 8f c2 f5 01 ed 1b 41 29 5c 8f
#>  [1753] f2 95 92 54 41 a4 70 3d 0a 07 ed 1b 41 1f 85 eb 91 95 92 54 41 d7 a3 70
#>  [1777] 3d 22 ed 1b 41 ec 51 b8 ee 93 92 54 41 00 00 00 00 84 ed 1b 41 00 00 00
#>  [1801] 70 8e 92 54 41 48 e1 7a 14 8a ed 1b 41 d7 a3 70 ed 8d 92 54 41 ec 51 b8
#>  [1825] 1e 9c ed 1b 41 1f 85 eb 01 8c 92 54 41 e1 7a 14 ae a0 ed 1b 41 66 66 66
#>  [1849] 36 8b 92 54 41 5c 8f c2 f5 b3 ed 1b 41 5c 8f c2 f5 86 92 54 41 33 33 33
#>  [1873] 33 b7 ed 1b 41 7b 14 ae 97 83 92 54 41 7b 14 ae 47 bd ed 1b 41 48 e1 7a
#>  [1897] d4 80 92 54 41 71 3d 0a d7 c0 ed 1b 41 48 e1 7a d4 7f 92 54 41 66 66 66
#>  [1921] 66 c6 ed 1b 41 c3 f5 28 cc 7e 92 54 41 ae 47 e1 7a cb ed 1b 41 5c 8f c2
#>  [1945] 45 7e 92 54 41 f6 28 5c 8f ef ed 1b 41 e1 7a 14 2e 7c 92 54 41 1f 85 eb
#>  [1969] 51 01 ee 1b 41 71 3d 0a 87 7a 92 54 41 8f c2 f5 28 05 ee 1b 41 3d 0a d7
#>  [1993] c3 79 92 54 41 1f 85 eb 51 0c ee 1b 41 f6 28 5c 5f 77 92 54 41 d7 a3 70
#>  [2017] 3d 0e ee 1b 41 b8 1e 85 bb 76 92 54 41 cd cc cc cc 11 ee 1b 41 7b 14 ae
#>  [2041] d7 75 92 54 41 33 33 33 33 1a ee 1b 41 ae 47 e1 aa 74 92 54 41 9a 99 99
#>  [2065] 99 24 ee 1b 41 ec 51 b8 9e 73 92 54 41 66 66 66 66 29 ee 1b 41 c3 f5 28
#>  [2089] fc 72 92 54 41 52 b8 1e 85 63 ee 1b 41 00 00 00 20 6a 92 54 41 7b 14 ae
#>  [2113] 47 65 ee 1b 41 66 66 66 b6 69 92 54 41 7b 14 ae 47 66 ee 1b 41 3d 0a d7
#>  [2137] 23 68 92 54 41 00 00 00 00 66 ee 1b 41 85 eb 51 18 64 92 54 41 29 5c 8f
#>  [2161] c2 66 ee 1b 41 85 eb 51 48 62 92 54 41 52 b8 1e 85 68 ee 1b 41 1f 85 eb
#>  [2185] 91 61 92 54 41 85 eb 51 b8 78 ee 1b 41 7b 14 ae 27 5d 92 54 41 52 b8 1e
#>  [2209] 85 7a ee 1b 41 33 33 33 23 5c 92 54 41 d7 a3 70 3d 7a ee 1b 41 00 00 00
#>  [2233] 40 5b 92 54 41 8f c2 f5 28 76 ee 1b 41 ec 51 b8 de 57 92 54 41 d7 a3 70
#>  [2257] 3d 6e ee 1b 41 d7 a3 70 ad 55 92 54 41 48 e1 7a 14 68 ee 1b 41 71 3d 0a
#>  [2281] 17 50 92 54 41 85 eb 51 b8 62 ee 1b 41 33 33 33 33 4f 92 54 41 9a 99 99
#>  [2305] 99 40 ee 1b 41 85 eb 51 48 4b 92 54 41 00 00 00 00 39 ee 1b 41 85 eb 51
#>  [2329] 48 4a 92 54 41 0a d7 a3 70 36 ee 1b 41 52 b8 1e 85 49 92 54 41 33 33 33
#>  [2353] 33 2a ee 1b 41 b8 1e 85 3b 43 92 54 41 5c 8f c2 f5 1e ee 1b 41 8f c2 f5
#>  [2377] 88 3f 92 54 41 3d 0a d7 a3 1b ee 1b 41 9a 99 99 b9 3e 92 54 41 b8 1e 85
#>  [2401] eb 0f ee 1b 41 29 5c 8f 02 3d 92 54 41 9a 99 99 99 0a ee 1b 41 48 e1 7a
#>  [2425] 64 3c 92 54 41 8f c2 f5 28 c5 ed 1b 41 29 5c 8f f2 35 92 54 41 48 e1 7a
#>  [2449] 14 c1 ed 1b 41 cd cc cc 7c 35 92 54 41 7b 14 ae 47 bf ed 1b 41 33 33 33
#>  [2473] 23 35 92 54 41 b8 1e 85 eb b7 ed 1b 41 29 5c 8f e2 32 92 54 41 66 66 66
#>  [2497] 66 b6 ed 1b 41 7b 14 ae 17 32 92 54 41 1f 85 eb 51 b6 ed 1b 41 29 5c 8f
#>  [2521] 02 2c 92 54 41 52 b8 1e 85 b5 ed 1b 41 ec 51 b8 1e 2a 92 54 41 33 33 33
#>  [2545] 33 b2 ed 1b 41 ae 47 e1 2a 29 92 54 41 71 3d 0a d7 ab ed 1b 41 d7 a3 70
#>  [2569] bd 27 92 54 41 29 5c 8f c2 a7 ed 1b 41 b8 1e 85 1b 27 92 54 41 71 3d 0a
#>  [2593] d7 9c ed 1b 41 a4 70 3d ea 25 92 54 41 b8 1e 85 eb 93 ed 1b 41 b8 1e 85
#>  [2617] 1b 25 92 54 41 f6 28 5c 8f 7e ed 1b 41 0a d7 a3 70 23 92 54 41 3d 0a d7
#>  [2641] a3 73 ed 1b 41 5c 8f c2 b5 22 92 54 41 ae 47 e1 7a 6b ed 1b 41 3d 0a d7
#>  [2665] 43 22 92 54 41 9a 99 99 99 63 ed 1b 41 29 5c 8f f2 21 92 54 41 85 eb 51
#>  [2689] b8 11 ed 1b 41 3d 0a d7 33 1f 92 54 41 8f c2 f5 28 0d ed 1b 41 33 33 33
#>  [2713] f3 1e 92 54 41 29 5c 8f c2 04 ed 1b 41 c3 f5 28 4c 1e 92 54 41 00 00 00
#>  [2737] 00 f5 ec 1b 41 71 3d 0a e7 1c 92 54 41 8f c2 f5 28 f1 ec 1b 41 8f c2 f5
#>  [2761] 68 1c 92 54 41 14 ae 47 e1 ef ec 1b 41 ae 47 e1 ea 1b 92 54 41 66 66 66
#>  [2785] 66 f0 ec 1b 41 85 eb 51 18 1a 92 54 41 b8 1e 85 eb f1 ec 1b 41 ec 51 b8
#>  [2809] ce 19 92 54 41 85 eb 51 b8 f5 ec 1b 41 52 b8 1e 75 19 92 54 41 c3 f5 28
#>  [2833] 5c ff ec 1b 41 ae 47 e1 ca 18 92 54 41 33 33 33 33 03 ed 1b 41 e1 7a 14
#>  [2857] 9e 18 92 54 41 00 00 00 00 08 ed 1b 41 14 ae 47 81 18 92 54 41 0a d7 a3
#>  [2881] 70 50 ed 1b 41 0a d7 a3 70 18 92 54 41 a4 70 3d 0a 56 ed 1b 41 3d 0a d7
#>  [2905] 53 18 92 54 41 9a 99 99 99 59 ed 1b 41 f6 28 5c 1f 18 92 54 41 c3 f5 28
#>  [2929] 5c 5c ed 1b 41 8f c2 f5 98 17 92 54 41 c3 f5 28 5c 5c ed 1b 41 7b 14 ae
#>  [2953] 47 17 92 54 41 1f 85 eb 51 59 ed 1b 41 e1 7a 14 ee 15 92 54 41 29 5c 8f
#>  [2977] c2 56 ed 1b 41 7b 14 ae 47 15 92 54 41 0a d7 a3 70 44 ed 1b 41 ec 51 b8
#>  [3001] fe 11 92 54 41 a4 70 3d 0a 3e ed 1b 41 33 33 33 23 11 92 54 41 9a 99 99
#>  [3025] 99 32 ed 1b 41 14 ae 47 d1 0f 92 54 41 33 33 33 33 29 ed 1b 41 66 66 66
#>  [3049] f6 0e 92 54 41 e1 7a 14 ae 0a ed 1b 41 0a d7 a3 70 0c 92 54 41 14 ae 47
#>  [3073] e1 cb ec 1b 41 66 66 66 d6 07 92 54 41 00 00 00 00 c6 ec 1b 41 48 e1 7a
#>  [3097] 54 07 92 54 41 85 eb 51 b8 b4 ec 1b 41 c3 f5 28 4c 05 92 54 41 c3 f5 28
#>  [3121] 5c af ec 1b 41 7b 14 ae 47 04 92 54 41 48 e1 7a 14 ae ec 1b 41 1f 85 eb
#>  [3145] c1 03 92 54 41 9a 99 99 99 ae ec 1b 41 14 ae 47 b1 02 92 54 41 3d 0a d7
#>  [3169] a3 b3 ec 1b 41 0a d7 a3 70 fe 91 54 41 ec 51 b8 1e b6 ec 1b 41 48 e1 7a
#>  [3193] 54 fd 91 54 41 85 eb 51 b8 ba ec 1b 41 8f c2 f5 98 fc 91 54 41 3d 0a d7
#>  [3217] a3 e6 ec 1b 41 9a 99 99 e9 f6 91 54 41 14 ae 47 e1 e8 ec 1b 41 7b 14 ae
#>  [3241] 77 f6 91 54 41 3d 0a d7 a3 e9 ec 1b 41 cd cc cc 9c f5 91 54 41 ae 47 e1
#>  [3265] 7a e5 ec 1b 41 33 33 33 53 ee 91 54 41 a4 70 3d 0a e6 ec 1b 41 b8 1e 85
#>  [3289] 7b ed 91 54 41 00 00 00 00 e7 ec 1b 41 a4 70 3d 1a ed 91 54 41 66 66 66
#>  [3313] 66 ef ec 1b 41 b8 1e 85 7b eb 91 54 41 d7 a3 70 3d f6 ec 1b 41 71 3d 0a
#>  [3337] 67 ea 91 54 41 1f 85 eb 51 1c ed 1b 41 1f 85 eb 21 e6 91 54 41 a4 70 3d
#>  [3361] 0a 2b ed 1b 41 8f c2 f5 f8 e2 91 54 41 f6 28 5c 8f 2c ed 1b 41 3d 0a d7
#>  [3385] b3 e1 91 54 41 7b 14 ae 47 2e ed 1b 41 48 e1 7a d4 dc 91 54 41 00 00 00
#>  [3409] 00 2d ed 1b 41 1f 85 eb 31 dc 91 54 41 d7 a3 70 3d 1f ed 1b 41 a4 70 3d
#>  [3433] 5a d8 91 54 41 8f c2 f5 28 1b ed 1b 41 33 33 33 83 d7 91 54 41 a4 70 3d
#>  [3457] 0a f8 ec 1b 41 3d 0a d7 c3 d2 91 54 41 14 ae 47 e1 e8 ec 1b 41 b8 1e 85
#>  [3481] 5b d1 91 54 41 cd cc cc cc d4 ec 1b 41 e1 7a 14 7e cf 91 54 41 ec 51 b8
#>  [3505] 1e b4 ec 1b 41 33 33 33 73 cc 91 54 41 7b 14 ae 47 af ec 1b 41 52 b8 1e
#>  [3529] e5 cb 91 54 41 8f c2 f5 28 9a ec 1b 41 52 b8 1e f5 c8 91 54 41 7b 14 ae
#>  [3553] 47 93 ec 1b 41 1f 85 eb 31 c8 91 54 41 0a d7 a3 70 44 ec 1b 41 a4 70 3d
#>  [3577] 0a c2 91 54 41 48 e1 7a 14 3f ec 1b 41 7b 14 ae 77 c1 91 54 41 29 5c 8f
#>  [3601] c2 3a ec 1b 41 d7 a3 70 dd c0 91 54 41 29 5c 8f c2 39 ec 1b 41 3d 0a d7
#>  [3625] 83 c0 91 54 41 0a d7 a3 70 38 ec 1b 41 66 66 66 26 be 91 54 41 ae 47 e1
#>  [3649] 7a 3a ec 1b 41 7b 14 ae 97 bd 91 54 41 52 b8 1e 85 4f ec 1b 41 33 33 33
#>  [3673] 73 ba 91 54 41 9a 99 99 99 53 ec 1b 41 33 33 33 83 b9 91 54 41 9a 99 99
#>  [3697] 99 53 ec 1b 41 85 eb 51 a8 b7 91 54 41 7b 14 ae 47 52 ec 1b 41 e1 7a 14
#>  [3721] 2e b7 91 54 41 85 eb 51 b8 4d ec 1b 41 b8 1e 85 8b b6 91 54 41 52 b8 1e
#>  [3745] 85 33 ec 1b 41 85 eb 51 b8 b4 91 54 41 66 66 66 66 2d ec 1b 41 33 33 33
#>  [3769] 63 b4 91 54 41 b8 1e 85 eb 0e ec 1b 41 71 3d 0a 57 b3 91 54 41 f6 28 5c
#>  [3793] 8f 08 ec 1b 41 1f 85 eb 01 b3 91 54 41 3d 0a d7 a3 f0 eb 1b 41 52 b8 1e
#>  [3817] f5 b0 91 54 41 71 3d 0a d7 ed eb 1b 41 00 00 00 a0 b0 91 54 41 f6 28 5c
#>  [3841] 8f ec eb 1b 41 ae 47 e1 4a b0 91 54 41 cd cc cc cc ec eb 1b 41 f6 28 5c
#>  [3865] 6f af 91 54 41 9a 99 99 99 f0 eb 1b 41 1f 85 eb 31 ad 91 54 41 14 ae 47
#>  [3889] e1 f3 eb 1b 41 7b 14 ae a7 ac 91 54 41 71 3d 0a d7 10 ec 1b 41 c3 f5 28
#>  [3913] ac a9 91 54 41 3d 0a d7 a3 14 ec 1b 41 1f 85 eb 21 a9 91 54 41 d7 a3 70
#>  [3937] 3d 1b ec 1b 41 0a d7 a3 d0 a7 91 54 41 b8 1e 85 eb 18 ec 1b 41 1f 85 eb
#>  [3961] 51 a4 91 54 41 c3 f5 28 5c 16 ec 1b 41 7b 14 ae a7 a3 91 54 41 f6 28 5c
#>  [3985] 8f 11 ec 1b 41 48 e1 7a e4 a2 91 54 41 f6 28 5c 8f d9 eb 1b 41 66 66 66
#>  [4009] 46 9d 91 54 41 ec 51 b8 1e cd eb 1b 41 9a 99 99 19 9c 91 54 41 3d 0a d7
#>  [4033] a3 69 eb 1b 41 ec 51 b8 5e 94 91 54 41 33 33 33 33 15 eb 1b 41 ae 47 e1
#>  [4057] 4a 8e 91 54 41 ae 47 e1 7a 08 eb 1b 41 ec 51 b8 1e 8d 91 54 41 48 e1 7a
#>  [4081] 14 c7 ea 1b 41 48 e1 7a 74 86 91 54 41 48 e1 7a 14 a9 ea 1b 41 85 eb 51
#>  [4105] 88 83 91 54 41 a4 70 3d 0a 99 ea 1b 41 33 33 33 43 82 91 54 41 c3 f5 28
#>  [4129] 5c 71 ea 1b 41 48 e1 7a 84 7f 91 54 41 f6 28 5c 8f 5e ea 1b 41 3d 0a d7
#>  [4153] 53 7e 91 54 41 8f c2 f5 28 57 ea 1b 41 9a 99 99 c9 7d 91 54 41 48 e1 7a
#>  [4177] 14 17 ea 1b 41 33 33 33 13 78 91 54 41 e1 7a 14 ae 0e ea 1b 41 52 b8 1e
#>  [4201] 85 76 91 54 41 1f 85 eb 51 f9 e9 1b 41 8f c2 f5 78 73 91 54 41 a4 70 3d
#>  [4225] 0a ce e9 1b 41 85 eb 51 38 6e 91 54 41 e1 7a 14 ae c8 e9 1b 41 66 66 66
#>  [4249] b6 6d 91 54 41 1f 85 eb 51 a4 e9 1b 41 3d 0a d7 13 6b 91 54 41 66 66 66
#>  [4273] 66 99 e9 1b 41 0a d7 a3 60 6a 91 54 41 1f 85 eb 51 86 e9 1b 41 9a 99 99
#>  [4297] 89 69 91 54 41 29 5c 8f c2 47 e9 1b 41 85 eb 51 38 67 91 54 41 66 66 66
#>  [4321] 66 42 e9 1b 41 7b 14 ae f7 66 91 54 41 48 e1 7a 14 3e e9 1b 41 e1 7a 14
#>  [4345] ae 66 91 54 41 d7 a3 70 3d 28 e9 1b 41 66 66 66 c6 64 91 54 41 66 66 66
#>  [4369] 66 22 e9 1b 41 7b 14 ae 17 64 91 54 41 3d 0a d7 a3 10 e9 1b 41 7b 14 ae
#>  [4393] 57 60 91 54 41 0a d7 a3 70 d4 e4 1b 41 66 66 66 36 93 91 54 41 7b 14 ae
#>  [4417] 47 91 e2 1b 41 8f c2 f5 38 45 91 54 41 ae 47 e1 7a 80 e2 1b 41 48 e1 7a
#>  [4441] f4 42 91 54 41 85 eb 51 b8 3f e2 1b 41 00 00 00 f0 44 91 54 41 71 3d 0a
#>  [4465] d7 50 db 1b 41 9a 99 99 79 8d 91 54 41 ae 47 e1 7a 64 dc 1b 41 ec 51 b8
#>  [4489] 8e f8 91 54 41 00 00 00 00 95 d1 1b 41 ec 51 b8 7e 79 92 54 41 71 3d 0a
#>  [4513] d7 a5 d1 1b 41 8f c2 f5 88 77 92 54 41 8f c2 f5 28 a9 d1 1b 41 ec 51 b8
#>  [4537] ee 76 92 54 41 e1 7a 14 ae ab d1 1b 41 7b 14 ae 37 76 92 54 41 00 00 00
#>  [4561] 00 b2 d1 1b 41 33 33 33 23 71 92 54 41 5c 8f c2 f5 b0 d1 1b 41 14 ae 47
#>  [4585] a1 70 92 54 41 00 00 00 00 a6 d1 1b 41 a4 70 3d da 6e 92 54 41 f6 28 5c
#>  [4609] 8f 99 d1 1b 41 5c 8f c2 a5 6d 92 54 41 71 3d 0a d7 6f d1 1b 41 5c 8f c2
#>  [4633] b5 6a 92 54 41 5c 8f c2 f5 66 d1 1b 41 c3 f5 28 4c 6a 92 54 41 7b 14 ae
#>  [4657] 47 04 d1 1b 41 52 b8 1e 85 66 92 54 41 00 00 00 00 f3 d0 1b 41 ec 51 b8
#>  [4681] ce 65 92 54 41 ec 51 b8 1e ed d0 1b 41 cd cc cc 6c 65 92 54 41 7b 14 ae
#>  [4705] 47 c9 d0 1b 41 14 ae 47 51 62 92 54 41 5c 8f c2 f5 c4 d0 1b 41 33 33 33
#>  [4729] d3 61 92 54 41 8f c2 f5 28 c2 d0 1b 41 48 e1 7a 34 61 92 54 41 48 e1 7a
#>  [4753] 14 bf d0 1b 41 5c 8f c2 95 5f 92 54 41 48 e1 7a 14 bf d0 1b 41 ae 47 e1
#>  [4777] da 5e 92 54 41 ec 51 b8 1e c1 d0 1b 41 33 33 33 f3 5c 92 54 41 71 3d 0a
#>  [4801] d7 c2 d0 1b 41 d7 a3 70 6d 5c 92 54 41 66 66 66 66 c6 d0 1b 41 ec 51 b8
#>  [4825] de 5b 92 54 41 0a d7 a3 70 d8 d0 1b 41 d7 a3 70 9d 59 92 54 41 f6 28 5c
#>  [4849] 8f df d0 1b 41 71 3d 0a 17 59 92 54 41 cd cc cc cc ef d0 1b 41 f6 28 5c
#>  [4873] 0f 58 92 54 41 ec 51 b8 1e f3 d0 1b 41 5c 8f c2 a5 57 92 54 41 9a 99 99
#>  [4897] 99 f3 d0 1b 41 3d 0a d7 33 57 92 54 41 9a 99 99 99 f2 d0 1b 41 ae 47 e1
#>  [4921] ea 56 92 54 41 33 33 33 33 eb d0 1b 41 ec 51 b8 ee 55 92 54 41 e1 7a 14
#>  [4945] ae e7 d0 1b 41 9a 99 99 99 55 92 54 41 52 b8 1e 85 e1 d0 1b 41 c3 f5 28
#>  [4969] 4c 55 92 54 41 85 eb 51 b8 be d0 1b 41 48 e1 7a 54 54 92 54 41 f6 28 5c
#>  [4993] 8f b5 d0 1b 41 c3 f5 28 2c 54 92 54 41 cd cc cc cc 48 d0 1b 41 5c 8f c2
#>  [5017] 65 53 92 54 41 14 ae 47 e1 40 d0 1b 41 14 ae 47 41 53 92 54 41 5c 8f c2
#>  [5041] f5 34 d0 1b 41 7b 14 ae e7 52 92 54 41 1f 85 eb 51 2e d0 1b 41 ec 51 b8
#>  [5065] 9e 52 92 54 41 52 b8 1e 85 29 d0 1b 41 0a d7 a3 30 52 92 54 41 66 66 66
#>  [5089] 66 23 d0 1b 41 9a 99 99 69 51 92 54 41 3d 0a d7 a3 21 d0 1b 41 33 33 33
#>  [5113] c3 50 92 54 41 3d 0a d7 a3 24 d0 1b 41 52 b8 1e 55 4e 92 54 41 8f c2 f5
#>  [5137] 28 27 d0 1b 41 1f 85 eb 91 4d 92 54 41 d7 a3 70 3d 2b d0 1b 41 0a d7 a3
#>  [5161] 30 4d 92 54 41 e1 7a 14 ae 37 d0 1b 41 8f c2 f5 68 4c 92 54 41 f6 28 5c
#>  [5185] 8f 40 d0 1b 41 3d 0a d7 03 4c 92 54 41 cd cc cc cc 7e d0 1b 41 d7 a3 70
#>  [5209] 6d 4a 92 54 41 00 00 00 00 8a d0 1b 41 3d 0a d7 13 4a 92 54 41 66 66 66
#>  [5233] 66 93 d0 1b 41 8f c2 f5 78 49 92 54 41 a4 70 3d 0a 9d d0 1b 41 9a 99 99
#>  [5257] a9 48 92 54 41 71 3d 0a d7 9f d0 1b 41 c3 f5 28 4c 48 92 54 41 7b 14 ae
#>  [5281] 47 9f d0 1b 41 48 e1 7a 74 44 92 54 41 f6 28 5c 8f 94 d0 1b 41 f6 28 5c
#>  [5305] ff 3e 92 54 41 7b 14 ae 47 94 d0 1b 41 3d 0a d7 23 3e 92 54 41 48 e1 7a
#>  [5329] 14 97 d0 1b 41 48 e1 7a 64 3d 92 54 41 b8 1e 85 eb 9e d0 1b 41 29 5c 8f
#>  [5353] e2 3b 92 54 41 cd cc cc cc a5 d0 1b 41 c3 f5 28 4c 3b 92 54 41 33 33 33
#>  [5377] 33 bf d0 1b 41 85 eb 51 78 39 92 54 41 d7 a3 70 3d c3 d0 1b 41 29 5c 8f
#>  [5401] 12 39 92 54 41 cd cc cc cc c4 d0 1b 41 d7 a3 70 cd 38 92 54 41 f6 28 5c
#>  [5425] 8f c5 d0 1b 41 b8 1e 85 5b 38 92 54 41 00 00 00 00 c3 d0 1b 41 14 ae 47
#>  [5449] e1 35 92 54 41 33 33 33 33 c0 d0 1b 41 7b 14 ae 67 35 92 54 41 71 3d 0a
#>  [5473] d7 ac d0 1b 41 0a d7 a3 70 33 92 54 41 29 5c 8f c2 a8 d0 1b 41 e1 7a 14
#>  [5497] ce 32 92 54 41 48 e1 7a 14 91 d0 1b 41 85 eb 51 78 2e 92 54 41 a4 70 3d
#>  [5521] 0a 8e d0 1b 41 ae 47 e1 2a 2e 92 54 41 cd cc cc cc 7f d0 1b 41 7b 14 ae
#>  [5545] 47 2d 92 54 41 71 3d 0a d7 73 d0 1b 41 d7 a3 70 ad 2c 92 54 41 85 eb 51
#>  [5569] b8 6b d0 1b 41 3d 0a d7 63 2c 92 54 41 14 ae 47 e1 4a d0 1b 41 c3 f5 28
#>  [5593] 6c 2b 92 54 41 85 eb 51 b8 3f d0 1b 41 29 5c 8f 02 2b 92 54 41 3d 0a d7
#>  [5617] a3 3b d0 1b 41 9a 99 99 c9 2a 92 54 41 d7 a3 70 3d 24 d0 1b 41 a4 70 3d
#>  [5641] fa 28 92 54 41 5c 8f c2 f5 21 d0 1b 41 14 ae 47 b1 28 92 54 41 0a d7 a3
#>  [5665] 70 1f d0 1b 41 ae 47 e1 1a 28 92 54 41 71 3d 0a d7 1c d0 1b 41 33 33 33
#>  [5689] f3 26 92 54 41 71 3d 0a d7 1d d0 1b 41 66 66 66 e6 24 92 54 41 8f c2 f5
#>  [5713] 28 20 d0 1b 41 b8 1e 85 2b 24 92 54 41 14 ae 47 e1 30 d0 1b 41 e1 7a 14
#>  [5737] ee 20 92 54 41 3d 0a d7 a3 31 d0 1b 41 cd cc cc 8c 20 92 54 41 48 e1 7a
#>  [5761] 14 30 d0 1b 41 5c 8f c2 d5 1d 92 54 41 f6 28 5c 8f 2e d0 1b 41 f6 28 5c
#>  [5785] 4f 1d 92 54 41 d7 a3 70 3d 2a d0 1b 41 9a 99 99 c9 1c 92 54 41 85 eb 51
#>  [5809] b8 1b d0 1b 41 48 e1 7a 94 1b 92 54 41 8f c2 f5 28 17 d0 1b 41 7b 14 ae
#>  [5833] 57 1b 92 54 41 52 b8 1e 85 0f d0 1b 41 ec 51 b8 1e 1b 92 54 41 f6 28 5c
#>  [5857] 8f 02 d0 1b 41 29 5c 8f e2 1a 92 54 41 71 3d 0a d7 b8 cf 1b 41 f6 28 5c
#>  [5881] 0f 1a 92 54 41 48 e1 7a 14 99 cf 1b 41 c3 f5 28 3c 19 92 54 41 ec 51 b8
#>  [5905] 1e 60 cf 1b 41 a4 70 3d 9a 17 92 54 41 52 b8 1e 85 59 cf 1b 41 48 e1 7a
#>  [5929] 44 17 92 54 41 66 66 66 66 35 cf 1b 41 14 ae 47 61 14 92 54 41 48 e1 7a
#>  [5953] 14 32 cf 1b 41 b8 1e 85 fb 13 92 54 41 52 b8 1e 85 2f cf 1b 41 14 ae 47
#>  [5977] 61 13 92 54 41 9a 99 99 99 26 cf 1b 41 3d 0a d7 23 10 92 54 41 52 b8 1e
#>  [6001] 85 22 cf 1b 41 3d 0a d7 f3 0d 92 54 41 48 e1 7a 14 20 cf 1b 41 00 00 00
#>  [6025] 50 0d 92 54 41 29 5c 8f c2 1e cf 1b 41 33 33 33 13 0d 92 54 41 66 66 66
#>  [6049] 66 1c cf 1b 41 66 66 66 a6 0c 92 54 41 c3 f5 28 5c 18 cf 1b 41 48 e1 7a
#>  [6073] 24 0c 92 54 41 5c 8f c2 f5 01 cf 1b 41 52 b8 1e 65 0a 92 54 41 00 00 00
#>  [6097] 00 f6 ce 1b 41 a4 70 3d aa 09 92 54 41 b8 1e 85 eb ef ce 1b 41 1f 85 eb
#>  [6121] 71 09 92 54 41 f6 28 5c 8f e9 ce 1b 41 8f c2 f5 48 09 92 54 41 e1 7a 14
#>  [6145] ae a6 ce 1b 41 48 e1 7a 14 08 92 54 41 c3 f5 28 5c a1 ce 1b 41 7b 14 ae
#>  [6169] e7 07 92 54 41 85 eb 51 b8 99 ce 1b 41 e1 7a 14 6e 07 92 54 41 48 e1 7a
#>  [6193] 14 74 ce 1b 41 52 b8 1e 45 04 92 54 41 0a d7 a3 70 6c ce 1b 41 3d 0a d7
#>  [6217] d3 03 92 54 41 9a 99 99 99 66 ce 1b 41 33 33 33 b3 03 92 54 41 66 66 66
#>  [6241] 66 2d ce 1b 41 66 66 66 66 03 92 54 41 cd cc cc cc 25 ce 1b 41 9a 99 99
#>  [6265] 39 03 92 54 41 0a d7 a3 70 20 ce 1b 41 7b 14 ae d7 02 92 54 41 cd cc cc
#>  [6289] cc 19 ce 1b 41 1f 85 eb 31 02 92 54 41 7b 14 ae 47 17 ce 1b 41 00 00 00
#>  [6313] a0 01 92 54 41 00 00 00 00 17 ce 1b 41 29 5c 8f 62 00 92 54 41 29 5c 8f
#>  [6337] c2 17 ce 1b 41 ae 47 e1 8a ff 91 54 41 1f 85 eb 51 1a ce 1b 41 c3 f5 28
#>  [6361] dc fe 91 54 41 3d 0a d7 a3 31 ce 1b 41 7b 14 ae c7 f9 91 54 41 14 ae 47
#>  [6385] e1 32 ce 1b 41 52 b8 1e 45 f9 91 54 41 14 ae 47 e1 32 ce 1b 41 c3 f5 28
#>  [6409] fc f7 91 54 41 c3 f5 28 5c 30 ce 1b 41 29 5c 8f 82 f7 91 54 41 a4 70 3d
#>  [6433] 0a 1f ce 1b 41 00 00 00 d0 f5 91 54 41 33 33 33 33 1a ce 1b 41 ec 51 b8
#>  [6457] 6e f5 91 54 41 1f 85 eb 51 12 ce 1b 41 8f c2 f5 18 f5 91 54 41 9a 99 99
#>  [6481] 99 b9 cd 1b 41 0a d7 a3 e0 f2 91 54 41 9a 99 99 99 9c cd 1b 41 8f c2 f5
#>  [6505] d8 f1 91 54 41 7b 14 ae 47 97 cd 1b 41 7b 14 ae 87 f1 91 54 41 1f 85 eb
#>  [6529] 51 8d cd 1b 41 7b 14 ae 97 f0 91 54 41 33 33 33 33 58 cd 1b 41 f6 28 5c
#>  [6553] 7f ec 91 54 41 9a 99 99 99 52 cd 1b 41 1f 85 eb 21 ec 91 54 41 52 b8 1e
#>  [6577] 85 4d cd 1b 41 cd cc cc ec eb 91 54 41 b8 1e 85 eb 47 cd 1b 41 0a d7 a3
#>  [6601] d0 eb 91 54 41 33 33 33 33 2c cd 1b 41 7b 14 ae 87 eb 91 54 41 48 e1 7a
#>  [6625] 14 23 cd 1b 41 b8 1e 85 7b eb 91 54 41 9a 99 99 99 17 cd 1b 41 85 eb 51
#>  [6649] 98 eb 91 54 41 ec 51 b8 1e ea cc 1b 41 3d 0a d7 73 ec 91 54 41 ae 47 e1
#>  [6673] 7a d1 cc 1b 41 0a d7 a3 a0 ec 91 54 41 0a d7 a3 70 cd cc 1b 41 c3 f5 28
#>  [6697] 8c ec 91 54 41 14 ae 47 e1 c9 cc 1b 41 00 00 00 60 ec 91 54 41 ec 51 b8
#>  [6721] 1e bc cc 1b 41 9a 99 99 e9 ea 91 54 41 1f 85 eb 51 b9 cc 1b 41 f6 28 5c
#>  [6745] 3f ea 91 54 41 f6 28 5c 8f ba cc 1b 41 ae 47 e1 0a e9 91 54 41 b8 1e 85
#>  [6769] eb c0 cc 1b 41 0a d7 a3 a0 e5 91 54 41 b8 1e 85 eb c0 cc 1b 41 1f 85 eb
#>  [6793] f1 e4 91 54 41 cd cc cc cc bc cc 1b 41 ae 47 e1 3a e4 91 54 41 1f 85 eb
#>  [6817] 51 af cc 1b 41 0a d7 a3 90 e2 91 54 41 52 b8 1e 85 aa cc 1b 41 29 5c 8f
#>  [6841] 12 e2 91 54 41 66 66 66 66 a3 cc 1b 41 48 e1 7a 94 e1 91 54 41 48 e1 7a
#>  [6865] 14 73 cc 1b 41 8f c2 f5 b8 de 91 54 41 0a d7 a3 70 6d cc 1b 41 3d 0a d7
#>  [6889] 73 de 91 54 41 00 00 00 00 07 cc 1b 41 9a 99 99 89 da 91 54 41 85 eb 51
#>  [6913] b8 f4 cb 1b 41 66 66 66 c6 d9 91 54 41 cd cc cc cc e9 cb 1b 41 3d 0a d7
#>  [6937] 13 d9 91 54 41 33 33 33 33 80 cb 1b 41 a4 70 3d ba d1 91 54 41 00 00 00
#>  [6961] 00 73 cb 1b 41 ae 47 e1 aa d0 91 54 41 14 ae 47 e1 6c cb 1b 41 3d 0a d7
#>  [6985] 03 d0 91 54 41 29 5c 8f c2 58 cb 1b 41 ae 47 e1 ba cb 91 54 41 b8 1e 85
#>  [7009] eb 53 cb 1b 41 0a d7 a3 30 ca 91 54 41 85 eb 51 b8 4e cb 1b 41 e1 7a 14
#>  [7033] 9e c1 91 54 41 85 eb 51 b8 4d cb 1b 41 b8 1e 85 0b c1 91 54 41 71 3d 0a
#>  [7057] d7 46 cb 1b 41 b8 1e 85 fb be 91 54 41 52 b8 1e 85 43 cb 1b 41 0a d7 a3
#>  [7081] 50 be 91 54 41 8f c2 f5 28 3b cb 1b 41 b8 1e 85 1b bd 91 54 41 d7 a3 70
#>  [7105] 3d 07 cb 1b 41 8f c2 f5 38 b7 91 54 41 a4 70 3d 0a fe ca 1b 41 85 eb 51
#>  [7129] 18 b6 91 54 41 e1 7a 14 ae f7 ca 1b 41 71 3d 0a d7 b4 91 54 41 cd cc cc
#>  [7153] cc f4 ca 1b 41 7b 14 ae 27 b4 91 54 41 a4 70 3d 0a eb ca 1b 41 b8 1e 85
#>  [7177] db b1 91 54 41 33 33 33 33 ea ca 1b 41 a4 70 3d aa b1 91 54 41 8f c2 f5
#>  [7201] 28 e9 ca 1b 41 c3 f5 28 3c b1 91 54 41 a4 70 3d 0a e8 ca 1b 41 85 eb 51
#>  [7225] 58 af 91 54 41 9a 99 99 99 a5 ca 1b 41 a4 70 3d 5a b2 91 54 41 cd cc cc
#>  [7249] cc 58 c9 1b 41 a4 70 3d 6a c1 91 54 41 71 3d 0a d7 50 c9 1b 41 66 66 66
#>  [7273] c6 c1 91 54 41 00 00 00 00 10 c9 1b 41 a4 70 3d 1a c7 91 54 41 ae 47 e1
#>  [7297] 7a 82 c8 1b 41 ec 51 b8 be cd 91 54 41 00 00 00 00 c0 c6 1b 41 3d 0a d7
#>  [7321] 03 e3 91 54 41 9a 99 99 99 3f c4 1b 41 cd cc cc 6c 01 92 54 41 14 ae 47
#>  [7345] e1 0a bd 1b 41 9a 99 99 49 65 91 54 41 8f c2 f5 28 47 bd 1b 41 9a 99 99
#>  [7369] 89 62 91 54 41 7b 14 ae 47 97 bd 1b 41 14 ae 47 e1 5e 91 54 41 29 5c 8f
#>  [7393] c2 66 bd 1b 41 00 00 00 d0 5a 91 54 41 33 33 33 33 28 ba 1b 41 3d 0a d7
#>  [7417] 33 15 91 54 41 9a 99 99 99 f0 b9 1b 41 ae 47 e1 8a 10 91 54 41 00 00 00
#>  [7441] 00 c6 b8 1b 41 cd cc cc 7c f7 90 54 41 29 5c 8f c2 33 b8 1b 41 14 ae 47
#>  [7465] e1 fd 90 54 41 c3 f5 28 5c 12 b8 1b 41 33 33 33 63 ff 90 54 41 b8 1e 85
#>  [7489] eb 0d b7 1b 41 0a d7 a3 30 ea 90 54 41 14 ae 47 e1 63 b3 1b 41 8f c2 f5
#>  [7513] d8 9d 90 54 41 66 66 66 66 74 b3 1b 41 c3 f5 28 2c 9d 90 54 41 48 e1 7a
#>  [7537] 14 82 b5 1b 41 85 eb 51 98 87 90 54 41 66 66 66 66 56 b5 1b 41 d7 a3 70
#>  [7561] 6d 83 90 54 41 66 66 66 66 e5 b3 1b 41 1f 85 eb 31 60 90 54 41 a4 70 3d
#>  [7585] 0a bc b0 1b 41 ae 47 e1 1a 14 90 54 41 29 5c 8f c2 80 b5 1b 41 7b 14 ae
#>  [7609] 37 dd 8f 54 41 e1 7a 14 ae 79 b0 1b 41 14 ae 47 c1 6f 8f 54 41 e1 7a 14
#>  [7633] ae c9 aa 1b 41 f6 28 5c 3f f6 8e 54 41 c3 f5 28 5c c7 b2 1b 41 52 b8 1e
#>  [7657] f5 b1 8d 54 41 14 ae 47 e1 aa b2 1b 41 cd cc cc bc b2 8d 54 41 8f c2 f5
#>  [7681] 28 8e b2 1b 41 f6 28 5c 3f b3 8d 54 41 ae 47 e1 7a 71 b2 1b 41 cd cc cc
#>  [7705] 8c b3 8d 54 41 7b 14 ae 47 55 b2 1b 41 1f 85 eb a1 b3 8d 54 41 d7 a3 70
#>  [7729] 3d 37 b2 1b 41 5c 8f c2 75 b3 8d 54 41 71 3d 0a d7 ff b1 1b 41 f6 28 5c
#>  [7753] af b2 8d 54 41 ec 51 b8 1e 9a b0 1b 41 1f 85 eb e1 ab 8d 54 41 7b 14 ae
#>  [7777] 47 77 b0 1b 41 8f c2 f5 68 ab 8d 54 41 71 3d 0a d7 2e b0 1b 41 b8 1e 85
#>  [7801] cb aa 8d 54 41 e1 7a 14 ae 16 b0 1b 41 66 66 66 76 aa 8d 54 41 71 3d 0a
#>  [7825] d7 f1 af 1b 41 f6 28 5c 8f a9 8d 54 41 ae 47 e1 7a 94 af 1b 41 48 e1 7a
#>  [7849] 74 a6 8d 54 41 48 e1 7a 14 5d af 1b 41 d7 a3 70 7d a5 8d 54 41 48 e1 7a
#>  [7873] 14 50 af 1b 41 00 00 00 10 a5 8d 54 41 8f c2 f5 28 45 af 1b 41 e1 7a 14
#>  [7897] 6e a4 8d 54 41 8f c2 f5 28 1d af 1b 41 9a 99 99 29 a1 8d 54 41 33 33 33
#>  [7921] 33 02 af 1b 41 71 3d 0a 67 9f 8d 54 41 85 eb 51 b8 e5 ae 1b 41 e1 7a 14
#>  [7945] fe 9d 8d 54 41 3d 0a d7 a3 c4 ae 1b 41 e1 7a 14 de 9c 8d 54 41 8f c2 f5
#>  [7969] 28 6c ae 1b 41 f6 28 5c df 9a 8d 54 41 7b 14 ae 47 18 ae 1b 41 ae 47 e1
#>  [7993] 7a 99 8d 54 41 ec 51 b8 1e f0 ad 1b 41 0a d7 a3 b0 98 8d 54 41 7b 14 ae
#>  [8017] 47 de ad 1b 41 b8 1e 85 7b 98 8d 54 41 85 eb 51 b8 ba ad 1b 41 c3 f5 28
#>  [8041] 7c 98 8d 54 41 cd cc cc cc b0 ad 1b 41 85 eb 51 58 98 8d 54 41 14 ae 47
#>  [8065] e1 a6 ad 1b 41 33 33 33 f3 97 8d 54 41 14 ae 47 e1 89 ad 1b 41 85 eb 51
#>  [8089] 28 96 8d 54 41 f6 28 5c 8f 77 ad 1b 41 5c 8f c2 65 95 8d 54 41 0a d7 a3
#>  [8113] 70 62 ad 1b 41 cd cc cc 0c 95 8d 54 41 c3 f5 28 5c 2e ad 1b 41 3d 0a d7
#>  [8137] b3 94 8d 54 41 14 ae 47 e1 03 ad 1b 41 d7 a3 70 ed 93 8d 54 41 33 33 33
#>  [8161] 33 83 ac 1b 41 a4 70 3d 9a 90 8d 54 41 a4 70 3d 0a 20 ac 1b 41 9a 99 99
#>  [8185] 19 8d 8d 54 41 cd cc cc cc 04 ac 1b 41 7b 14 ae 77 8c 8d 54 41 a4 70 3d
#>  [8209] 0a f3 ab 1b 41 71 3d 0a 57 8c 8d 54 41 85 eb 51 b8 df ab 1b 41 3d 0a d7
#>  [8233] 63 8c 8d 54 41 33 33 33 33 80 ab 1b 41 e1 7a 14 9e 8d 8d 54 41 29 5c 8f
#>  [8257] c2 65 ab 1b 41 71 3d 0a b7 8d 8d 54 41 71 3d 0a d7 3c ab 1b 41 ae 47 e1
#>  [8281] 7a 8d 8d 54 41 d7 a3 70 3d ed aa 1b 41 c3 f5 28 7c 8c 8d 54 41 85 eb 51
#>  [8305] b8 db aa 1b 41 85 eb 51 78 8c 8d 54 41 71 3d 0a d7 d4 aa 1b 41 48 e1 7a
#>  [8329] 94 8c 8d 54 41 71 3d 0a d7 c2 aa 1b 41 7b 14 ae 37 8d 8d 54 41 a4 70 3d
#>  [8353] 0a b0 aa 1b 41 8f c2 f5 88 8e 8d 54 41 52 b8 1e 85 79 aa 1b 41 48 e1 7a
#>  [8377] 34 94 8d 54 41 52 b8 1e 85 67 aa 1b 41 0a d7 a3 10 95 8d 54 41 a4 70 3d
#>  [8401] 0a 5c aa 1b 41 8f c2 f5 28 95 8d 54 41 29 5c 8f c2 49 aa 1b 41 3d 0a d7
#>  [8425] d3 94 8d 54 41 3d 0a d7 a3 34 aa 1b 41 c3 f5 28 dc 93 8d 54 41 f6 28 5c
#>  [8449] 8f cd a9 1b 41 0a d7 a3 90 8c 8d 54 41 48 e1 7a 14 a4 a9 1b 41 8f c2 f5
#>  [8473] 58 8a 8d 54 41 52 b8 1e 85 72 a9 1b 41 a4 70 3d 7a 88 8d 54 41 71 3d 0a
#>  [8497] d7 4b a9 1b 41 29 5c 8f 62 87 8d 54 41 9a 99 99 99 20 a9 1b 41 33 33 33
#>  [8521] 73 86 8d 54 41 71 3d 0a d7 f1 a8 1b 41 0a d7 a3 a0 85 8d 54 41 f6 28 5c
#>  [8545] 8f a6 a8 1b 41 d7 a3 70 8d 84 8d 54 41 ec 51 b8 1e 5d a8 1b 41 f6 28 5c
#>  [8569] bf 83 8d 54 41 33 33 33 33 34 a8 1b 41 f6 28 5c 8f 83 8d 54 41 66 66 66
#>  [8593] 66 12 a8 1b 41 cd cc cc bc 83 8d 54 41 71 3d 0a d7 e1 a7 1b 41 cd cc cc
#>  [8617] 8c 84 8d 54 41 cd cc cc cc 6f a7 1b 41 48 e1 7a 24 87 8d 54 41 ec 51 b8
#>  [8641] 1e 16 a7 1b 41 48 e1 7a c4 88 8d 54 41 e1 7a 14 ae bc a6 1b 41 ae 47 e1
#>  [8665] ea 8a 8d 54 41 c3 f5 28 5c a5 a6 1b 41 85 eb 51 48 8b 8d 54 41 52 b8 1e
#>  [8689] 85 90 a6 1b 41 0a d7 a3 50 8b 8d 54 41 e1 7a 14 ae 7c a6 1b 41 85 eb 51
#>  [8713] 18 8b 8d 54 41 71 3d 0a d7 67 a6 1b 41 ec 51 b8 8e 8a 8d 54 41 52 b8 1e
#>  [8737] 85 ef a5 1b 41 e1 7a 14 ee 85 8d 54 41 a4 70 3d 0a 89 a5 1b 41 ae 47 e1
#>  [8761] ca 82 8d 54 41 66 66 66 66 27 a5 1b 41 5c 8f c2 25 81 8d 54 41 52 b8 1e
#>  [8785] 85 01 a5 1b 41 52 b8 1e c5 80 8d 54 41 c3 f5 28 5c ba a4 1b 41 52 b8 1e
#>  [8809] 95 80 8d 54 41 a4 70 3d 0a 97 a4 1b 41 e1 7a 14 be 80 8d 54 41 8f c2 f5
#>  [8833] 28 33 a4 1b 41 8f c2 f5 18 82 8d 54 41 9a 99 99 99 00 a4 1b 41 f6 28 5c
#>  [8857] 6f 82 8d 54 41 1f 85 eb 51 c1 a3 1b 41 85 eb 51 58 82 8d 54 41 b8 1e 85
#>  [8881] eb 99 a3 1b 41 ec 51 b8 ce 81 8d 54 41 ae 47 e1 7a 8e a3 1b 41 9a 99 99
#>  [8905] 79 81 8d 54 41 66 66 66 66 7a a3 1b 41 a4 70 3d 9a 80 8d 54 41 cd cc cc
#>  [8929] cc 67 a3 1b 41 1f 85 eb 61 7f 8d 54 41 0a d7 a3 70 38 a3 1b 41 7b 14 ae
#>  [8953] a7 7b 8d 54 41 52 b8 1e 85 2e a3 1b 41 9a 99 99 19 7b 8d 54 41 b8 1e 85
#>  [8977] eb 18 a3 1b 41 29 5c 8f 42 7a 8d 54 41 d7 a3 70 3d 00 a3 1b 41 52 b8 1e
#>  [9001] c5 79 8d 54 41 00 00 00 00 e4 a2 1b 41 0a d7 a3 80 79 8d 54 41 29 5c 8f
#>  [9025] c2 b5 a2 1b 41 cd cc cc 6c 79 8d 54 41 f6 28 5c 8f 6b a2 1b 41 66 66 66
#>  [9049] 86 79 8d 54 41 c3 f5 28 5c 41 a2 1b 41 9a 99 99 29 79 8d 54 41 52 b8 1e
#>  [9073] 85 1d a2 1b 41 7b 14 ae 87 78 8d 54 41 0a d7 a3 70 c0 a1 1b 41 85 eb 51
#>  [9097] 58 76 8d 54 41 3d 0a d7 a3 92 a1 1b 41 33 33 33 d3 74 8d 54 41 b8 1e 85
#>  [9121] eb 83 a1 1b 41 7b 14 ae e7 73 8d 54 41 ec 51 b8 1e 67 a1 1b 41 48 e1 7a
#>  [9145] e4 70 8d 54 41 71 3d 0a d7 57 a1 1b 41 48 e1 7a c4 6f 8d 54 41 cd cc cc
#>  [9169] cc ff a0 1b 41 f6 28 5c 1f 6b 8d 54 41 f6 28 5c 8f f0 a0 1b 41 d7 a3 70
#>  [9193] bd 6a 8d 54 41 14 ae 47 e1 d6 a0 1b 41 66 66 66 d6 6a 8d 54 41 14 ae 47
#>  [9217] e1 a6 a0 1b 41 33 33 33 c3 6b 8d 54 41 48 e1 7a 14 94 a0 1b 41 00 00 00
#>  [9241] e0 6b 8d 54 41 85 eb 51 b8 7e a0 1b 41 b8 1e 85 bb 6b 8d 54 41 71 3d 0a
#>  [9265] d7 47 a0 1b 41 d7 a3 70 0d 6b 8d 54 41 8f c2 f5 28 21 a0 1b 41 1f 85 eb
#>  [9289] 01 6b 8d 54 41 f6 28 5c 8f ab 9f 1b 41 e1 7a 14 8e 6c 8d 54 41 1f 85 eb
#>  [9313] 51 4e 9f 1b 41 9a 99 99 09 6e 8d 54 41 cd cc cc cc 2f 9f 1b 41 71 3d 0a
#>  [9337] 57 6e 8d 54 41 71 3d 0a d7 21 9f 1b 41 b8 1e 85 5b 6e 8d 54 41 3d 0a d7
#>  [9361] a3 f8 9e 1b 41 66 66 66 f6 6d 8d 54 41 c3 f5 28 5c ab 9e 1b 41 a4 70 3d
#>  [9385] 9a 6c 8d 54 41 0a d7 a3 70 55 9e 1b 41 14 ae 47 11 6c 8d 54 41 c3 f5 28
#>  [9409] 5c 42 9e 1b 41 f6 28 5c 6f 6b 8d 54 41 c3 f5 28 5c 34 9e 1b 41 29 5c 8f
#>  [9433] 32 6a 8d 54 41 33 33 33 33 2c 9e 1b 41 0a d7 a3 c0 68 8d 54 41 3d 0a d7
#>  [9457] a3 1c 9e 1b 41 85 eb 51 78 64 8d 54 41 8f c2 f5 28 13 9e 1b 41 85 eb 51
#>  [9481] 78 62 8d 54 41 c3 f5 28 5c 03 9e 1b 41 7b 14 ae 47 60 8d 54 41 14 ae 47
#>  [9505] e1 e8 9d 1b 41 29 5c 8f d2 5d 8d 54 41 9a 99 99 99 e7 9d 1b 41 48 e1 7a
#>  [9529] 54 5d 8d 54 41 e1 7a 14 ae ed 9d 1b 41 b8 1e 85 2b 5c 8d 54 41 29 5c 8f
#>  [9553] c2 04 9e 1b 41 00 00 00 20 59 8d 54 41 c3 f5 28 5c 0f 9e 1b 41 f6 28 5c
#>  [9577] ff 56 8d 54 41 7b 14 ae 47 22 9e 1b 41 85 eb 51 48 4e 8d 54 41 85 eb 51
#>  [9601] b8 34 9e 1b 41 71 3d 0a f7 48 8d 54 41 a4 70 3d 0a 4e 9e 1b 41 3d 0a d7
#>  [9625] 43 44 8d 54 41 a4 70 3d 0a 76 9e 1b 41 48 e1 7a 84 3e 8d 54 41 0a d7 a3
#>  [9649] 70 80 9e 1b 41 52 b8 1e 95 3c 8d 54 41 d7 a3 70 3d 86 9e 1b 41 0a d7 a3
#>  [9673] a0 3a 8d 54 41 0a d7 a3 70 87 9e 1b 41 00 00 00 60 37 8d 54 41 8f c2 f5
#>  [9697] 28 80 9e 1b 41 a4 70 3d da 2e 8d 54 41 14 ae 47 e1 82 9e 1b 41 b8 1e 85
#>  [9721] 1b 2a 8d 54 41 b8 1e 85 eb 8c 9e 1b 41 f6 28 5c 0f 25 8d 54 41 b8 1e 85
#>  [9745] eb a5 9e 1b 41 c3 f5 28 2c 1c 8d 54 41 5c 8f c2 f5 af 9e 1b 41 14 ae 47
#>  [9769] 71 16 8d 54 41 48 e1 7a 14 b1 9e 1b 41 cd cc cc 3c 0f 8d 54 41 f6 28 5c
#>  [9793] 8f a5 9e 1b 41 d7 a3 70 4d 00 8d 54 41 cd cc cc cc a6 9e 1b 41 3d 0a d7
#>  [9817] c3 fd 8c 54 41 66 66 66 66 b3 9e 1b 41 29 5c 8f 82 f8 8c 54 41 66 66 66
#>  [9841] 66 b4 9e 1b 41 a4 70 3d 5a f6 8c 54 41 29 5c 8f c2 af 9e 1b 41 e1 7a 14
#>  [9865] 3e f4 8c 54 41 14 ae 47 e1 9d 9e 1b 41 0a d7 a3 e0 ef 8c 54 41 85 eb 51
#>  [9889] b8 96 9e 1b 41 3d 0a d7 a3 ed 8c 54 41 48 e1 7a 14 89 9e 1b 41 7b 14 ae
#>  [9913] 67 e6 8c 54 41 1f 85 eb 51 73 9e 1b 41 a4 70 3d da de 8c 54 41 9a 99 99
#>  [9937] 99 69 9e 1b 41 1f 85 eb c1 da 8c 54 41 1f 85 eb 51 3b 9e 1b 41 0a d7 a3
#>  [9961] 10 c0 8c 54 41 52 b8 1e 85 32 9e 1b 41 8f c2 f5 e8 b9 8c 54 41 66 66 66
#>  [9985] 66 30 9e 1b 41 a4 70 3d 2a b6 8c 54 41 3d 0a d7 a3 32 9e 1b 41 85 eb 51
#> [10009] b8 b3 8c 54 41 8f c2 f5 28 4e 9e 1b 41 00 00 00 b0 a9 8c 54 41 8f c2 f5
#> [10033] 28 50 9e 1b 41 9a 99 99 19 a8 8c 54 41 c3 f5 28 5c 4f 9e 1b 41 0a d7 a3
#> [10057] c0 a6 8c 54 41 a4 70 3d 0a 4c 9e 1b 41 7b 14 ae a7 a5 8c 54 41 3d 0a d7
#> [10081] a3 42 9e 1b 41 33 33 33 63 a4 8c 54 41 1f 85 eb 51 24 9e 1b 41 ae 47 e1
#> [10105] 1a a2 8c 54 41 71 3d 0a d7 17 9e 1b 41 e1 7a 14 ce a0 8c 54 41 85 eb 51
#> [10129] b8 12 9e 1b 41 66 66 66 d6 9f 8c 54 41 8f c2 f5 28 10 9e 1b 41 7b 14 ae
#> [10153] 37 9e 8c 54 41 ae 47 e1 7a 15 9e 1b 41 29 5c 8f 02 9c 8c 54 41 33 33 33
#> [10177] 33 29 9e 1b 41 e1 7a 14 fe 97 8c 54 41 00 00 00 00 31 9e 1b 41 cd cc cc
#> [10201] 8c 95 8c 54 41 29 5c 8f c2 32 9e 1b 41 a4 70 3d da 93 8c 54 41 b8 1e 85
#> [10225] eb 2f 9e 1b 41 33 33 33 23 91 8c 54 41 0a d7 a3 70 26 9e 1b 41 71 3d 0a
#> [10249] 17 8e 8c 54 41 48 e1 7a 14 14 9e 1b 41 52 b8 1e 85 8a 8c 54 41 1f 85 eb
#> [10273] 51 fa 9d 1b 41 ec 51 b8 9e 86 8c 54 41 0a d7 a3 70 cb 9d 1b 41 29 5c 8f
#> [10297] 62 80 8c 54 41 b8 1e 85 eb bd 9d 1b 41 5c 8f c2 25 7e 8c 54 41 c3 f5 28
#> [10321] 5c b0 9d 1b 41 e1 7a 14 fe 7a 8c 54 41 85 eb 51 b8 a9 9d 1b 41 5c 8f c2
#> [10345] e5 77 8c 54 41 14 ae 47 e1 a7 9d 1b 41 d7 a3 70 dd 74 8c 54 41 b8 1e 85
#> [10369] eb ac 9d 1b 41 8f c2 f5 d8 71 8c 54 41 52 b8 1e 85 b7 9d 1b 41 14 ae 47
#> [10393] c1 6f 8c 54 41 00 00 00 00 c8 9d 1b 41 48 e1 7a a4 6d 8c 54 41 29 5c 8f
#> [10417] c2 da 9d 1b 41 48 e1 7a b4 6b 8c 54 41 5c 8f c2 f5 0b 9e 1b 41 1f 85 eb
#> [10441] 51 67 8c 54 41 cd cc cc cc 11 9e 1b 41 d7 a3 70 2d 66 8c 54 41 cd cc cc
#> [10465] cc 11 9e 1b 41 a4 70 3d 6a 65 8c 54 41 ae 47 e1 7a 0e 9e 1b 41 3d 0a d7
#> [10489] 93 64 8c 54 41 9a 99 99 99 fa 9d 1b 41 14 ae 47 e1 61 8c 54 41 e1 7a 14
#> [10513] ae f3 9d 1b 41 85 eb 51 a8 60 8c 54 41 d7 a3 70 3d ea 9d 1b 41 f6 28 5c
#> [10537] 4f 5d 8c 54 41 ec 51 b8 1e e6 9d 1b 41 00 00 00 80 5c 8c 54 41 5c 8f c2
#> [10561] f5 db 9d 1b 41 8f c2 f5 b8 5b 8c 54 41 85 eb 51 b8 cc 9d 1b 41 3d 0a d7
#> [10585] 73 5b 8c 54 41 00 00 00 00 c0 9d 1b 41 8f c2 f5 88 5b 8c 54 41 e1 7a 14
#> [10609] ae ab 9d 1b 41 ae 47 e1 fa 5b 8c 54 41 ae 47 e1 7a 9e 9d 1b 41 cd cc cc
#> [10633] 7c 5c 8c 54 41 f6 28 5c 8f 92 9d 1b 41 3d 0a d7 23 5d 8c 54 41 ec 51 b8
#> [10657] 1e 88 9d 1b 41 33 33 33 f3 5d 8c 54 41 33 33 33 33 69 9d 1b 41 00 00 00
#> [10681] 00 61 8c 54 41 5c 8f c2 f5 46 9d 1b 41 7b 14 ae d7 63 8c 54 41 33 33 33
#> [10705] 33 c2 9c 1b 41 a4 70 3d 0a 6d 8c 54 41 5c 8f c2 f5 9e 9c 1b 41 29 5c 8f
#> [10729] f2 6f 8c 54 41 1f 85 eb 51 86 9c 1b 41 14 ae 47 d1 72 8c 54 41 29 5c 8f
#> [10753] c2 56 9c 1b 41 66 66 66 e6 7a 8c 54 41 0a d7 a3 70 1d 9c 1b 41 8f c2 f5
#> [10777] 68 82 8c 54 41 85 eb 51 b8 0c 9c 1b 41 c3 f5 28 2c 85 8c 54 41 7b 14 ae
#> [10801] 47 cc 9b 1b 41 29 5c 8f a2 95 8c 54 41 a4 70 3d 0a 90 9b 1b 41 33 33 33
#> [10825] d3 a2 8c 54 41 c3 f5 28 5c 6c 9b 1b 41 66 66 66 66 a9 8c 54 41 1f 85 eb
#> [10849] 51 32 9b 1b 41 8f c2 f5 e8 b1 8c 54 41 0a d7 a3 70 15 9b 1b 41 ec 51 b8
#> [10873] 3e b7 8c 54 41 48 e1 7a 14 0b 9b 1b 41 1f 85 eb 11 ba 8c 54 41 a4 70 3d
#> [10897] 0a 04 9b 1b 41 14 ae 47 11 bd 8c 54 41 c3 f5 28 5c fa 9a 1b 41 29 5c 8f
#> [10921] 62 c8 8c 54 41 1f 85 eb 51 f4 9a 1b 41 a4 70 3d 4a cb 8c 54 41 ae 47 e1
#> [10945] 7a eb 9a 1b 41 8f c2 f5 08 ce 8c 54 41 ae 47 e1 7a d8 9a 1b 41 cd cc cc
#> [10969] dc d1 8c 54 41 7b 14 ae 47 a3 9a 1b 41 ae 47 e1 3a d9 8c 54 41 1f 85 eb
#> [10993] 51 92 9a 1b 41 71 3d 0a 37 dc 8c 54 41 33 33 33 33 20 9a 1b 41 8f c2 f5
#> [11017] 98 f4 8c 54 41 f6 28 5c 8f 11 9a 1b 41 14 ae 47 c1 f8 8c 54 41 52 b8 1e
#> [11041] 85 f5 99 1b 41 d7 a3 70 ad 02 8d 54 41 1f 85 eb 51 e1 99 1b 41 71 3d 0a
#> [11065] 27 08 8d 54 41 f6 28 5c 8f cf 99 1b 41 14 ae 47 a1 0b 8d 54 41 ec 51 b8
#> [11089] 1e aa 99 1b 41 29 5c 8f e2 11 8d 54 41 ae 47 e1 7a a1 99 1b 41 a4 70 3d
#> [11113] ca 13 8d 54 41 00 00 00 00 a0 99 1b 41 0a d7 a3 70 15 8d 54 41 48 e1 7a
#> [11137] 14 a3 99 1b 41 52 b8 1e 95 16 8d 54 41 85 eb 51 b8 c3 99 1b 41 9a 99 99
#> [11161] b9 1b 8d 54 41 9a 99 99 99 c8 99 1b 41 ec 51 b8 fe 1c 8d 54 41 14 ae 47
#> [11185] e1 c9 99 1b 41 ec 51 b8 3e 1e 8d 54 41 9a 99 99 99 c5 99 1b 41 71 3d 0a
#> [11209] 37 20 8d 54 41 c3 f5 28 5c b3 99 1b 41 a4 70 3d 0a 24 8d 54 41 cd cc cc
#> [11233] cc ab 99 1b 41 c3 f5 28 7c 26 8d 54 41 52 b8 1e 85 a8 99 1b 41 66 66 66
#> [11257] f6 28 8d 54 41 3d 0a d7 a3 a7 99 1b 41 ec 51 b8 3e 2e 8d 54 41 71 3d 0a
#> [11281] d7 9e 99 1b 41 9a 99 99 d9 31 8d 54 41 c3 f5 28 5c 90 99 1b 41 29 5c 8f
#> [11305] 12 35 8d 54 41 14 ae 47 e1 7b 99 1b 41 14 ae 47 d1 38 8d 54 41 3d 0a d7
#> [11329] a3 57 99 1b 41 9a 99 99 b9 3e 8d 54 41 ae 47 e1 7a 25 99 1b 41 66 66 66
#> [11353] d6 45 8d 54 41 7b 14 ae 47 04 99 1b 41 a4 70 3d ba 49 8d 54 41 52 b8 1e
#> [11377] 85 f2 98 1b 41 14 ae 47 71 4b 8d 54 41 0a d7 a3 70 dd 98 1b 41 3d 0a d7
#> [11401] e3 4c 8d 54 41 ae 47 e1 7a d0 98 1b 41 5c 8f c2 65 4d 8d 54 41 3d 0a d7
#> [11425] a3 ba 98 1b 41 00 00 00 d0 4d 8d 54 41 1f 85 eb 51 78 98 1b 41 5c 8f c2
#> [11449] 05 4e 8d 54 41 3d 0a d7 a3 5d 98 1b 41 7b 14 ae 57 4e 8d 54 41 52 b8 1e
#> [11473] 85 f8 97 1b 41 5c 8f c2 b5 50 8d 54 41 0a d7 a3 70 e4 97 1b 41 7b 14 ae
#> [11497] 07 51 8d 54 41 8f c2 f5 28 d3 97 1b 41 3d 0a d7 23 51 8d 54 41 33 33 33
#> [11521] 33 c4 97 1b 41 48 e1 7a 14 51 8d 54 41 14 ae 47 e1 b1 97 1b 41 ec 51 b8
#> [11545] 9e 50 8d 54 41 e1 7a 14 ae a7 97 1b 41 8f c2 f5 18 50 8d 54 41 00 00 00
#> [11569] 00 8e 97 1b 41 66 66 66 46 4e 8d 54 41 14 ae 47 e1 79 97 1b 41 ec 51 b8
#> [11593] 4e 4d 8d 54 41 ec 51 b8 1e 5a 97 1b 41 00 00 00 70 4c 8d 54 41 5c 8f c2
#> [11617] f5 41 97 1b 41 71 3d 0a 27 4c 8d 54 41 5c 8f c2 f5 31 97 1b 41 00 00 00
#> [11641] 40 4c 8d 54 41 1f 85 eb 51 cb 96 1b 41 cd cc cc 2c 4e 8d 54 41 66 66 66
#> [11665] 66 83 96 1b 41 9a 99 99 29 4f 8d 54 41 7b 14 ae 47 5e 96 1b 41 ec 51 b8
#> [11689] 4e 4f 8d 54 41 33 33 33 33 1d 96 1b 41 52 b8 1e b5 4e 8d 54 41 c3 f5 28
#> [11713] 5c 06 96 1b 41 9a 99 99 c9 4e 8d 54 41 00 00 00 00 e2 95 1b 41 d7 a3 70
#> [11737] 7d 4f 8d 54 41 00 00 00 00 af 95 1b 41 c3 f5 28 fc 50 8d 54 41 c3 f5 28
#> [11761] 5c 88 95 1b 41 29 5c 8f 82 52 8d 54 41 f6 28 5c 8f 64 95 1b 41 5c 8f c2
#> [11785] 25 54 8d 54 41 1f 85 eb 51 44 95 1b 41 71 3d 0a 47 55 8d 54 41 85 eb 51
#> [11809] b8 c1 94 1b 41 48 e1 7a 44 58 8d 54 41 3d 0a d7 a3 7d 94 1b 41 d7 a3 70
#> [11833] 1d 5a 8d 54 41 85 eb 51 b8 32 94 1b 41 66 66 66 56 5c 8d 54 41 48 e1 7a
#> [11857] 14 18 94 1b 41 14 ae 47 d1 5c 8d 54 41 85 eb 51 b8 03 94 1b 41 5c 8f c2
#> [11881] d5 5c 8d 54 41 b8 1e 85 eb f0 93 1b 41 48 e1 7a 84 5c 8d 54 41 71 3d 0a
#> [11905] d7 bf 93 1b 41 66 66 66 b6 5a 8d 54 41 14 ae 47 e1 68 93 1b 41 b8 1e 85
#> [11929] ab 58 8d 54 41 7b 14 ae 47 46 93 1b 41 5c 8f c2 15 58 8d 54 41 48 e1 7a
#> [11953] 14 ef 92 1b 41 c3 f5 28 4c 57 8d 54 41 7b 14 ae 47 bf 92 1b 41 5c 8f c2
#> [11977] 95 56 8d 54 41 e1 7a 14 ae 51 92 1b 41 9a 99 99 09 54 8d 54 41 71 3d 0a
#> [12001] d7 ff 91 1b 41 c3 f5 28 5c 52 8d 54 41 3d 0a d7 a3 f4 91 1b 41 a4 70 3d
#> [12025] ea 51 8d 54 41 29 5c 8f c2 ec 91 1b 41 00 00 00 60 51 8d 54 41 ae 47 e1
#> [12049] 7a de 91 1b 41 c3 f5 28 4c 4f 8d 54 41 f6 28 5c 8f d6 91 1b 41 9a 99 99
#> [12073] 79 4e 8d 54 41 1f 85 eb 51 c8 91 1b 41 e1 7a 14 7e 4d 8d 54 41 33 33 33
#> [12097] 33 b2 91 1b 41 14 ae 47 51 4c 8d 54 41 71 3d 0a d7 82 91 1b 41 1f 85 eb
#> [12121] 41 4a 8d 54 41 a4 70 3d 0a b3 90 1b 41 e1 7a 14 8e 42 8d 54 41 a4 70 3d
#> [12145] 0a 3e 90 1b 41 71 3d 0a 37 3d 8d 54 41 29 5c 8f c2 1d 90 1b 41 ec 51 b8
#> [12169] ee 3b 8d 54 41 33 33 33 33 ef 8f 1b 41 8f c2 f5 68 3a 8d 54 41 e1 7a 14
#> [12193] ae d0 8f 1b 41 29 5c 8f a2 39 8d 54 41 5c 8f c2 f5 b2 8f 1b 41 0a d7 a3
#> [12217] 10 39 8d 54 41 ae 47 e1 7a 89 8f 1b 41 b8 1e 85 9b 38 8d 54 41 cd cc cc
#> [12241] cc 6f 8f 1b 41 b8 1e 85 8b 38 8d 54 41 33 33 33 33 57 8f 1b 41 cd cc cc
#> [12265] ac 38 8d 54 41 00 00 00 00 3e 8f 1b 41 ae 47 e1 1a 39 8d 54 41 c3 f5 28
#> [12289] 5c 22 8f 1b 41 a4 70 3d ea 39 8d 54 41 0a d7 a3 70 c8 8e 1b 41 d7 a3 70
#> [12313] 5d 3d 8d 54 41 7b 14 ae 47 9f 8e 1b 41 b8 1e 85 ab 3e 8d 54 41 f6 28 5c
#> [12337] 8f 22 8e 1b 41 00 00 00 40 41 8d 54 41 e1 7a 14 ae ec 8d 1b 41 d7 a3 70
#> [12361] 9d 42 8d 54 41 c3 f5 28 5c c9 8d 1b 41 66 66 66 b6 43 8d 54 41 1f 85 eb
#> [12385] 51 93 8d 1b 41 3d 0a d7 f3 45 8d 54 41 cd cc cc cc 31 8d 1b 41 9a 99 99
#> [12409] f9 4a 8d 54 41 8f c2 f5 28 09 8d 1b 41 a4 70 3d ea 4c 8d 54 41 48 e1 7a
#> [12433] 14 e3 8c 1b 41 14 ae 47 81 4e 8d 54 41 f6 28 5c 8f b1 8c 1b 41 f6 28 5c
#> [12457] ff 4f 8d 54 41 a4 70 3d 0a 92 8c 1b 41 a4 70 3d aa 50 8d 54 41 0a d7 a3
#> [12481] 70 3c 8c 1b 41 5c 8f c2 25 52 8d 54 41 9a 99 99 99 f9 8b 1b 41 5c 8f c2
#> [12505] f5 52 8d 54 41 9a 99 99 99 db 8b 1b 41 ec 51 b8 1e 53 8d 54 41 b8 1e 85
#> [12529] eb bf 8b 1b 41 71 3d 0a 17 53 8d 54 41 00 00 00 00 7a 8b 1b 41 ec 51 b8
#> [12553] 9e 52 8d 54 41 cd cc cc cc 5e 8b 1b 41 71 3d 0a 97 52 8d 54 41 48 e1 7a
#> [12577] 14 52 8b 1b 41 33 33 33 b3 52 8d 54 41 5c 8f c2 f5 39 8b 1b 41 5c 8f c2
#> [12601] 35 53 8d 54 41 cd cc cc cc 2d 8b 1b 41 3d 0a d7 a3 53 8d 54 41 3d 0a d7
#> [12625] a3 13 8b 1b 41 14 ae 47 e1 54 8d 54 41 5c 8f c2 f5 a4 8a 1b 41 14 ae 47
#> [12649] b1 5b 8d 54 41 85 eb 51 b8 84 8a 1b 41 52 b8 1e 75 5d 8d 54 41 ae 47 e1
#> [12673] 7a 66 8a 1b 41 66 66 66 b6 5e 8d 54 41 ae 47 e1 7a 57 8a 1b 41 cd cc cc
#> [12697] 2c 5f 8d 54 41 29 5c 8f c2 26 8a 1b 41 14 ae 47 21 60 8d 54 41 3d 0a d7
#> [12721] a3 02 8a 1b 41 f6 28 5c 8f 60 8d 54 41 c3 f5 28 5c d1 89 1b 41 d7 a3 70
#> [12745] dd 60 8d 54 41 14 ae 47 e1 b6 89 1b 41 d7 a3 70 cd 60 8d 54 41 52 b8 1e
#> [12769] 85 a0 89 1b 41 48 e1 7a 74 60 8d 54 41 52 b8 1e 85 82 89 1b 41 0a d7 a3
#> [12793] 60 5f 8d 54 41 5c 8f c2 f5 45 89 1b 41 1f 85 eb 61 5c 8d 54 41 cd cc cc
#> [12817] cc 2d 89 1b 41 29 5c 8f 72 5b 8d 54 41 1f 85 eb 51 11 89 1b 41 c3 f5 28
#> [12841] 9c 5a 8d 54 41 ae 47 e1 7a dd 88 1b 41 00 00 00 70 59 8d 54 41 ec 51 b8
#> [12865] 1e 71 88 1b 41 8f c2 f5 48 57 8d 54 41 c3 f5 28 5c 25 88 1b 41 ec 51 b8
#> [12889] 5e 55 8d 54 41 71 3d 0a d7 15 88 1b 41 a4 70 3d 3a 55 8d 54 41 8f c2 f5
#> [12913] 28 0a 88 1b 41 33 33 33 73 55 8d 54 41 d7 a3 70 3d fe 87 1b 41 71 3d 0a
#> [12937] 47 56 8d 54 41 d7 a3 70 3d ec 87 1b 41 ae 47 e1 2a 58 8d 54 41 52 b8 1e
#> [12961] 85 b9 87 1b 41 00 00 00 70 5e 8d 54 41 48 e1 7a 14 99 87 1b 41 b8 1e 85
#> [12985] 3b 63 8d 54 41 b8 1e 85 eb 77 87 1b 41 d7 a3 70 bd 68 8d 54 41 3d 0a d7
#> [13009] a3 63 87 1b 41 cd cc cc 7c 6b 8d 54 41 52 b8 1e 85 4b 87 1b 41 71 3d 0a
#> [13033] d7 6d 8d 54 41 7b 14 ae 47 29 87 1b 41 14 ae 47 41 70 8d 54 41 8f c2 f5
#> [13057] 28 af 86 1b 41 29 5c 8f 42 77 8d 54 41 8f c2 f5 28 7d 86 1b 41 cd cc cc
#> [13081] 9c 79 8d 54 41 8f c2 f5 28 2e 86 1b 41 71 3d 0a e7 7c 8d 54 41 48 e1 7a
#> [13105] 14 08 86 1b 41 9a 99 99 59 7e 8d 54 41 52 b8 1e 85 e5 85 1b 41 9a 99 99
#> [13129] 59 7f 8d 54 41 66 66 66 66 cd 85 1b 41 7b 14 ae c7 7f 8d 54 41 f6 28 5c
#> [13153] 8f 8a 85 1b 41 33 33 33 83 80 8d 54 41 e1 7a 14 ae 71 85 1b 41 1f 85 eb
#> [13177] 01 81 8d 54 41 8f c2 f5 28 51 85 1b 41 ae 47 e1 2a 82 8d 54 41 1f 85 eb
#> [13201] 51 ca 84 1b 41 5c 8f c2 55 88 8d 54 41 00 00 00 00 41 84 1b 41 3d 0a d7
#> [13225] 83 8f 8d 54 41 9a 99 99 99 f5 83 1b 41 a4 70 3d ca 93 8d 54 41 cd cc cc
#> [13249] cc c2 83 1b 41 f6 28 5c ef 96 8d 54 41 d7 a3 70 3d ad 83 1b 41 d7 a3 70
#> [13273] 7d 98 8d 54 41 7b 14 ae 47 9e 83 1b 41 33 33 33 e3 99 8d 54 41 f6 28 5c
#> [13297] 8f 8f 83 1b 41 f6 28 5c df 9b 8d 54 41 cd cc cc cc 7b 83 1b 41 0a d7 a3
#> [13321] 20 9f 8d 54 41 14 ae 47 e1 6e 83 1b 41 cd cc cc 0c a0 8d 54 41 33 33 33
#> [13345] 33 64 83 1b 41 66 66 66 66 a0 8d 54 41 66 66 66 66 51 83 1b 41 3d 0a d7
#> [13369] a3 a0 8d 54 41 d7 a3 70 3d 1a 83 1b 41 33 33 33 53 a0 8d 54 41 33 33 33
#> [13393] 33 07 83 1b 41 33 33 33 53 a0 8d 54 41 1f 85 eb 51 ef 82 1b 41 d7 a3 70
#> [13417] bd a0 8d 54 41 9a 99 99 99 e2 82 1b 41 00 00 00 50 a1 8d 54 41 ec 51 b8
#> [13441] 1e d2 82 1b 41 1f 85 eb b1 a2 8d 54 41 52 b8 1e 85 a8 82 1b 41 9a 99 99
#> [13465] 99 a7 8d 54 41 29 5c 8f c2 95 82 1b 41 d7 a3 70 8d a9 8d 54 41 cd cc cc
#> [13489] cc 52 82 1b 41 7b 14 ae d7 af 8d 54 41 ae 47 e1 7a 2a 82 1b 41 5c 8f c2
#> [13513] 55 b3 8d 54 41 8f c2 f5 28 f5 81 1b 41 5c 8f c2 25 b7 8d 54 41 14 ae 47
#> [13537] e1 af 81 1b 41 29 5c 8f 42 bd 8d 54 41 e1 7a 14 ae 90 81 1b 41 c3 f5 28
#> [13561] 6c bf 8d 54 41 5c 8f c2 f5 50 81 1b 41 48 e1 7a 54 c3 8d 54 41 d7 a3 70
#> [13585] 3d 2f 81 1b 41 a4 70 3d 9a c5 8d 54 41 cd cc cc cc df 80 1b 41 66 66 66
#> [13609] 66 cc 8d 54 41 66 66 66 66 c4 80 1b 41 a4 70 3d 6a ce 8d 54 41 a4 70 3d
#> [13633] 0a 9d 80 1b 41 14 ae 47 f1 d0 8d 54 41 ae 47 e1 7a 77 80 1b 41 1f 85 eb
#> [13657] f1 d2 8d 54 41 85 eb 51 b8 68 80 1b 41 48 e1 7a 84 d3 8d 54 41 a4 70 3d
#> [13681] 0a 5a 80 1b 41 e1 7a 14 de d3 8d 54 41 d7 a3 70 3d 49 80 1b 41 f6 28 5c
#> [13705] 0f d4 8d 54 41 b8 1e 85 eb f8 7f 1b 41 0a d7 a3 10 d4 8d 54 41 e1 7a 14
#> [13729] ae e8 7f 1b 41 14 ae 47 41 d4 8d 54 41 f6 28 5c 8f d2 7f 1b 41 c3 f5 28
#> [13753] dc d4 8d 54 41 ec 51 b8 1e c6 7f 1b 41 ec 51 b8 7e d5 8d 54 41 71 3d 0a
#> [13777] d7 b3 7f 1b 41 5c 8f c2 45 d7 8d 54 41 7b 14 ae 47 ae 7f 1b 41 1f 85 eb
#> [13801] 41 d8 8d 54 41 ae 47 e1 7a a9 7f 1b 41 00 00 00 c0 d9 8d 54 41 00 00 00
#> [13825] 00 a8 7f 1b 41 33 33 33 93 dc 8d 54 41 ae 47 e1 7a a3 7f 1b 41 52 b8 1e
#> [13849] 15 de 8d 54 41 f6 28 5c 8f 98 7f 1b 41 ec 51 b8 5e df 8d 54 41 33 33 33
#> [13873] 33 80 7f 1b 41 14 ae 47 01 e1 8d 54 41 9a 99 99 99 67 7f 1b 41 5c 8f c2
#> [13897] 25 e2 8d 54 41 00 00 00 00 53 7f 1b 41 8f c2 f5 d8 e2 8d 54 41 b8 1e 85
#> [13921] eb ee 7e 1b 41 7b 14 ae 37 e5 8d 54 41 f6 28 5c 8f c7 7e 1b 41 d7 a3 70
#> [13945] 6d e6 8d 54 41 a4 70 3d 0a 87 7e 1b 41 00 00 00 e0 e8 8d 54 41 a4 70 3d
#> [13969] 0a a4 7d 1b 41 71 3d 0a d7 f2 8d 54 41 00 00 00 00 82 7d 1b 41 e1 7a 14
#> [13993] 7e f4 8d 54 41 1f 85 eb 51 65 7d 1b 41 52 b8 1e 25 f6 8d 54 41 f6 28 5c
#> [14017] 8f 54 7d 1b 41 e1 7a 14 4e f7 8d 54 41 a4 70 3d 0a 31 7d 1b 41 29 5c 8f
#> [14041] 42 fa 8d 54 41 f6 28 5c 8f 0f 7d 1b 41 c3 f5 28 bc fd 8d 54 41 85 eb 51
#> [14065] b8 89 7c 1b 41 cd cc cc 7c 0e 8e 54 41 e1 7a 14 ae 72 7c 1b 41 c3 f5 28
#> [14089] 3c 12 8e 54 41 c3 f5 28 5c 55 7c 1b 41 33 33 33 13 19 8e 54 41 5c 8f c2
#> [14113] f5 4b 7c 1b 41 5c 8f c2 95 1a 8e 54 41 a4 70 3d 0a 3f 7c 1b 41 3d 0a d7
#> [14137] 23 1c 8e 54 41 7b 14 ae 47 2d 7c 1b 41 ae 47 e1 ca 1d 8e 54 41 85 eb 51
#> [14161] b8 08 7c 1b 41 d7 a3 70 7d 20 8e 54 41 a4 70 3d 0a eb 7b 1b 41 a4 70 3d
#> [14185] 6a 22 8e 54 41 29 5c 8f c2 76 7b 1b 41 f6 28 5c 3f 29 8e 54 41 1f 85 eb
#> [14209] 51 59 7b 1b 41 33 33 33 33 2b 8e 54 41 0a d7 a3 70 3e 7b 1b 41 b8 1e 85
#> [14233] 3b 2d 8e 54 41 b8 1e 85 eb 1b 7b 1b 41 85 eb 51 58 30 8e 54 41 a4 70 3d
#> [14257] 0a 00 7b 1b 41 48 e1 7a 44 33 8e 54 41 52 b8 1e 85 ec 7a 1b 41 f6 28 5c
#> [14281] df 35 8e 54 41 e1 7a 14 ae e5 7a 1b 41 14 ae 47 71 37 8e 54 41 b8 1e 85
#> [14305] eb e2 7a 1b 41 ec 51 b8 ee 38 8e 54 41 e1 7a 14 ae e3 7a 1b 41 cd cc cc
#> [14329] 6c 3a 8e 54 41 cd cc cc cc e7 7a 1b 41 71 3d 0a e7 3b 8e 54 41 85 eb 51
#> [14353] b8 ef 7a 1b 41 85 eb 51 48 3d 8e 54 41 1f 85 eb 51 03 7b 1b 41 71 3d 0a
#> [14377] 17 3f 8e 54 41 71 3d 0a d7 2f 7b 1b 41 29 5c 8f 12 42 8e 54 41 66 66 66
#> [14401] 66 3f 7b 1b 41 71 3d 0a 57 43 8e 54 41 9a 99 99 99 49 7b 1b 41 7b 14 ae
#> [14425] 97 44 8e 54 41 33 33 33 33 4f 7b 1b 41 0a d7 a3 e0 45 8e 54 41 00 00 00
#> [14449] 00 50 7b 1b 41 85 eb 51 d8 47 8e 54 41 5c 8f c2 f5 4b 7b 1b 41 9a 99 99
#> [14473] 29 49 8e 54 41 ec 51 b8 1e 44 7b 1b 41 ec 51 b8 6e 4a 8e 54 41 29 5c 8f
#> [14497] c2 29 7b 1b 41 71 3d 0a 57 4d 8e 54 41 14 ae 47 e1 22 7b 1b 41 b8 1e 85
#> [14521] 6b 4e 8e 54 41 ec 51 b8 1e 20 7b 1b 41 b8 1e 85 7b 4f 8e 54 41 b8 1e 85
#> [14545] eb 22 7b 1b 41 00 00 00 80 50 8e 54 41 0a d7 a3 70 2f 7b 1b 41 52 b8 1e
#> [14569] f5 51 8e 54 41 1f 85 eb 51 44 7b 1b 41 29 5c 8f 62 53 8e 54 41 8f c2 f5
#> [14593] 28 5a 7b 1b 41 1f 85 eb 61 54 8e 54 41 b8 1e 85 eb b0 7b 1b 41 52 b8 1e
#> [14617] 95 57 8e 54 41 ae 47 e1 7a c3 7b 1b 41 52 b8 1e 95 58 8e 54 41 ec 51 b8
#> [14641] 1e d9 7b 1b 41 7b 14 ae 47 5a 8e 54 41 85 eb 51 b8 ea 7b 1b 41 c3 f5 28
#> [14665] 6c 5c 8e 54 41 8f c2 f5 28 f5 7b 1b 41 9a 99 99 f9 5e 8e 54 41 52 b8 1e
#> [14689] 85 f7 7b 1b 41 48 e1 7a b4 60 8e 54 41 00 00 00 00 f6 7b 1b 41 ae 47 e1
#> [14713] 4a 62 8e 54 41 66 66 66 66 ed 7b 1b 41 29 5c 8f 42 64 8e 54 41 b8 1e 85
#> [14737] eb dc 7b 1b 41 d7 a3 70 ed 65 8e 54 41 1f 85 eb 51 93 7b 1b 41 7b 14 ae
#> [14761] 57 6b 8e 54 41 14 ae 47 e1 63 7b 1b 41 00 00 00 40 6f 8e 54 41 5c 8f c2
#> [14785] f5 45 7b 1b 41 33 33 33 f3 71 8e 54 41 3d 0a d7 a3 30 7b 1b 41 14 ae 47
#> [14809] 51 74 8e 54 41 d7 a3 70 3d 10 7b 1b 41 d7 a3 70 4d 79 8e 54 41 7b 14 ae
#> [14833] 47 02 7b 1b 41 ec 51 b8 9e 7a 8e 54 41 0a d7 a3 70 ed 7a 1b 41 cd cc cc
#> [14857] 1c 7c 8e 54 41 a4 70 3d 0a d3 7a 1b 41 3d 0a d7 b3 7d 8e 54 41 85 eb 51
#> [14881] b8 8a 7a 1b 41 7b 14 ae 67 81 8e 54 41 85 eb 51 b8 7a 7a 1b 41 3d 0a d7
#> [14905] 83 82 8e 54 41 48 e1 7a 14 71 7a 1b 41 52 b8 1e d5 83 8e 54 41 3d 0a d7
#> [14929] a3 60 7a 1b 41 52 b8 1e d5 87 8e 54 41 29 5c 8f c2 57 7a 1b 41 71 3d 0a
#> [14953] 27 89 8e 54 41 9a 99 99 99 4a 7a 1b 41 0a d7 a3 90 8a 8e 54 41 9a 99 99
#> [14977] 99 e6 79 1b 41 e1 7a 14 ae 92 8e 54 41 7b 14 ae 47 ac 79 1b 41 85 eb 51
#> [15001] f8 98 8e 54 41 b8 1e 85 eb 93 79 1b 41 9a 99 99 59 9b 8e 54 41 a4 70 3d
#> [15025] 0a 69 79 1b 41 52 b8 1e 05 9f 8e 54 41 c3 f5 28 5c 08 79 1b 41 8f c2 f5
#> [15049] c8 a6 8e 54 41 b8 1e 85 eb d5 78 1b 41 48 e1 7a 84 ab 8e 54 41 33 33 33
#> [15073] 33 c9 78 1b 41 f6 28 5c 0f ad 8e 54 41 ec 51 b8 1e c1 78 1b 41 9a 99 99
#> [15097] a9 ae 8e 54 41 ec 51 b8 1e be 78 1b 41 cd cc cc 7c b0 8e 54 41 b8 1e 85
#> [15121] eb bc 78 1b 41 b8 1e 85 5b b4 8e 54 41 66 66 66 66 ba 78 1b 41 f6 28 5c
#> [15145] 2f b6 8e 54 41 48 e1 7a 14 b2 78 1b 41 52 b8 1e b5 b8 8e 54 41 33 33 33
#> [15169] 33 a7 78 1b 41 00 00 00 50 ba 8e 54 41 52 b8 1e 85 8a 78 1b 41 f6 28 5c
#> [15193] 0f bd 8e 54 41 7b 14 ae 47 24 78 1b 41 ae 47 e1 aa c4 8e 54 41 e1 7a 14
#> [15217] ae 0d 78 1b 41 33 33 33 b3 c6 8e 54 41 52 b8 1e 85 01 78 1b 41 33 33 33
#> [15241] a3 c8 8e 54 41 33 33 33 33 f5 77 1b 41 5c 8f c2 55 cf 8e 54 41 66 66 66
#> [15265] 66 ef 77 1b 41 33 33 33 c3 d0 8e 54 41 52 b8 1e 85 e6 77 1b 41 cd cc cc
#> [15289] 1c d2 8e 54 41 c3 f5 28 5c da 77 1b 41 a4 70 3d 6a d3 8e 54 41 ae 47 e1
#> [15313] 7a af 77 1b 41 48 e1 7a c4 d6 8e 54 41 e1 7a 14 ae 57 77 1b 41 48 e1 7a
#> [15337] 84 de 8e 54 41 b8 1e 85 eb 33 77 1b 41 71 3d 0a 07 e2 8e 54 41 cd cc cc
#> [15361] cc 19 77 1b 41 66 66 66 e6 e4 8e 54 41 e1 7a 14 ae ff 76 1b 41 00 00 00
#> [15385] 30 e8 8e 54 41 33 33 33 33 da 76 1b 41 9a 99 99 89 ed 8e 54 41 71 3d 0a
#> [15409] d7 87 76 1b 41 e1 7a 14 7e fa 8e 54 41 9a 99 99 99 76 76 1b 41 ec 51 b8
#> [15433] be fc 8e 54 41 3d 0a d7 a3 53 76 1b 41 66 66 66 86 00 8f 54 41 71 3d 0a
#> [15457] d7 2f 76 1b 41 8f c2 f5 e8 03 8f 54 41 14 ae 47 e1 10 76 1b 41 33 33 33
#> [15481] 53 06 8f 54 41 8f c2 f5 28 f1 75 1b 41 33 33 33 43 08 8f 54 41 ae 47 e1
#> [15505] 7a b2 75 1b 41 52 b8 1e 95 0b 8f 54 41 66 66 66 66 9e 75 1b 41 71 3d 0a
#> [15529] 07 0d 8f 54 41 66 66 66 66 8b 75 1b 41 f6 28 5c 0f 0f 8f 54 41 3d 0a d7
#> [15553] a3 65 75 1b 41 7b 14 ae 07 15 8f 54 41 85 eb 51 b8 47 75 1b 41 66 66 66
#> [15577] a6 18 8f 54 41 48 e1 7a 14 2f 75 1b 41 29 5c 8f a2 1a 8f 54 41 b8 1e 85
#> [15601] eb 12 75 1b 41 14 ae 47 41 1c 8f 54 41 c3 f5 28 5c ef 74 1b 41 48 e1 7a
#> [15625] d4 1d 8f 54 41 14 ae 47 e1 8f 74 1b 41 71 3d 0a 17 21 8f 54 41 29 5c 8f
#> [15649] c2 77 74 1b 41 66 66 66 06 22 8f 54 41 00 00 00 00 66 74 1b 41 66 66 66
#> [15673] f6 22 8f 54 41 cd cc cc cc 58 74 1b 41 ae 47 e1 0a 24 8f 54 41 ec 51 b8
#> [15697] 1e 4c 74 1b 41 66 66 66 e6 25 8f 54 41 c3 f5 28 5c 48 74 1b 41 e1 7a 14
#> [15721] be 27 8f 54 41 0a d7 a3 70 4d 74 1b 41 cd cc cc 8c 29 8f 54 41 52 b8 1e
#> [15745] 85 68 74 1b 41 52 b8 1e d5 2d 8f 54 41 cd cc cc cc 69 74 1b 41 a4 70 3d
#> [15769] 2a 2f 8f 54 41 b8 1e 85 eb 60 74 1b 41 c3 f5 28 7c 30 8f 54 41 d7 a3 70
#> [15793] 3d 43 74 1b 41 48 e1 7a 74 32 8f 54 41 7b 14 ae 47 e4 73 1b 41 33 33 33
#> [15817] d3 37 8f 54 41 3d 0a d7 a3 96 73 1b 41 ae 47 e1 9a 3c 8f 54 41 7b 14 ae
#> [15841] 47 6f 73 1b 41 ec 51 b8 4e 3f 8f 54 41 9a 99 99 99 3f 73 1b 41 f6 28 5c
#> [15865] 5f 43 8f 54 41 14 ae 47 e1 32 73 1b 41 33 33 33 43 44 8f 54 41 e1 7a 14
#> [15889] ae 25 73 1b 41 a4 70 3d ea 44 8f 54 41 29 5c 8f c2 eb 72 1b 41 ec 51 b8
#> [15913] fe 46 8f 54 41 ec 51 b8 1e d2 72 1b 41 33 33 33 13 48 8f 54 41 5c 8f c2
#> [15937] f5 73 72 1b 41 3d 0a d7 e3 4c 8f 54 41 33 33 33 33 47 72 1b 41 ae 47 e1
#> [15961] 7a 4e 8f 54 41 00 00 00 00 28 72 1b 41 5c 8f c2 05 50 8f 54 41 1f 85 eb
#> [15985] 51 1b 72 1b 41 14 ae 47 e1 50 8f 54 41 66 66 66 66 10 72 1b 41 9a 99 99
#> [16009] e9 51 8f 54 41 00 00 00 00 06 72 1b 41 29 5c 8f 52 53 8f 54 41 a4 70 3d
#> [16033] 0a e3 71 1b 41 29 5c 8f 12 59 8f 54 41 3d 0a d7 a3 d5 71 1b 41 85 eb 51
#> [16057] 68 5a 8f 54 41 33 33 33 33 b9 71 1b 41 b8 1e 85 1b 5c 8f 54 41 0a d7 a3
#> [16081] 70 27 71 1b 41 b8 1e 85 cb 62 8f 54 41 48 e1 7a 14 11 71 1b 41 ae 47 e1
#> [16105] 8a 63 8f 54 41 ec 51 b8 1e 02 71 1b 41 0a d7 a3 d0 63 8f 54 41 cd cc cc
#> [16129] cc ef 70 1b 41 8f c2 f5 e8 63 8f 54 41 52 b8 1e 85 92 70 1b 41 0a d7 a3
#> [16153] 80 63 8f 54 41 a4 70 3d 0a 75 70 1b 41 d7 a3 70 8d 63 8f 54 41 52 b8 1e
#> [16177] 85 54 70 1b 41 b8 1e 85 db 63 8f 54 41 e1 7a 14 ae 30 70 1b 41 66 66 66
#> [16201] 76 64 8f 54 41 f6 28 5c 8f 09 70 1b 41 a4 70 3d 6a 65 8f 54 41 5c 8f c2
#> [16225] f5 83 6f 1b 41 52 b8 1e 95 69 8f 54 41 5c 8f c2 f5 73 6f 1b 41 ec 51 b8
#> [16249] de 69 8f 54 41 29 5c 8f c2 67 6f 1b 41 71 3d 0a e7 69 8f 54 41 c3 f5 28
#> [16273] 5c 5e 6f 1b 41 ec 51 b8 be 69 8f 54 41 5c 8f c2 f5 56 6f 1b 41 5c 8f c2
#> [16297] 75 69 8f 54 41 c3 f5 28 5c 4f 6f 1b 41 ae 47 e1 ca 68 8f 54 41 d7 a3 70
#> [16321] 3d 4a 6f 1b 41 7b 14 ae e7 67 8f 54 41 ae 47 e1 7a 41 6f 1b 41 ec 51 b8
#> [16345] be 63 8f 54 41 9a 99 99 99 39 6f 1b 41 8f c2 f5 38 62 8f 54 41 1f 85 eb
#> [16369] 51 2b 6f 1b 41 f6 28 5c cf 60 8f 54 41 8f c2 f5 28 f8 6e 1b 41 29 5c 8f
#> [16393] 32 5d 8f 54 41 c3 f5 28 5c e7 6e 1b 41 7b 14 ae 77 5b 8f 54 41 00 00 00
#> [16417] 00 e2 6e 1b 41 33 33 33 43 5a 8f 54 41 33 33 33 33 e1 6e 1b 41 48 e1 7a
#> [16441] a4 59 8f 54 41 ae 47 e1 7a e4 6e 1b 41 8f c2 f5 b8 57 8f 54 41 e1 7a 14
#> [16465] ae f1 6e 1b 41 0a d7 a3 b0 55 8f 54 41 f6 28 5c 8f 0c 6f 1b 41 ec 51 b8
#> [16489] 6e 53 8f 54 41 a4 70 3d 0a 2c 6f 1b 41 71 3d 0a 77 51 8f 54 41 f6 28 5c
#> [16513] 8f b1 6f 1b 41 71 3d 0a e7 49 8f 54 41 29 5c 8f c2 cf 6f 1b 41 71 3d 0a
#> [16537] f7 47 8f 54 41 71 3d 0a d7 f6 6f 1b 41 ec 51 b8 ee 44 8f 54 41 14 ae 47
#> [16561] e1 09 70 1b 41 a4 70 3d fa 42 8f 54 41 48 e1 7a 14 1b 70 1b 41 00 00 00
#> [16585] 70 40 8f 54 41 8f c2 f5 28 22 70 1b 41 85 eb 51 98 3e 8f 54 41 ec 51 b8
#> [16609] 1e 21 70 1b 41 9a 99 99 c9 3c 8f 54 41 8f c2 f5 28 15 70 1b 41 a4 70 3d
#> [16633] fa 3a 8f 54 41 ae 47 e1 7a fe 6f 1b 41 7b 14 ae 17 39 8f 54 41 cd cc cc
#> [16657] cc 82 6f 1b 41 f6 28 5c 7f 31 8f 54 41 cd cc cc cc 66 6f 1b 41 00 00 00
#> [16681] a0 2f 8f 54 41 7b 14 ae 47 3b 6f 1b 41 3d 0a d7 33 2c 8f 54 41 cd cc cc
#> [16705] cc db 6e 1b 41 c3 f5 28 fc 23 8f 54 41 1f 85 eb 51 b3 6e 1b 41 52 b8 1e
#> [16729] f5 20 8f 54 41 b8 1e 85 eb 8d 6e 1b 41 00 00 00 80 1e 8f 54 41 66 66 66
#> [16753] 66 63 6e 1b 41 71 3d 0a 17 1c 8f 54 41 d7 a3 70 3d 3c 6e 1b 41 52 b8 1e
#> [16777] 85 1a 8f 54 41 5c 8f c2 f5 2a 6e 1b 41 00 00 00 10 1a 8f 54 41 8f c2 f5
#> [16801] 28 1a 6e 1b 41 33 33 33 d3 19 8f 54 41 14 ae 47 e1 08 6e 1b 41 b8 1e 85
#> [16825] db 19 8f 54 41 ae 47 e1 7a ed 6d 1b 41 33 33 33 a3 1a 8f 54 41 9a 99 99
#> [16849] 99 b6 6d 1b 41 00 00 00 a0 1d 8f 54 41 f6 28 5c 8f 91 6d 1b 41 29 5c 8f
#> [16873] 32 1f 8f 54 41 f6 28 5c 8f 82 6d 1b 41 48 e1 7a a4 1f 8f 54 41 ae 47 e1
#> [16897] 7a 5d 6d 1b 41 0a d7 a3 70 20 8f 54 41 e1 7a 14 ae 1b 6d 1b 41 14 ae 47
#> [16921] 81 21 8f 54 41 1f 85 eb 51 e7 6c 1b 41 ec 51 b8 9e 22 8f 54 41 e1 7a 14
#> [16945] ae cd 6c 1b 41 ec 51 b8 8e 23 8f 54 41 48 e1 7a 14 b6 6c 1b 41 c3 f5 28
#> [16969] cc 24 8f 54 41 48 e1 7a 14 74 6c 1b 41 14 ae 47 e1 28 8f 54 41 71 3d 0a
#> [16993] d7 f1 6b 1b 41 1f 85 eb c1 2f 8f 54 41 8f c2 f5 28 d6 6b 1b 41 9a 99 99
#> [17017] 69 31 8f 54 41 3d 0a d7 a3 81 6b 1b 41 d7 a3 70 1d 37 8f 54 41 14 ae 47
#> [17041] e1 5f 6b 1b 41 d7 a3 70 0d 39 8f 54 41 9a 99 99 99 4d 6b 1b 41 52 b8 1e
#> [17065] d5 39 8f 54 41 f6 28 5c 8f 3c 6b 1b 41 ec 51 b8 4e 3a 8f 54 41 1f 85 eb
#> [17089] 51 0e 6b 1b 41 d7 a3 70 dd 3a 8f 54 41 66 66 66 66 f2 6a 1b 41 c3 f5 28
#> [17113] 6c 3b 8f 54 41 29 5c 8f c2 bb 6a 1b 41 48 e1 7a 34 3d 8f 54 41 8f c2 f5
#> [17137] 28 a6 6a 1b 41 66 66 66 a6 3d 8f 54 41 66 66 66 66 94 6a 1b 41 d7 a3 70
#> [17161] 6d 3d 8f 54 41 29 5c 8f c2 70 6a 1b 41 b8 1e 85 cb 3b 8f 54 41 1f 85 eb
#> [17185] 51 64 6a 1b 41 66 66 66 76 3b 8f 54 41 0a d7 a3 70 4e 6a 1b 41 66 66 66
#> [17209] 46 3b 8f 54 41 7b 14 ae 47 35 6a 1b 41 7b 14 ae 87 3b 8f 54 41 a4 70 3d
#> [17233] 0a 25 6a 1b 41 d7 a3 70 fd 3b 8f 54 41 a4 70 3d 0a 16 6a 1b 41 8f c2 f5
#> [17257] a8 3c 8f 54 41 71 3d 0a d7 08 6a 1b 41 c3 f5 28 7c 3d 8f 54 41 29 5c 8f
#> [17281] c2 ef 69 1b 41 0a d7 a3 70 3f 8f 54 41 29 5c 8f c2 df 69 1b 41 cd cc cc
#> [17305] 6c 40 8f 54 41 b8 1e 85 eb c9 69 1b 41 85 eb 51 58 41 8f 54 41 8f c2 f5
#> [17329] 28 bb 69 1b 41 ae 47 e1 ca 41 8f 54 41 8f c2 f5 28 7c 69 1b 41 48 e1 7a
#> [17353] 14 43 8f 54 41 7b 14 ae 47 ba 68 1b 41 33 33 33 f3 45 8f 54 41 7b 14 ae
#> [17377] 47 89 68 1b 41 33 33 33 f3 46 8f 54 41 5c 8f c2 f5 57 68 1b 41 8f c2 f5
#> [17401] 28 48 8f 54 41 7b 14 ae 47 2b 68 1b 41 ae 47 e1 8a 49 8f 54 41 3d 0a d7
#> [17425] a3 02 68 1b 41 9a 99 99 19 4b 8f 54 41 7b 14 ae 47 da 67 1b 41 5c 8f c2
#> [17449] f5 4c 8f 54 41 66 66 66 66 71 67 1b 41 cd cc cc 3c 52 8f 54 41 9a 99 99
#> [17473] 99 4d 67 1b 41 29 5c 8f c2 53 8f 54 41 48 e1 7a 14 db 66 1b 41 0a d7 a3
#> [17497] c0 57 8f 54 41 8f c2 f5 28 c1 66 1b 41 52 b8 1e d5 58 8f 54 41 5c 8f c2
#> [17521] f5 a3 66 1b 41 85 eb 51 78 5a 8f 54 41 14 ae 47 e1 6a 66 1b 41 ec 51 b8
#> [17545] ee 5e 8f 54 41 a4 70 3d 0a 11 66 1b 41 85 eb 51 18 65 8f 54 41 29 5c 8f
#> [17569] c2 db 65 1b 41 a4 70 3d 5a 69 8f 54 41 1f 85 eb 51 9b 65 1b 41 29 5c 8f
#> [17593] 32 6f 8f 54 41 c3 f5 28 5c 7a 65 1b 41 52 b8 1e b5 72 8f 54 41 8f c2 f5
#> [17617] 28 6d 65 1b 41 48 e1 7a 84 74 8f 54 41 7b 14 ae 47 64 65 1b 41 33 33 33
#> [17641] 33 76 8f 54 41 1f 85 eb 51 4f 65 1b 41 f6 28 5c 3f 7b 8f 54 41 ec 51 b8
#> [17665] 1e 41 65 1b 41 52 b8 1e b5 7d 8f 54 41 ec 51 b8 1e 2d 65 1b 41 0a d7 a3
#> [17689] 70 80 8f 54 41 d7 a3 70 3d 13 65 1b 41 cd cc cc 6c 83 8f 54 41 c3 f5 28
#> [17713] 5c e8 64 1b 41 a4 70 3d ba 87 8f 54 41 b8 1e 85 eb c9 64 1b 41 d7 a3 70
#> [17737] 6d 8a 8f 54 41 8f c2 f5 28 b9 64 1b 41 14 ae 47 71 8b 8f 54 41 cd cc cc
#> [17761] cc b0 64 1b 41 a4 70 3d 9a 8b 8f 54 41 3d 0a d7 a3 a7 64 1b 41 e1 7a 14
#> [17785] 6e 8b 8f 54 41 ec 51 b8 1e 7b 64 1b 41 5c 8f c2 35 89 8f 54 41 c3 f5 28
#> [17809] 5c 5e 64 1b 41 1f 85 eb 31 88 8f 54 41 14 ae 47 e1 40 64 1b 41 c3 f5 28
#> [17833] ac 87 8f 54 41 e1 7a 14 ae 24 64 1b 41 8f c2 f5 a8 87 8f 54 41 d7 a3 70
#> [17857] 3d 0a 64 1b 41 ec 51 b8 1e 88 8f 54 41 33 33 33 33 f8 63 1b 41 e1 7a 14
#> [17881] be 88 8f 54 41 00 00 00 00 ba 63 1b 41 ec 51 b8 ce 8b 8f 54 41 52 b8 1e
#> [17905] 85 9b 63 1b 41 0a d7 a3 20 8d 8f 54 41 14 ae 47 e1 81 63 1b 41 c3 f5 28
#> [17929] dc 8d 8f 54 41 66 66 66 66 73 63 1b 41 8f c2 f5 f8 8d 8f 54 41 0a d7 a3
#> [17953] 70 66 63 1b 41 0a d7 a3 e0 8d 8f 54 41 1f 85 eb 51 51 63 1b 41 71 3d 0a
#> [17977] 67 8d 8f 54 41 8f c2 f5 28 fe 62 1b 41 52 b8 1e 95 8a 8f 54 41 cd cc cc
#> [18001] cc d8 62 1b 41 1f 85 eb a1 89 8f 54 41 d7 a3 70 3d b7 62 1b 41 85 eb 51
#> [18025] 38 89 8f 54 41 ae 47 e1 7a 97 62 1b 41 5c 8f c2 55 89 8f 54 41 85 eb 51
#> [18049] b8 6a 62 1b 41 a4 70 3d 5a 8a 8f 54 41 b8 1e 85 eb 16 62 1b 41 9a 99 99
#> [18073] c9 8c 8f 54 41 8f c2 f5 28 b7 61 1b 41 71 3d 0a d7 8f 8f 54 41 66 66 66
#> [18097] 66 45 61 1b 41 cd cc cc fc 92 8f 54 41 52 b8 1e 85 1d 61 1b 41 29 5c 8f
#> [18121] 52 94 8f 54 41 66 66 66 66 f7 60 1b 41 a4 70 3d fa 95 8f 54 41 52 b8 1e
#> [18145] 85 df 60 1b 41 f6 28 5c 2f 97 8f 54 41 0a d7 a3 70 c8 60 1b 41 14 ae 47
#> [18169] 91 98 8f 54 41 c3 f5 28 5c b1 60 1b 41 0a d7 a3 50 9a 8f 54 41 29 5c 8f
#> [18193] c2 98 60 1b 41 14 ae 47 91 9c 8f 54 41 5c 8f c2 f5 40 60 1b 41 7b 14 ae
#> [18217] d7 a5 8f 54 41 52 b8 1e 85 ea 5f 1b 41 66 66 66 46 b0 8f 54 41 48 e1 7a
#> [18241] 14 dc 5f 1b 41 85 eb 51 b8 b1 8f 54 41 48 e1 7a 14 bb 5f 1b 41 3d 0a d7
#> [18265] 83 b4 8f 54 41 f6 28 5c 8f 96 5f 1b 41 e1 7a 14 1e b7 8f 54 41 85 eb 51
#> [18289] b8 70 5f 1b 41 c3 f5 28 6c b9 8f 54 41 a4 70 3d 0a d3 5e 1b 41 52 b8 1e
#> [18313] 35 c1 8f 54 41 a4 70 3d 0a b2 5e 1b 41 8f c2 f5 f8 c2 8f 54 41 85 eb 51
#> [18337] b8 8b 5e 1b 41 29 5c 8f 52 c5 8f 54 41 29 5c 8f c2 6d 5e 1b 41 0a d7 a3
#> [18361] a0 c7 8f 54 41 8f c2 f5 28 55 5e 1b 41 a4 70 3d fa c9 8f 54 41 b8 1e 85
#> [18385] eb 31 5e 1b 41 9a 99 99 e9 cd 8f 54 41 f6 28 5c 8f e7 5d 1b 41 14 ae 47
#> [18409] 71 d3 8f 54 41 71 3d 0a d7 d1 5d 1b 41 1f 85 eb 31 d5 8f 54 41 66 66 66
#> [18433] 66 b5 5d 1b 41 e1 7a 14 5e d7 8f 54 41 29 5c 8f c2 99 5d 1b 41 a4 70 3d
#> [18457] ca d9 8f 54 41 33 33 33 33 85 5d 1b 41 33 33 33 23 dd 8f 54 41 14 ae 47
#> [18481] e1 25 62 1b 41 ec 51 b8 5e 7a 91 54 41 8f c2 f5 28 4c 64 1b 41 52 b8 1e
#> [18505] 55 3a 92 54 41 5c 8f c2 f5 67 64 1b 41 d7 a3 70 fd 39 92 54 41 e1 7a 14
#> [18529] ae 83 65 1b 41 d7 a3 70 fd 9e 92 54 41 9a 99 99 99 37 64 1b 41 1f 85 eb
#> [18553] a1 a2 92 54 41 48 e1 7a 14 ce 64 1b 41 29 5c 8f 32 d8 92 54 41 5c 8f c2
#> [18577] f5 27 66 1b 41 33 33 33 53 53 93 54 41 48 e1 7a 14 5f 6a 1b 41 48 e1 7a
#> [18601] 54 47 93 54 41 0a d7 a3 70 73 6b 1b 41 a4 70 3d ba a2 93 54 41 48 e1 7a
#> [18625] 14 20 6c 1b 41 33 33 33 d3 e2 93 54 41 14 ae 47 e1 a3 6c 1b 41 8f c2 f5
#> [18649] 68 e1 93 54 41 8f c2 f5 28 2b 70 1b 41 5c 8f c2 b5 d7 93 54 41 3d 0a d7
#> [18673] a3 13 71 1b 41 8f c2 f5 f8 2c 94 54 41 29 5c 8f c2 28 72 1b 41 71 3d 0a
#> [18697] 77 8c 94 54 41 b8 1e 85 eb 2f 72 1b 41 00 00 00 f0 8e 94 54 41 0a d7 a3
#> [18721] 70 7f 72 1b 41 1f 85 eb 31 8e 94 54 41 33 33 33 33 93 78 1b 41 0a d7 a3
#> [18745] b0 7f 94 54 41 5c 8f c2 f5 17 7f 1b 41 14 ae 47 21 70 94 54 41 8f c2 f5
#> [18769] 28 25 83 1b 41 a4 70 3d 1a 68 94 54 41 48 e1 7a 14 75 83 1b 41 b8 1e 85
#> [18793] 7b 67 94 54 41 ec 51 b8 1e a3 83 1b 41 d7 a3 70 ed 79 94 54 41 3d 0a d7
#> [18817] a3 60 85 1b 41 52 b8 1e 95 75 94 54 41 0a d7 a3 70 33 86 1b 41 cd cc cc
#> [18841] 7c 73 94 54 41 ec 51 b8 1e 49 86 1b 41 14 ae 47 41 73 94 54 41 3d 0a d7
#> [18865] a3 50 86 1b 41 b8 1e 85 7b 79 94 54 41 f6 28 5c 8f 86 85 1b 41 3d 0a d7
#> [18889] c3 84 94 54 41 ec 51 b8 1e 4f 85 1b 41 b8 1e 85 db 87 94 54 41 8f c2 f5
#> [18913] 28 a0 85 1b 41 a4 70 3d 0a 8f 94 54 41 ae 47 e1 7a fc 85 1b 41 9a 99 99
#> [18937] 39 97 94 54 41 a4 70 3d 0a fc 85 1b 41 0a d7 a3 00 a1 94 54 41 ae 47 e1
#> [18961] 7a cf 85 1b 41 e1 7a 14 fe a1 94 54 41 b8 1e 85 eb af 85 1b 41 1f 85 eb
#> [18985] b1 a2 94 54 41 ec 51 b8 1e 27 85 1b 41 3d 0a d7 c3 a4 94 54 41 29 5c 8f
#> [19009] c2 03 84 1b 41 8f c2 f5 28 a2 94 54 41 33 33 33 33 ac 83 1b 41 14 ae 47
#> [19033] f1 ad 94 54 41 52 b8 1e 85 09 83 1b 41 71 3d 0a 07 b7 94 54 41 c3 f5 28
#> [19057] 5c 37 83 1b 41 0a d7 a3 00 bf 94 54 41 ae 47 e1 7a 76 84 1b 41 52 b8 1e
#> [19081] 05 b7 94 54 41 33 33 33 33 ce 85 1b 41 00 00 00 e0 af 94 54 41 29 5c 8f
#> [19105] c2 f0 85 1b 41 29 5c 8f 52 af 94 54 41 66 66 66 66 bd 86 1b 41 c3 f5 28
#> [19129] 0c ac 94 54 41 33 33 33 33 4b 87 1b 41 00 00 00 f0 b6 94 54 41 33 33 33
#> [19153] 33 50 8b 1b 41 8f c2 f5 18 ac 94 54 41 0a d7 a3 70 bb 8b 1b 41 1f 85 eb
#> [19177] f1 aa 94 54 41 33 33 33 33 e9 8b 1b 41 33 33 33 83 aa 94 54 41 cd cc cc
#> [19201] cc 0b 8c 1b 41 00 00 00 30 aa 94 54 41 c3 f5 28 5c 14 8c 1b 41 b8 1e 85
#> [19225] 1b aa 94 54 41 33 33 33 33 2a 91 1b 41 a4 70 3d da 9d 94 54 41 52 b8 1e
#> [19249] 85 95 91 1b 41 71 3d 0a d7 f4 94 54 41 48 e1 7a 14 ce 96 1b 41 00 00 00
#> [19273] a0 e6 94 54 41 b8 1e 85 eb 71 99 1b 41 cd cc cc dc e0 95 54 41 3d 0a d7
#> [19297] a3 4b 9d 1b 41 8f c2 f5 68 d6 95 54 41 71 3d 0a d7 61 9e 1b 41 9a 99 99
#> [19321] f9 86 96 54 41 00 00 00 00 67 9e 1b 41 48 e1 7a 04 8a 96 54 41 e1 7a 14
#> [19345] ae 01 9f 1b 41 0a d7 a3 f0 d9 96 54 41 00 00 00 00 a0 a1 1b 41 66 66 66
#> [19369] d6 cf 96 54 41 7b 14 ae 47 7a a1 1b 41 00 00 00 10 c6 96 54 41 3d 0a d7
#> [19393] a3 16 a2 1b 41 48 e1 7a b4 c3 96 54 41 66 66 66 66 3c a2 1b 41 b8 1e 85
#> [19417] 7b cd 96 54 41 85 eb 51 b8 8b a3 1b 41 d7 a3 70 6d c8 96 54 41 85 eb 51
#> [19441] b8 d5 a4 1b 41 0a d7 a3 c0 c4 96 54 41 d7 a3 70 3d 90 a6 1b 41 e1 7a 14
#> [19465] 0e c0 96 54 41 14 ae 47 e1 78 bc 1b 41 48 e1 7a b4 75 96 54 41
#> 
#> [[2]]
#>     [1] 01 06 00 00 00 1d 00 00 00 01 03 00 00 00 01 00 00 00 17 04 00 00 0a d7
#>    [25] a3 70 2d f8 1b 41 f6 28 5c 5f b9 a3 54 41 a4 70 3d 0a 41 f7 1b 41 71 3d
#>    [49] 0a f7 65 a3 54 41 52 b8 1e 85 6c f8 1b 41 f6 28 5c 3f 63 a3 54 41 29 5c
#>    [73] 8f c2 03 f9 1b 41 0a d7 a3 e0 61 a3 54 41 3d 0a d7 a3 22 02 1c 41 c3 f5
#>    [97] 28 ec 4a a3 54 41 e1 7a 14 ae 38 02 1c 41 48 e1 7a 04 51 a3 54 41 85 eb
#>   [121] 51 b8 55 02 1c 41 a4 70 3d 0a 59 a3 54 41 d7 a3 70 3d 5c 02 1c 41 ae 47
#>   [145] e1 da 5a a3 54 41 f6 28 5c 8f d9 04 1c 41 cd cc cc 9c 52 a3 54 41 a4 70
#>   [169] 3d 0a d8 08 1c 41 33 33 33 63 45 a3 54 41 52 b8 1e 85 c9 08 1c 41 8f c2
#>   [193] f5 38 3f a3 54 41 71 3d 0a d7 bd 08 1c 41 48 e1 7a 44 3a a3 54 41 1f 85
#>   [217] eb 51 04 08 1c 41 52 b8 1e 85 eb a2 54 41 1f 85 eb 51 a6 07 1c 41 cd cc
#>   [241] cc 5c c0 a2 54 41 33 33 33 33 90 07 1c 41 0a d7 a3 b0 b7 a2 54 41 cd cc
#>   [265] cc cc 8a 07 1c 41 5c 8f c2 15 b6 a2 54 41 a4 70 3d 0a 02 07 1c 41 5c 8f
#>   [289] c2 f5 93 a2 54 41 33 33 33 33 14 06 1c 41 7b 14 ae f7 5b a2 54 41 0a d7
#>   [313] a3 70 89 08 1c 41 a4 70 3d ea 53 a2 54 41 7b 14 ae 47 e5 06 1c 41 5c 8f
#>   [337] c2 75 d6 a1 54 41 d7 a3 70 3d 35 06 1c 41 d7 a3 70 fd a1 a1 54 41 1f 85
#>   [361] eb 51 b4 05 1c 41 48 e1 7a c4 7b a1 54 41 1f 85 eb 51 b1 05 1c 41 14 ae
#>   [385] 47 e1 7a a1 54 41 1f 85 eb 51 a6 05 1c 41 ec 51 b8 6e 77 a1 54 41 5c 8f
#>   [409] c2 f5 a4 05 1c 41 a4 70 3d 0a 77 a1 54 41 7b 14 ae 47 a3 05 1c 41 8f c2
#>   [433] f5 88 76 a1 54 41 c3 f5 28 5c 81 05 1c 41 9a 99 99 99 78 a1 54 41 ae 47
#>   [457] e1 7a ac 04 1c 41 00 00 00 80 85 a1 54 41 33 33 33 33 86 04 1c 41 9a 99
#>   [481] 99 19 89 a1 54 41 cd cc cc cc ae 03 1c 41 c3 f5 28 3c 57 a1 54 41 8f c2
#>   [505] f5 28 c8 03 1c 41 ae 47 e1 da 54 a1 54 41 85 eb 51 b8 92 03 1c 41 14 ae
#>   [529] 47 e1 44 a1 54 41 66 66 66 66 dc 02 1c 41 c3 f5 28 3c fe a0 54 41 71 3d
#>   [553] 0a d7 d5 02 1c 41 1f 85 eb b1 fb a0 54 41 3d 0a d7 a3 a8 02 1c 41 b8 1e
#>   [577] 85 2b ea a0 54 41 b8 1e 85 eb e3 01 1c 41 f6 28 5c ef 9d a0 54 41 5c 8f
#>   [601] c2 f5 ca 01 1c 41 0a d7 a3 40 94 a0 54 41 8f c2 f5 28 c5 01 1c 41 33 33
#>   [625] 33 c3 91 a0 54 41 0a d7 a3 70 a7 00 1c 41 c3 f5 28 3c 16 a0 54 41 8f c2
#>   [649] f5 28 77 ff 1b 41 c3 f5 28 1c 9b 9f 54 41 0a d7 a3 70 64 ff 1b 41 1f 85
#>   [673] eb 41 94 9f 54 41 8f c2 f5 28 56 ff 1b 41 3d 0a d7 23 8f 9f 54 41 14 ae
#>   [697] 47 e1 88 fe 1b 41 29 5c 8f 62 46 9f 54 41 29 5c 8f c2 7f fe 1b 41 66 66
#>   [721] 66 c6 42 9f 54 41 ae 47 e1 7a 7a fe 1b 41 d7 a3 70 ad 40 9f 54 41 8f c2
#>   [745] f5 28 76 fe 1b 41 b8 1e 85 fb 3e 9f 54 41 7b 14 ae 47 72 fd 1b 41 ae 47
#>   [769] e1 7a 41 9f 54 41 52 b8 1e 85 e7 fb 1b 41 b8 1e 85 9b 48 9f 54 41 ec 51
#>   [793] b8 1e 70 fb 1b 41 ae 47 e1 ba 4c 9f 54 41 8f c2 f5 28 2a fb 1b 41 b8 1e
#>   [817] 85 9b 55 9f 54 41 d7 a3 70 3d 14 fb 1b 41 b8 1e 85 bb 5e 9f 54 41 ae 47
#>   [841] e1 7a bd fa 1b 41 b8 1e 85 3b 67 9f 54 41 66 66 66 66 73 fa 1b 41 b8 1e
#>   [865] 85 7b 68 9f 54 41 14 ae 47 e1 91 f9 1b 41 b8 1e 85 db 68 9f 54 41 ec 51
#>   [889] b8 1e 0a f8 1b 41 b8 1e 85 1b 6a 9f 54 41 71 3d 0a d7 d1 f6 1b 41 c3 f5
#>   [913] 28 7c 63 9f 54 41 85 eb 51 b8 29 f6 1b 41 b8 1e 85 7b 5f 9f 54 41 33 33
#>   [937] 33 33 df f5 1b 41 b8 1e 85 bb 60 9f 54 41 0a d7 a3 70 db f5 1b 41 9a 99
#>   [961] 99 79 65 9f 54 41 a4 70 3d 0a d3 f5 1b 41 52 b8 1e 45 66 9f 54 41 0a d7
#>   [985] a3 70 bb f5 1b 41 c3 f5 28 1c 69 9f 54 41 52 b8 1e 85 ae f5 1b 41 1f 85
#>  [1009] eb 71 6b 9f 54 41 00 00 00 00 ad f5 1b 41 48 e1 7a 04 6c 9f 54 41 52 b8
#>  [1033] 1e 85 ae f5 1b 41 ae 47 e1 ba 71 9f 54 41 f6 28 5c 8f af f5 1b 41 b8 1e
#>  [1057] 85 eb 72 9f 54 41 85 eb 51 b8 c4 f5 1b 41 00 00 00 00 7b 9f 54 41 00 00
#>  [1081] 00 00 c6 f5 1b 41 00 00 00 f0 7b 9f 54 41 d7 a3 70 3d c6 f5 1b 41 85 eb
#>  [1105] 51 38 7f 9f 54 41 ae 47 e1 7a c5 f5 1b 41 0a d7 a3 30 81 9f 54 41 85 eb
#>  [1129] 51 b8 c3 f5 1b 41 71 3d 0a c7 81 9f 54 41 cd cc cc cc b7 f5 1b 41 ae 47
#>  [1153] e1 aa 84 9f 54 41 e1 7a 14 ae b0 f5 1b 41 e1 7a 14 6e 85 9f 54 41 f6 28
#>  [1177] 5c 8f a8 f5 1b 41 3d 0a d7 f3 85 9f 54 41 48 e1 7a 14 9b f5 1b 41 a4 70
#>  [1201] 3d 7a 86 9f 54 41 33 33 33 33 92 f5 1b 41 71 3d 0a b7 86 9f 54 41 48 e1
#>  [1225] 7a 14 8b f5 1b 41 b8 1e 85 cb 86 9f 54 41 7b 14 ae 47 0f f5 1b 41 e1 7a
#>  [1249] 14 4e 87 9f 54 41 c3 f5 28 5c 02 f5 1b 41 71 3d 0a 87 87 9f 54 41 ae 47
#>  [1273] e1 7a fc f4 1b 41 3d 0a d7 b3 87 9f 54 41 5c 8f c2 f5 f7 f4 1b 41 85 eb
#>  [1297] 51 e8 87 9f 54 41 48 e1 7a 14 f3 f4 1b 41 e1 7a 14 3e 88 9f 54 41 8f c2
#>  [1321] f5 28 e5 f4 1b 41 ae 47 e1 6a 89 9f 54 41 3d 0a d7 a3 c5 f4 1b 41 f6 28
#>  [1345] 5c 9f 8c 9f 54 41 7b 14 ae 47 be f4 1b 41 66 66 66 76 8d 9f 54 41 5c 8f
#>  [1369] c2 f5 98 f4 1b 41 8f c2 f5 e8 93 9f 54 41 a4 70 3d 0a 8f f4 1b 41 66 66
#>  [1393] 66 76 96 9f 54 41 c3 f5 28 5c 4f f4 1b 41 14 ae 47 01 a8 9f 54 41 66 66
#>  [1417] 66 66 2c f4 1b 41 85 eb 51 b8 b7 9f 54 41 ae 47 e1 7a 20 f4 1b 41 a4 70
#>  [1441] 3d 2a ba 9f 54 41 66 66 66 66 0b f4 1b 41 8f c2 f5 e8 be 9f 54 41 66 66
#>  [1465] 66 66 fc f3 1b 41 1f 85 eb 31 c1 9f 54 41 c3 f5 28 5c c6 f3 1b 41 5c 8f
#>  [1489] c2 35 cc 9f 54 41 c3 f5 28 5c b5 f3 1b 41 7b 14 ae 97 d0 9f 54 41 3d 0a
#>  [1513] d7 a3 87 f3 1b 41 7b 14 ae 97 d6 9f 54 41 9a 99 99 99 85 f3 1b 41 d7 a3
#>  [1537] 70 0d d7 9f 54 41 f6 28 5c 8f 82 f3 1b 41 a4 70 3d 2a d8 9f 54 41 5c 8f
#>  [1561] c2 f5 56 f3 1b 41 ec 51 b8 4e ec 9f 54 41 0a d7 a3 70 54 f3 1b 41 e1 7a
#>  [1585] 14 2e ed 9f 54 41 66 66 66 66 00 f3 1b 41 8f c2 f5 e8 fd 9f 54 41 f6 28
#>  [1609] 5c 8f e8 f2 1b 41 b8 1e 85 5b 04 a0 54 41 00 00 00 00 e5 f2 1b 41 e1 7a
#>  [1633] 14 1e 05 a0 54 41 ae 47 e1 7a d3 f2 1b 41 9a 99 99 09 07 a0 54 41 85 eb
#>  [1657] 51 b8 41 f2 1b 41 33 33 33 53 16 a0 54 41 3d 0a d7 a3 1e f2 1b 41 71 3d
#>  [1681] 0a 37 19 a0 54 41 c3 f5 28 5c 1c f2 1b 41 8f c2 f5 98 19 a0 54 41 7b 14
#>  [1705] ae 47 17 f2 1b 41 ec 51 b8 0e 1a a0 54 41 b8 1e 85 eb a0 f1 1b 41 3d 0a
#>  [1729] d7 43 23 a0 54 41 85 eb 51 b8 95 f1 1b 41 00 00 00 60 24 a0 54 41 85 eb
#>  [1753] 51 b8 75 f1 1b 41 ec 51 b8 fe 27 a0 54 41 48 e1 7a 14 6a f1 1b 41 7b 14
#>  [1777] ae 37 29 a0 54 41 5c 8f c2 f5 45 f1 1b 41 a4 70 3d ea 2b a0 54 41 71 3d
#>  [1801] 0a d7 1c f1 1b 41 c3 f5 28 3c 30 a0 54 41 f6 28 5c 8f fd f0 1b 41 c3 f5
#>  [1825] 28 4c 32 a0 54 41 d7 a3 70 3d e8 f0 1b 41 c3 f5 28 4c 32 a0 54 41 33 33
#>  [1849] 33 33 b9 f0 1b 41 d7 a3 70 6d 32 a0 54 41 48 e1 7a 14 b0 f0 1b 41 d7 a3
#>  [1873] 70 8d 32 a0 54 41 85 eb 51 b8 a8 f0 1b 41 ec 51 b8 de 32 a0 54 41 d7 a3
#>  [1897] 70 3d 8b f0 1b 41 ae 47 e1 ba 34 a0 54 41 e1 7a 14 ae 86 f0 1b 41 c3 f5
#>  [1921] 28 1c 35 a0 54 41 48 e1 7a 14 82 f0 1b 41 66 66 66 a6 35 a0 54 41 1f 85
#>  [1945] eb 51 41 f0 1b 41 e1 7a 14 9e 3a a0 54 41 1f 85 eb 51 33 f0 1b 41 b8 1e
#>  [1969] 85 eb 3b a0 54 41 14 ae 47 e1 06 f0 1b 41 5c 8f c2 75 40 a0 54 41 f6 28
#>  [1993] 5c 8f 02 f0 1b 41 ae 47 e1 ca 40 a0 54 41 e1 7a 14 ae b8 ef 1b 41 8f c2
#>  [2017] f5 48 45 a0 54 41 8f c2 f5 28 9b ef 1b 41 66 66 66 96 46 a0 54 41 f6 28
#>  [2041] 5c 8f 75 ef 1b 41 00 00 00 00 48 a0 54 41 ec 51 b8 1e 68 ef 1b 41 ec 51
#>  [2065] b8 ae 48 a0 54 41 29 5c 8f c2 33 ef 1b 41 5c 8f c2 55 4c a0 54 41 33 33
#>  [2089] 33 33 2e ef 1b 41 00 00 00 f0 4c a0 54 41 a4 70 3d 0a 25 ef 1b 41 c3 f5
#>  [2113] 28 fc 4c a0 54 41 ae 47 e1 7a d5 ee 1b 41 14 ae 47 11 4c a0 54 41 8f c2
#>  [2137] f5 28 c2 ee 1b 41 85 eb 51 e8 4b a0 54 41 d7 a3 70 3d 51 ee 1b 41 8f c2
#>  [2161] f5 c8 4b a0 54 41 9a 99 99 99 33 ee 1b 41 d7 a3 70 ed 4b a0 54 41 48 e1
#>  [2185] 7a 14 84 ec 1b 41 cd cc cc 9c 4e a0 54 41 c3 f5 28 5c 8f ea 1b 41 d7 a3
#>  [2209] 70 2d 4e a0 54 41 ae 47 e1 7a 0b ea 1b 41 3d 0a d7 b3 4e a0 54 41 b8 1e
#>  [2233] 85 eb e9 e9 1b 41 48 e1 7a e4 4e a0 54 41 e1 7a 14 ae b6 e8 1b 41 c3 f5
#>  [2257] 28 7c 51 a0 54 41 8f c2 f5 28 b0 e7 1b 41 1f 85 eb b1 51 a0 54 41 52 b8
#>  [2281] 1e 85 a8 e7 1b 41 29 5c 8f c2 51 a0 54 41 e1 7a 14 ae 35 e7 1b 41 71 3d
#>  [2305] 0a d7 53 a0 54 41 ec 51 b8 1e 23 e7 1b 41 52 b8 1e 55 54 a0 54 41 0a d7
#>  [2329] a3 70 e9 e6 1b 41 48 e1 7a 14 56 a0 54 41 cd cc cc cc d3 e6 1b 41 7b 14
#>  [2353] ae e7 56 a0 54 41 71 3d 0a d7 66 e6 1b 41 66 66 66 86 5b a0 54 41 5c 8f
#>  [2377] c2 f5 2f e6 1b 41 71 3d 0a 97 5e a0 54 41 3d 0a d7 a3 1d e6 1b 41 66 66
#>  [2401] 66 66 5f a0 54 41 cd cc cc cc d7 e5 1b 41 b8 1e 85 ab 63 a0 54 41 ae 47
#>  [2425] e1 7a d3 e5 1b 41 14 ae 47 01 64 a0 54 41 3d 0a d7 a3 cd e5 1b 41 f6 28
#>  [2449] 5c 9f 64 a0 54 41 f6 28 5c 8f a9 e5 1b 41 9a 99 99 29 67 a0 54 41 0a d7
#>  [2473] a3 70 a1 e5 1b 41 c3 f5 28 cc 67 a0 54 41 29 5c 8f c2 85 e5 1b 41 a4 70
#>  [2497] 3d 5a 6a a0 54 41 e1 7a 14 ae 42 e5 1b 41 85 eb 51 a8 6f a0 54 41 9a 99
#>  [2521] 99 99 3c e5 1b 41 e1 7a 14 3e 70 a0 54 41 66 66 66 66 30 e5 1b 41 48 e1
#>  [2545] 7a c4 71 a0 54 41 29 5c 8f c2 06 e5 1b 41 52 b8 1e f5 75 a0 54 41 1f 85
#>  [2569] eb 51 dc e4 1b 41 5c 8f c2 05 79 a0 54 41 d7 a3 70 3d d7 e4 1b 41 00 00
#>  [2593] 00 a0 79 a0 54 41 e1 7a 14 ae d2 e4 1b 41 d7 a3 70 fd 79 a0 54 41 f6 28
#>  [2617] 5c 8f 4d e4 1b 41 9a 99 99 09 83 a0 54 41 c3 f5 28 5c 41 e4 1b 41 8f c2
#>  [2641] f5 c8 83 a0 54 41 00 00 00 00 2d e4 1b 41 85 eb 51 98 84 a0 54 41 a4 70
#>  [2665] 3d 0a b1 e3 1b 41 29 5c 8f 12 89 a0 54 41 1f 85 eb 51 cb e2 1b 41 3d 0a
#>  [2689] d7 33 90 a0 54 41 d7 a3 70 3d cd e1 1b 41 ec 51 b8 ce 99 a0 54 41 48 e1
#>  [2713] 7a 14 96 e1 1b 41 29 5c 8f a2 9b a0 54 41 52 b8 1e 85 82 e1 1b 41 9a 99
#>  [2737] 99 59 9c a0 54 41 c3 f5 28 5c dd e0 1b 41 e1 7a 14 4e a3 a0 54 41 52 b8
#>  [2761] 1e 85 c6 e0 1b 41 9a 99 99 29 a4 a0 54 41 cd cc cc cc 6c e0 1b 41 14 ae
#>  [2785] 47 21 a7 a0 54 41 d7 a3 70 3d 58 e0 1b 41 00 00 00 e0 a7 a0 54 41 8f c2
#>  [2809] f5 28 b8 df 1b 41 ec 51 b8 4e af a0 54 41 3d 0a d7 a3 39 df 1b 41 ec 51
#>  [2833] b8 3e b4 a0 54 41 0a d7 a3 70 2d df 1b 41 48 e1 7a c4 b4 a0 54 41 00 00
#>  [2857] 00 00 23 df 1b 41 ae 47 e1 5a b5 a0 54 41 52 b8 1e 85 d4 de 1b 41 33 33
#>  [2881] 33 43 ba a0 54 41 f6 28 5c 8f c8 de 1b 41 29 5c 8f 12 bb a0 54 41 a4 70
#>  [2905] 3d 0a c6 de 1b 41 f6 28 5c 5f bb a0 54 41 d7 a3 70 3d c3 de 1b 41 9a 99
#>  [2929] 99 e9 bb a0 54 41 33 33 33 33 c0 de 1b 41 33 33 33 33 bc a0 54 41 7b 14
#>  [2953] ae 47 96 de 1b 41 29 5c 8f 42 bf a0 54 41 1f 85 eb 51 89 de 1b 41 00 00
#>  [2977] 00 80 c0 a0 54 41 b8 1e 85 eb 80 de 1b 41 29 5c 8f 32 c1 a0 54 41 48 e1
#>  [3001] 7a 14 5b de 1b 41 9a 99 99 e9 c3 a0 54 41 00 00 00 00 56 de 1b 41 85 eb
#>  [3025] 51 98 c4 a0 54 41 cd cc cc cc 39 de 1b 41 14 ae 47 d1 c6 a0 54 41 d7 a3
#>  [3049] 70 3d 25 de 1b 41 a4 70 3d ea c8 a0 54 41 66 66 66 66 1f de 1b 41 85 eb
#>  [3073] 51 a8 c9 a0 54 41 0a d7 a3 70 13 de 1b 41 85 eb 51 88 cb a0 54 41 14 ae
#>  [3097] 47 e1 9e dd 1b 41 5c 8f c2 d5 d9 a0 54 41 b8 1e 85 eb 8d dd 1b 41 9a 99
#>  [3121] 99 d9 dc a0 54 41 9a 99 99 99 6a dd 1b 41 ae 47 e1 3a e5 a0 54 41 33 33
#>  [3145] 33 33 60 dd 1b 41 71 3d 0a 37 e7 a0 54 41 9a 99 99 99 1a dd 1b 41 33 33
#>  [3169] 33 33 eb a0 54 41 0a d7 a3 70 10 dd 1b 41 9a 99 99 b9 eb a0 54 41 d7 a3
#>  [3193] 70 3d f5 dc 1b 41 5c 8f c2 c5 ec a0 54 41 9a 99 99 99 be dc 1b 41 85 eb
#>  [3217] 51 88 ee a0 54 41 9a 99 99 99 bc dc 1b 41 f6 28 5c 3f f0 a0 54 41 0a d7
#>  [3241] a3 70 b2 dc 1b 41 71 3d 0a 27 f3 a0 54 41 48 e1 7a 14 a9 dc 1b 41 e1 7a
#>  [3265] 14 de f4 a0 54 41 48 e1 7a 14 ab dc 1b 41 ec 51 b8 1e f6 a0 54 41 33 33
#>  [3289] 33 33 b2 dc 1b 41 d7 a3 70 bd f6 a0 54 41 9a 99 99 99 ba dc 1b 41 52 b8
#>  [3313] 1e a5 f6 a0 54 41 ae 47 e1 7a d3 dc 1b 41 52 b8 1e c5 f3 a0 54 41 9a 99
#>  [3337] 99 99 d9 dc 1b 41 48 e1 7a 94 f3 a0 54 41 d7 a3 70 3d f2 dc 1b 41 cd cc
#>  [3361] cc ac f3 a0 54 41 c3 f5 28 5c f9 dc 1b 41 1f 85 eb e1 f3 a0 54 41 29 5c
#>  [3385] 8f c2 01 dd 1b 41 00 00 00 70 f4 a0 54 41 1f 85 eb 51 05 dd 1b 41 e1 7a
#>  [3409] 14 ee f4 a0 54 41 48 e1 7a 14 06 dd 1b 41 8f c2 f5 a8 f5 a0 54 41 9a 99
#>  [3433] 99 99 05 dd 1b 41 d7 a3 70 dd f7 a0 54 41 52 b8 1e 85 01 dd 1b 41 8f c2
#>  [3457] f5 a8 f9 a0 54 41 71 3d 0a d7 c1 dc 1b 41 33 33 33 53 0c a1 54 41 a4 70
#>  [3481] 3d 0a be dc 1b 41 a4 70 3d fa 0c a1 54 41 f6 28 5c 8f af dc 1b 41 5c 8f
#>  [3505] c2 e5 0e a1 54 41 d7 a3 70 3d 9a dc 1b 41 f6 28 5c 3f 11 a1 54 41 48 e1
#>  [3529] 7a 14 9f dc 1b 41 9a 99 99 c9 11 a1 54 41 00 00 00 00 b8 dc 1b 41 b8 1e
#>  [3553] 85 6b 15 a1 54 41 5c 8f c2 f5 a6 dc 1b 41 b8 1e 85 7b 16 a1 54 41 5c 8f
#>  [3577] c2 f5 a5 dc 1b 41 14 ae 47 d1 16 a1 54 41 b8 1e 85 eb ae dc 1b 41 52 b8
#>  [3601] 1e c5 1e a1 54 41 5c 8f c2 f5 b1 dc 1b 41 d7 a3 70 0d 20 a1 54 41 1f 85
#>  [3625] eb 51 d5 dc 1b 41 66 66 66 46 28 a1 54 41 c3 f5 28 5c d6 dc 1b 41 8f c2
#>  [3649] f5 e8 28 a1 54 41 b8 1e 85 eb d7 dc 1b 41 14 ae 47 11 2c a1 54 41 14 ae
#>  [3673] 47 e1 d4 dc 1b 41 8f c2 f5 f8 2c a1 54 41 85 eb 51 b8 ba dc 1b 41 52 b8
#>  [3697] 1e e5 31 a1 54 41 14 ae 47 e1 b5 dc 1b 41 66 66 66 56 32 a1 54 41 71 3d
#>  [3721] 0a d7 a3 dc 1b 41 71 3d 0a 67 33 a1 54 41 e1 7a 14 ae 98 dc 1b 41 14 ae
#>  [3745] 47 f1 33 a1 54 41 5c 8f c2 f5 55 db 1b 41 1f 85 eb e1 40 a1 54 41 1f 85
#>  [3769] eb 51 26 da 1b 41 71 3d 0a e7 48 a1 54 41 1f 85 eb 51 d9 d9 1b 41 ae 47
#>  [3793] e1 ea 4a a1 54 41 3d 0a d7 a3 13 d9 1b 41 5c 8f c2 85 51 a1 54 41 71 3d
#>  [3817] 0a d7 00 d9 1b 41 c3 f5 28 1c 52 a1 54 41 29 5c 8f c2 cc d8 1b 41 52 b8
#>  [3841] 1e 55 53 a1 54 41 71 3d 0a d7 c3 d8 1b 41 14 ae 47 71 53 a1 54 41 85 eb
#>  [3865] 51 b8 ba d8 1b 41 14 ae 47 71 53 a1 54 41 c3 f5 28 5c 96 d8 1b 41 8f c2
#>  [3889] f5 48 53 a1 54 41 c3 f5 28 5c 59 d8 1b 41 85 eb 51 18 52 a1 54 41 3d 0a
#>  [3913] d7 a3 4c d8 1b 41 b8 1e 85 eb 51 a1 54 41 c3 f5 28 5c 3d d8 1b 41 3d 0a
#>  [3937] d7 e3 51 a1 54 41 d7 a3 70 3d 24 d8 1b 41 00 00 00 f0 51 a1 54 41 14 ae
#>  [3961] 47 e1 1d d8 1b 41 85 eb 51 08 52 a1 54 41 48 e1 7a 14 0c d8 1b 41 a4 70
#>  [3985] 3d 7a 52 a1 54 41 ae 47 e1 7a 98 d7 1b 41 00 00 00 c0 55 a1 54 41 1f 85
#>  [4009] eb 51 8f d7 1b 41 14 ae 47 21 56 a1 54 41 52 b8 1e 85 8a d7 1b 41 29 5c
#>  [4033] 8f 72 56 a1 54 41 f6 28 5c 8f 70 d7 1b 41 ec 51 b8 7e 58 a1 54 41 52 b8
#>  [4057] 1e 85 6a d7 1b 41 d7 a3 70 0d 59 a1 54 41 b8 1e 85 eb 64 d7 1b 41 52 b8
#>  [4081] 1e 05 5a a1 54 41 e1 7a 14 ae 54 d7 1b 41 29 5c 8f 72 5e a1 54 41 3d 0a
#>  [4105] d7 a3 52 d7 1b 41 7b 14 ae b7 5e a1 54 41 7b 14 ae 47 4d d7 1b 41 d7 a3
#>  [4129] 70 1d 5f a1 54 41 66 66 66 66 44 d7 1b 41 71 3d 0a 97 5f a1 54 41 1f 85
#>  [4153] eb 51 3e d7 1b 41 3d 0a d7 d3 5f a1 54 41 66 66 66 66 34 d7 1b 41 8f c2
#>  [4177] f5 08 60 a1 54 41 ec 51 b8 1e e7 d6 1b 41 9a 99 99 29 61 a1 54 41 b8 1e
#>  [4201] 85 eb d5 d5 1b 41 33 33 33 23 63 a1 54 41 85 eb 51 b8 b9 d5 1b 41 7b 14
#>  [4225] ae 57 63 a1 54 41 7b 14 ae 47 ae d5 1b 41 00 00 00 80 63 a1 54 41 cd cc
#>  [4249] cc cc 51 d5 1b 41 33 33 33 33 65 a1 54 41 9a 99 99 99 45 d5 1b 41 00 00
#>  [4273] 00 60 65 a1 54 41 e1 7a 14 ae 39 d5 1b 41 48 e1 7a 64 65 a1 54 41 0a d7
#>  [4297] a3 70 ee d4 1b 41 7b 14 ae 27 65 a1 54 41 e1 7a 14 ae de d4 1b 41 3d 0a
#>  [4321] d7 33 65 a1 54 41 52 b8 1e 85 97 d4 1b 41 48 e1 7a 54 66 a1 54 41 ec 51
#>  [4345] b8 1e f5 d3 1b 41 33 33 33 03 69 a1 54 41 66 66 66 66 1f d3 1b 41 48 e1
#>  [4369] 7a 34 6b a1 54 41 48 e1 7a 14 af d2 1b 41 00 00 00 e0 6c a1 54 41 d7 a3
#>  [4393] 70 3d 8a d2 1b 41 e1 7a 14 5e 6d a1 54 41 00 00 00 00 4e d2 1b 41 ec 51
#>  [4417] b8 7e 6d a1 54 41 29 5c 8f c2 3e d2 1b 41 ae 47 e1 7a 6d a1 54 41 71 3d
#>  [4441] 0a d7 33 d2 1b 41 e1 7a 14 4e 6d a1 54 41 1f 85 eb 51 f5 d1 1b 41 0a d7
#>  [4465] a3 00 6c a1 54 41 14 ae 47 e1 ea d1 1b 41 48 e1 7a d4 6b a1 54 41 00 00
#>  [4489] 00 00 c4 d1 1b 41 7b 14 ae a7 6b a1 54 41 71 3d 0a d7 ba d1 1b 41 00 00
#>  [4513] 00 b0 6b a1 54 41 00 00 00 00 b5 d1 1b 41 c3 f5 28 cc 6b a1 54 41 8f c2
#>  [4537] f5 28 ae d1 1b 41 52 b8 1e 05 6c a1 54 41 29 5c 8f c2 67 d1 1b 41 cd cc
#>  [4561] cc dc 6e a1 54 41 5c 8f c2 f5 24 d1 1b 41 a4 70 3d 1a 71 a1 54 41 5c 8f
#>  [4585] c2 f5 0a d1 1b 41 66 66 66 16 72 a1 54 41 5c 8f c2 f5 9b d0 1b 41 ae 47
#>  [4609] e1 4a 76 a1 54 41 52 b8 1e 85 14 d0 1b 41 3d 0a d7 63 7a a1 54 41 14 ae
#>  [4633] 47 e1 42 cf 1b 41 8f c2 f5 78 7f a1 54 41 7b 14 ae 47 1d cf 1b 41 48 e1
#>  [4657] 7a 54 80 a1 54 41 14 ae 47 e1 d3 ce 1b 41 1f 85 eb b1 81 a1 54 41 ec 51
#>  [4681] b8 1e cf cd 1b 41 52 b8 1e 35 85 a1 54 41 9a 99 99 99 51 cd 1b 41 3d 0a
#>  [4705] d7 e3 87 a1 54 41 c3 f5 28 5c b9 cc 1b 41 85 eb 51 c8 8a a1 54 41 3d 0a
#>  [4729] d7 a3 aa cc 1b 41 52 b8 1e 05 8b a1 54 41 3d 0a d7 a3 99 cc 1b 41 9a 99
#>  [4753] 99 19 8b a1 54 41 8f c2 f5 28 c5 cb 1b 41 33 33 33 53 8b a1 54 41 e1 7a
#>  [4777] 14 ae b9 cb 1b 41 ae 47 e1 3a 8b a1 54 41 52 b8 1e 85 a0 cb 1b 41 85 eb
#>  [4801] 51 88 8a a1 54 41 66 66 66 66 98 cb 1b 41 29 5c 8f 22 8a a1 54 41 52 b8
#>  [4825] 1e 85 8f cb 1b 41 00 00 00 90 89 a1 54 41 52 b8 1e 85 82 cb 1b 41 f6 28
#>  [4849] 5c 4f 88 a1 54 41 a4 70 3d 0a 74 cb 1b 41 3d 0a d7 c3 86 a1 54 41 c3 f5
#>  [4873] 28 5c 3e cb 1b 41 ae 47 e1 0a 81 a1 54 41 85 eb 51 b8 25 cb 1b 41 1f 85
#>  [4897] eb c1 7d a1 54 41 7b 14 ae 47 0c cb 1b 41 29 5c 8f 12 7a a1 54 41 9a 99
#>  [4921] 99 99 f5 ca 1b 41 33 33 33 13 75 a1 54 41 cd cc cc cc f1 ca 1b 41 48 e1
#>  [4945] 7a 64 74 a1 54 41 e1 7a 14 ae eb ca 1b 41 e1 7a 14 ae 73 a1 54 41 ae 47
#>  [4969] e1 7a b1 ca 1b 41 1f 85 eb c1 6d a1 54 41 c3 f5 28 5c ad ca 1b 41 29 5c
#>  [4993] 8f f2 6c a1 54 41 f6 28 5c 8f a9 ca 1b 41 ec 51 b8 ee 6b a1 54 41 1f 85
#>  [5017] eb 51 a9 ca 1b 41 d7 a3 70 8d 6b a1 54 41 1f 85 eb 51 ab ca 1b 41 ae 47
#>  [5041] e1 0a 6b a1 54 41 14 ae 47 e1 ae ca 1b 41 9a 99 99 a9 6a a1 54 41 1f 85
#>  [5065] eb 51 b9 ca 1b 41 ae 47 e1 fa 69 a1 54 41 66 66 66 66 be ca 1b 41 e1 7a
#>  [5089] 14 be 69 a1 54 41 85 eb 51 b8 c2 ca 1b 41 14 ae 47 a1 69 a1 54 41 0a d7
#>  [5113] a3 70 d2 ca 1b 41 8f c2 f5 88 69 a1 54 41 0a d7 a3 70 e0 ca 1b 41 cd cc
#>  [5137] cc 8c 69 a1 54 41 1f 85 eb 51 e8 ca 1b 41 5c 8f c2 c5 69 a1 54 41 52 b8
#>  [5161] 1e 85 f2 ca 1b 41 7b 14 ae 47 6a a1 54 41 48 e1 7a 14 f8 ca 1b 41 8f c2
#>  [5185] f5 78 6a a1 54 41 d7 a3 70 3d 02 cb 1b 41 cd cc cc 8c 6a a1 54 41 cd cc
#>  [5209] cc cc 13 cb 1b 41 0a d7 a3 80 6a a1 54 41 33 33 33 33 1b cb 1b 41 48 e1
#>  [5233] 7a 64 6a a1 54 41 a4 70 3d 0a 24 cb 1b 41 71 3d 0a 17 6a a1 54 41 71 3d
#>  [5257] 0a d7 35 cb 1b 41 71 3d 0a 27 69 a1 54 41 66 66 66 66 3a cb 1b 41 66 66
#>  [5281] 66 c6 68 a1 54 41 ec 51 b8 1e 49 cb 1b 41 d7 a3 70 9d 66 a1 54 41 33 33
#>  [5305] 33 33 5e cb 1b 41 00 00 00 60 62 a1 54 41 14 ae 47 e1 79 cb 1b 41 7b 14
#>  [5329] ae 27 5b a1 54 41 7b 14 ae 47 84 cb 1b 41 66 66 66 e6 57 a1 54 41 71 3d
#>  [5353] 0a d7 87 cb 1b 41 3d 0a d7 33 57 a1 54 41 52 b8 1e 85 93 cb 1b 41 ae 47
#>  [5377] e1 fa 55 a1 54 41 f6 28 5c 8f c6 cb 1b 41 ec 51 b8 0e 51 a1 54 41 5c 8f
#>  [5401] c2 f5 d0 cb 1b 41 71 3d 0a f7 4f a1 54 41 71 3d 0a d7 18 cc 1b 41 f6 28
#>  [5425] 5c 2f 46 a1 54 41 7b 14 ae 47 48 cc 1b 41 71 3d 0a 27 3b a1 54 41 48 e1
#>  [5449] 7a 14 4b cc 1b 41 f6 28 5c 2f 3a a1 54 41 ae 47 e1 7a 57 cc 1b 41 52 b8
#>  [5473] 1e 95 34 a1 54 41 7b 14 ae 47 5d cc 1b 41 52 b8 1e b5 2e a1 54 41 00 00
#>  [5497] 00 00 5a cc 1b 41 85 eb 51 78 2c a1 54 41 71 3d 0a d7 52 cc 1b 41 33 33
#>  [5521] 33 03 2a a1 54 41 48 e1 7a 14 50 cc 1b 41 48 e1 7a 64 29 a1 54 41 5c 8f
#>  [5545] c2 f5 4b cc 1b 41 71 3d 0a 07 29 a1 54 41 ae 47 e1 7a 4b cc 1b 41 1f 85
#>  [5569] eb b1 28 a1 54 41 33 33 33 33 4a cc 1b 41 8f c2 f5 78 28 a1 54 41 48 e1
#>  [5593] 7a 14 44 cc 1b 41 33 33 33 f3 27 a1 54 41 85 eb 51 b8 2c cc 1b 41 85 eb
#>  [5617] 51 48 26 a1 54 41 b8 1e 85 eb 29 cc 1b 41 3d 0a d7 23 26 a1 54 41 a4 70
#>  [5641] 3d 0a 23 cc 1b 41 71 3d 0a f7 25 a1 54 41 29 5c 8f c2 11 cc 1b 41 1f 85
#>  [5665] eb b1 25 a1 54 41 71 3d 0a d7 ba cb 1b 41 7b 14 ae 27 25 a1 54 41 00 00
#>  [5689] 00 00 3c cb 1b 41 14 ae 47 61 23 a1 54 41 c3 f5 28 5c 31 cb 1b 41 85 eb
#>  [5713] 51 28 23 a1 54 41 ae 47 e1 7a df ca 1b 41 29 5c 8f a2 20 a1 54 41 e1 7a
#>  [5737] 14 ae ce ca 1b 41 00 00 00 10 20 a1 54 41 8f c2 f5 28 9e ca 1b 41 71 3d
#>  [5761] 0a a7 1d a1 54 41 66 66 66 66 61 ca 1b 41 71 3d 0a 87 1a a1 54 41 1f 85
#>  [5785] eb 51 a9 c9 1b 41 ae 47 e1 4a 0e a1 54 41 0a d7 a3 70 92 c9 1b 41 ae 47
#>  [5809] e1 4a 0d a1 54 41 a4 70 3d 0a 30 c9 1b 41 a4 70 3d 1a 08 a1 54 41 00 00
#>  [5833] 00 00 d3 c8 1b 41 d7 a3 70 bd 03 a1 54 41 f6 28 5c 8f 5f c8 1b 41 52 b8
#>  [5857] 1e a5 fd a0 54 41 0a d7 a3 70 29 c8 1b 41 ae 47 e1 1a fb a0 54 41 b8 1e
#>  [5881] 85 eb 1a c8 1b 41 52 b8 1e a5 fa a0 54 41 66 66 66 66 dc c7 1b 41 c3 f5
#>  [5905] 28 1c f9 a0 54 41 ec 51 b8 1e ad c7 1b 41 29 5c 8f d2 f7 a0 54 41 29 5c
#>  [5929] 8f c2 6a c7 1b 41 00 00 00 20 f5 a0 54 41 c3 f5 28 5c 61 c7 1b 41 71 3d
#>  [5953] 0a d7 f4 a0 54 41 29 5c 8f c2 49 c7 1b 41 9a 99 99 79 f4 a0 54 41 71 3d
#>  [5977] 0a d7 f4 c6 1b 41 1f 85 eb 71 f3 a0 54 41 3d 0a d7 a3 c8 c6 1b 41 c3 f5
#>  [6001] 28 0c f3 a0 54 41 71 3d 0a d7 a7 c6 1b 41 ae 47 e1 9a f2 a0 54 41 48 e1
#>  [6025] 7a 14 98 c6 1b 41 29 5c 8f 82 f2 a0 54 41 33 33 33 33 80 c6 1b 41 5c 8f
#>  [6049] c2 65 f2 a0 54 41 00 00 00 00 66 c6 1b 41 14 ae 47 41 f2 a0 54 41 ae 47
#>  [6073] e1 7a 47 c6 1b 41 d7 a3 70 4d f1 a0 54 41 33 33 33 33 26 c6 1b 41 9a 99
#>  [6097] 99 59 f0 a0 54 41 00 00 00 00 19 c6 1b 41 33 33 33 a3 ef a0 54 41 66 66
#>  [6121] 66 66 04 c6 1b 41 1f 85 eb 41 ef a0 54 41 14 ae 47 e1 e4 c5 1b 41 85 eb
#>  [6145] 51 d8 ee a0 54 41 71 3d 0a d7 c4 c5 1b 41 1f 85 eb 51 ee a0 54 41 48 e1
#>  [6169] 7a 14 96 c5 1b 41 1f 85 eb 21 ed a0 54 41 ec 51 b8 1e f0 c4 1b 41 7b 14
#>  [6193] ae 87 e9 a0 54 41 52 b8 1e 85 d9 c3 1b 41 1f 85 eb d1 e4 a0 54 41 f6 28
#>  [6217] 5c 8f 9c c2 1b 41 66 66 66 86 e0 a0 54 41 7b 14 ae 47 8b c2 1b 41 0a d7
#>  [6241] a3 20 e0 a0 54 41 52 b8 1e 85 11 c2 1b 41 33 33 33 c3 dc a0 54 41 b8 1e
#>  [6265] 85 eb 81 c1 1b 41 3d 0a d7 c3 d9 a0 54 41 ae 47 e1 7a 77 c1 1b 41 f6 28
#>  [6289] 5c af d9 a0 54 41 3d 0a d7 a3 33 c1 1b 41 48 e1 7a d4 d9 a0 54 41 33 33
#>  [6313] 33 33 2a c1 1b 41 8f c2 f5 f8 d9 a0 54 41 14 ae 47 e1 23 c1 1b 41 9a 99
#>  [6337] 99 29 da a0 54 41 52 b8 1e 85 fe c0 1b 41 b8 1e 85 8b db a0 54 41 ae 47
#>  [6361] e1 7a ed c0 1b 41 e1 7a 14 3e dc a0 54 41 8f c2 f5 28 d8 c0 1b 41 b8 1e
#>  [6385] 85 8b dd a0 54 41 c3 f5 28 5c 97 c0 1b 41 0a d7 a3 c0 e1 a0 54 41 52 b8
#>  [6409] 1e 85 8e c0 1b 41 71 3d 0a 87 e2 a0 54 41 85 eb 51 b8 7b c0 1b 41 33 33
#>  [6433] 33 83 e4 a0 54 41 3d 0a d7 a3 67 c0 1b 41 1f 85 eb 21 e6 a0 54 41 c3 f5
#>  [6457] 28 5c 56 c0 1b 41 e1 7a 14 1e e8 a0 54 41 0a d7 a3 70 4d c0 1b 41 14 ae
#>  [6481] 47 f1 e8 a0 54 41 ae 47 e1 7a 91 bf 1b 41 66 66 66 26 f7 a0 54 41 71 3d
#>  [6505] 0a d7 86 bf 1b 41 33 33 33 33 f8 a0 54 41 71 3d 0a d7 67 bf 1b 41 c3 f5
#>  [6529] 28 9c fb a0 54 41 29 5c 8f c2 63 bf 1b 41 ec 51 b8 2e fc a0 54 41 3d 0a
#>  [6553] d7 a3 4a bf 1b 41 b8 1e 85 5b 00 a1 54 41 9a 99 99 99 46 bf 1b 41 7b 14
#>  [6577] ae 57 01 a1 54 41 ae 47 e1 7a 3e bf 1b 41 52 b8 1e c5 03 a1 54 41 0a d7
#>  [6601] a3 70 3d bf 1b 41 b8 1e 85 5b 05 a1 54 41 5c 8f c2 f5 3d bf 1b 41 8f c2
#>  [6625] f5 b8 08 a1 54 41 d7 a3 70 3d 3f bf 1b 41 e1 7a 14 0e 09 a1 54 41 cd cc
#>  [6649] cc cc 41 bf 1b 41 7b 14 ae 67 09 a1 54 41 5c 8f c2 f5 49 bf 1b 41 a4 70
#>  [6673] 3d 2a 0a a1 54 41 5c 8f c2 f5 69 bf 1b 41 71 3d 0a 57 0c a1 54 41 71 3d
#>  [6697] 0a d7 70 bf 1b 41 cd cc cc ac 0c a1 54 41 52 b8 1e 85 a8 bf 1b 41 a4 70
#>  [6721] 3d 2a 0e a1 54 41 5c 8f c2 f5 2d c0 1b 41 d7 a3 70 fd 10 a1 54 41 7b 14
#>  [6745] ae 47 40 c0 1b 41 7b 14 ae 87 11 a1 54 41 f6 28 5c 8f 71 c0 1b 41 cd cc
#>  [6769] cc 1c 13 a1 54 41 cd cc cc cc 7f c0 1b 41 71 3d 0a a7 13 a1 54 41 e1 7a
#>  [6793] 14 ae 88 c0 1b 41 9a 99 99 39 14 a1 54 41 71 3d 0a d7 cc c0 1b 41 29 5c
#>  [6817] 8f 72 19 a1 54 41 29 5c 8f c2 01 c1 1b 41 ae 47 e1 8a 1e a1 54 41 85 eb
#>  [6841] 51 b8 1e c1 1b 41 52 b8 1e 55 22 a1 54 41 52 b8 1e 85 20 c1 1b 41 cd cc
#>  [6865] cc 1c 23 a1 54 41 00 00 00 00 1f c1 1b 41 e1 7a 14 8e 23 a1 54 41 ae 47
#>  [6889] e1 7a 0c c1 1b 41 3d 0a d7 f3 25 a1 54 41 8f c2 f5 28 08 c1 1b 41 71 3d
#>  [6913] 0a a7 26 a1 54 41 66 66 66 66 07 c1 1b 41 0a d7 a3 00 27 a1 54 41 8f c2
#>  [6937] f5 28 07 c1 1b 41 29 5c 8f 92 28 a1 54 41 e1 7a 14 ae 08 c1 1b 41 66 66
#>  [6961] 66 76 29 a1 54 41 00 00 00 00 0e c1 1b 41 85 eb 51 08 2a a1 54 41 e1 7a
#>  [6985] 14 ae 28 c1 1b 41 85 eb 51 38 2c a1 54 41 c3 f5 28 5c 31 c1 1b 41 71 3d
#>  [7009] 0a d7 2c a1 54 41 3d 0a d7 a3 52 c1 1b 41 71 3d 0a c7 2e a1 54 41 f6 28
#>  [7033] 5c 8f 5b c1 1b 41 c3 f5 28 2c 2f a1 54 41 14 ae 47 e1 6d c1 1b 41 33 33
#>  [7057] 33 d3 2f a1 54 41 29 5c 8f c2 76 c1 1b 41 85 eb 51 38 30 a1 54 41 14 ae
#>  [7081] 47 e1 7e c1 1b 41 ae 47 e1 da 30 a1 54 41 00 00 00 00 95 c1 1b 41 71 3d
#>  [7105] 0a d7 32 a1 54 41 3d 0a d7 a3 9a c1 1b 41 5c 8f c2 75 33 a1 54 41 ae 47
#>  [7129] e1 7a a1 c1 1b 41 48 e1 7a 34 35 a1 54 41 ec 51 b8 1e 1a c2 1b 41 f6 28
#>  [7153] 5c 1f 4b a1 54 41 3d 0a d7 a3 72 c2 1b 41 33 33 33 33 5a a1 54 41 71 3d
#>  [7177] 0a d7 7e c2 1b 41 1f 85 eb d1 5b a1 54 41 e1 7a 14 ae 85 c2 1b 41 29 5c
#>  [7201] 8f 32 5d a1 54 41 f6 28 5c 8f a6 c2 1b 41 e1 7a 14 ee 65 a1 54 41 c3 f5
#>  [7225] 28 5c a9 c2 1b 41 b8 1e 85 6b 66 a1 54 41 66 66 66 66 ad c2 1b 41 9a 99
#>  [7249] 99 e9 66 a1 54 41 1f 85 eb 51 0f c3 1b 41 ae 47 e1 5a 70 a1 54 41 33 33
#>  [7273] 33 33 17 c3 1b 41 a4 70 3d 3a 71 a1 54 41 cd cc cc cc 1b c3 1b 41 33 33
#>  [7297] 33 63 72 a1 54 41 5c 8f c2 f5 23 c3 1b 41 1f 85 eb 21 75 a1 54 41 9a 99
#>  [7321] 99 99 2b c3 1b 41 71 3d 0a 77 79 a1 54 41 1f 85 eb 51 2b c3 1b 41 9a 99
#>  [7345] 99 19 7a a1 54 41 71 3d 0a d7 29 c3 1b 41 f6 28 5c 7f 7a a1 54 41 ec 51
#>  [7369] b8 1e 13 c3 1b 41 29 5c 8f c2 7d a1 54 41 14 ae 47 e1 0d c3 1b 41 3d 0a
#>  [7393] d7 83 7e a1 54 41 52 b8 1e 85 06 c3 1b 41 a4 70 3d 2a 7f a1 54 41 3d 0a
#>  [7417] d7 a3 ee c2 1b 41 0a d7 a3 d0 80 a1 54 41 9a 99 99 99 da c2 1b 41 a4 70
#>  [7441] 3d ba 81 a1 54 41 d7 a3 70 3d 97 c2 1b 41 71 3d 0a 57 84 a1 54 41 00 00
#>  [7465] 00 00 8a c2 1b 41 8f c2 f5 e8 84 a1 54 41 c3 f5 28 5c 31 c2 1b 41 14 ae
#>  [7489] 47 c1 89 a1 54 41 d7 a3 70 3d 28 c2 1b 41 f6 28 5c 5f 8a a1 54 41 66 66
#>  [7513] 66 66 24 c2 1b 41 14 ae 47 d1 8a a1 54 41 c3 f5 28 5c f1 c1 1b 41 71 3d
#>  [7537] 0a 57 93 a1 54 41 1f 85 eb 51 ac c1 1b 41 0a d7 a3 b0 9a a1 54 41 b8 1e
#>  [7561] 85 eb a3 c1 1b 41 7b 14 ae 67 9b a1 54 41 7b 14 ae 47 9b c1 1b 41 5c 8f
#>  [7585] c2 e5 9b a1 54 41 3d 0a d7 a3 94 c1 1b 41 e1 7a 14 1e 9c a1 54 41 e1 7a
#>  [7609] 14 ae 28 c1 1b 41 8f c2 f5 a8 9d a1 54 41 71 3d 0a d7 23 c1 1b 41 e1 7a
#>  [7633] 14 de 9d a1 54 41 5c 8f c2 f5 1c c1 1b 41 7b 14 ae 57 9e a1 54 41 e1 7a
#>  [7657] 14 ae 18 c1 1b 41 1f 85 eb d1 9e a1 54 41 66 66 66 66 0a c1 1b 41 5c 8f
#>  [7681] c2 d5 a0 a1 54 41 00 00 00 00 01 c1 1b 41 14 ae 47 b1 a1 a1 54 41 d7 a3
#>  [7705] 70 3d ff c0 1b 41 66 66 66 f6 a1 a1 54 41 0a d7 a3 70 e9 c0 1b 41 48 e1
#>  [7729] 7a 84 a7 a1 54 41 66 66 66 66 e6 c0 1b 41 29 5c 8f 12 a8 a1 54 41 33 33
#>  [7753] 33 33 da c0 1b 41 f6 28 5c 2f a9 a1 54 41 14 ae 47 e1 b5 c0 1b 41 b8 1e
#>  [7777] 85 4b ad a1 54 41 e1 7a 14 ae 99 c0 1b 41 b8 1e 85 1b b0 a1 54 41 5c 8f
#>  [7801] c2 f5 6b c0 1b 41 f6 28 5c 2f b4 a1 54 41 14 ae 47 e1 65 c0 1b 41 52 b8
#>  [7825] 1e a5 b4 a1 54 41 a4 70 3d 0a 31 c0 1b 41 9a 99 99 a9 b7 a1 54 41 8f c2
#>  [7849] f5 28 2a c0 1b 41 ae 47 e1 fa b7 a1 54 41 14 ae 47 e1 d9 bf 1b 41 29 5c
#>  [7873] 8f e2 ba a1 54 41 33 33 33 33 73 bf 1b 41 ae 47 e1 fa be a1 54 41 d7 a3
#>  [7897] 70 3d 64 bf 1b 41 d7 a3 70 7d bf a1 54 41 71 3d 0a d7 3b bf 1b 41 9a 99
#>  [7921] 99 99 c0 a1 54 41 48 e1 7a 14 2d bf 1b 41 71 3d 0a e7 c0 a1 54 41 5c 8f
#>  [7945] c2 f5 06 bf 1b 41 d7 a3 70 6d c1 a1 54 41 5c 8f c2 f5 e3 be 1b 41 b8 1e
#>  [7969] 85 bb c1 a1 54 41 33 33 33 33 3d be 1b 41 8f c2 f5 28 c3 a1 54 41 66 66
#>  [7993] 66 66 2c be 1b 41 9a 99 99 59 c3 a1 54 41 a4 70 3d 0a 16 be 1b 41 00 00
#>  [8017] 00 e0 c3 a1 54 41 e1 7a 14 ae 93 bd 1b 41 e1 7a 14 3e c7 a1 54 41 71 3d
#>  [8041] 0a d7 7e bd 1b 41 ae 47 e1 7a c7 a1 54 41 cd cc cc cc 10 bd 1b 41 f6 28
#>  [8065] 5c 7f c8 a1 54 41 29 5c 8f c2 fb bc 1b 41 7b 14 ae 77 c8 a1 54 41 e1 7a
#>  [8089] 14 ae bd bc 1b 41 a4 70 3d 2a c8 a1 54 41 66 66 66 66 ac bc 1b 41 a4 70
#>  [8113] 3d 0a c8 a1 54 41 e1 7a 14 ae 41 bc 1b 41 ae 47 e1 3a c6 a1 54 41 85 eb
#>  [8137] 51 b8 34 bc 1b 41 e1 7a 14 0e c6 a1 54 41 52 b8 1e 85 1a bc 1b 41 e1 7a
#>  [8161] 14 fe c5 a1 54 41 33 33 33 33 5c bb 1b 41 a4 70 3d da c5 a1 54 41 00 00
#>  [8185] 00 00 14 bb 1b 41 b8 1e 85 2b c5 a1 54 41 f6 28 5c 8f 07 bb 1b 41 ae 47
#>  [8209] e1 fa c4 a1 54 41 00 00 00 00 f6 ba 1b 41 52 b8 1e 75 c4 a1 54 41 a4 70
#>  [8233] 3d 0a e9 ba 1b 41 f6 28 5c ff c3 a1 54 41 ec 51 b8 1e df ba 1b 41 52 b8
#>  [8257] 1e 85 c3 a1 54 41 0a d7 a3 70 d3 ba 1b 41 a4 70 3d ba c2 a1 54 41 0a d7
#>  [8281] a3 70 c4 ba 1b 41 d7 a3 70 7d c1 a1 54 41 c3 f5 28 5c 87 ba 1b 41 29 5c
#>  [8305] 8f a2 bb a1 54 41 ae 47 e1 7a 7e ba 1b 41 7b 14 ae 07 bb a1 54 41 9a 99
#>  [8329] 99 99 76 ba 1b 41 f6 28 5c cf ba a1 54 41 a4 70 3d 0a cb b9 1b 41 e1 7a
#>  [8353] 14 6e b8 a1 54 41 9a 99 99 99 af b9 1b 41 8f c2 f5 18 b8 a1 54 41 3d 0a
#>  [8377] d7 a3 93 b9 1b 41 14 ae 47 21 b8 a1 54 41 9a 99 99 99 44 b9 1b 41 a4 70
#>  [8401] 3d 5a b8 a1 54 41 b8 1e 85 eb 37 b9 1b 41 ae 47 e1 7a b8 a1 54 41 3d 0a
#>  [8425] d7 a3 19 b9 1b 41 14 ae 47 11 b9 a1 54 41 a4 70 3d 0a 12 b9 1b 41 e1 7a
#>  [8449] 14 4e b9 a1 54 41 33 33 33 33 ee b8 1b 41 52 b8 1e 05 bb a1 54 41 a4 70
#>  [8473] 3d 0a e4 b8 1b 41 ae 47 e1 6a bb a1 54 41 8f c2 f5 28 cd b8 1b 41 cd cc
#>  [8497] cc ec bb a1 54 41 85 eb 51 b8 bf b8 1b 41 a4 70 3d 2a bc a1 54 41 48 e1
#>  [8521] 7a 14 10 b8 1b 41 29 5c 8f 22 bd a1 54 41 48 e1 7a 14 00 b8 1b 41 3d 0a
#>  [8545] d7 63 bd a1 54 41 ae 47 e1 7a 8e b7 1b 41 14 ae 47 c1 bf a1 54 41 9a 99
#>  [8569] 99 99 86 b7 1b 41 9a 99 99 d9 bf a1 54 41 14 ae 47 e1 3a b7 1b 41 e1 7a
#>  [8593] 14 de bf a1 54 41 ec 51 b8 1e 1c b7 1b 41 c3 f5 28 6c c0 a1 54 41 33 33
#>  [8617] 33 33 00 b7 1b 41 ec 51 b8 0e c3 a1 54 41 e1 7a 14 ae ff b6 1b 41 00 00
#>  [8641] 00 50 c3 a1 54 41 00 00 00 00 05 b7 1b 41 66 66 66 e6 c4 a1 54 41 d7 a3
#>  [8665] 70 3d 04 b7 1b 41 b8 1e 85 3b c5 a1 54 41 5c 8f c2 f5 01 b7 1b 41 48 e1
#>  [8689] 7a 74 c5 a1 54 41 5c 8f c2 f5 e2 b6 1b 41 00 00 00 30 c6 a1 54 41 1f 85
#>  [8713] eb 51 d8 b6 1b 41 85 eb 51 58 c6 a1 54 41 48 e1 7a 14 ac b6 1b 41 1f 85
#>  [8737] eb a1 c6 a1 54 41 0a d7 a3 70 fa b5 1b 41 a4 70 3d 9a c7 a1 54 41 b8 1e
#>  [8761] 85 eb d9 b5 1b 41 c3 f5 28 fc c7 a1 54 41 71 3d 0a d7 69 b5 1b 41 66 66
#>  [8785] 66 76 c9 a1 54 41 66 66 66 66 50 b5 1b 41 c3 f5 28 dc c9 a1 54 41 85 eb
#>  [8809] 51 b8 aa b4 1b 41 1f 85 eb 41 cd a1 54 41 48 e1 7a 14 82 b4 1b 41 85 eb
#>  [8833] 51 c8 cd a1 54 41 33 33 33 33 0e b4 1b 41 5c 8f c2 25 cf a1 54 41 85 eb
#>  [8857] 51 b8 00 b4 1b 41 f6 28 5c 5f cf a1 54 41 8f c2 f5 28 fc b3 1b 41 7b 14
#>  [8881] ae 87 cf a1 54 41 b8 1e 85 eb ec b3 1b 41 f6 28 5c 6f d0 a1 54 41 a4 70
#>  [8905] 3d 0a e6 b3 1b 41 1f 85 eb 11 d1 a1 54 41 cd cc cc cc d5 b3 1b 41 5c 8f
#>  [8929] c2 05 d3 a1 54 41 29 5c 8f c2 d3 b3 1b 41 f6 28 5c 6f d3 a1 54 41 85 eb
#>  [8953] 51 b8 d1 b3 1b 41 00 00 00 90 d4 a1 54 41 85 eb 51 b8 d1 b3 1b 41 f6 28
#>  [8977] 5c 3f d6 a1 54 41 29 5c 8f c2 d3 b3 1b 41 a4 70 3d 0a d7 a1 54 41 0a d7
#>  [9001] a3 70 db b3 1b 41 d7 a3 70 ed d8 a1 54 41 52 b8 1e 85 e0 b3 1b 41 48 e1
#>  [9025] 7a d4 d9 a1 54 41 0a d7 a3 70 60 b4 1b 41 d7 a3 70 1d ea a1 54 41 a4 70
#>  [9049] 3d 0a 73 b4 1b 41 a4 70 3d 5a ec a1 54 41 ae 47 e1 7a e9 b4 1b 41 00 00
#>  [9073] 00 00 f5 a1 54 41 85 eb 51 b8 7c b5 1b 41 29 5c 8f c2 fc a1 54 41 29 5c
#>  [9097] 8f c2 cb b5 1b 41 1f 85 eb c1 ff a1 54 41 85 eb 51 b8 f6 b5 1b 41 ae 47
#>  [9121] e1 fa 00 a2 54 41 66 66 66 66 02 b6 1b 41 b8 1e 85 1b 01 a2 54 41 e1 7a
#>  [9145] 14 ae 10 b6 1b 41 33 33 33 23 01 a2 54 41 5c 8f c2 f5 15 b6 1b 41 ae 47
#>  [9169] e1 fa 00 a2 54 41 52 b8 1e 85 17 b6 1b 41 d7 a3 70 ad 00 a2 54 41 52 b8
#>  [9193] 1e 85 17 b6 1b 41 48 e1 7a 54 00 a2 54 41 5c 8f c2 f5 13 b6 1b 41 cd cc
#>  [9217] cc 8c ff a1 54 41 5c 8f c2 f5 14 b6 1b 41 0a d7 a3 60 ff a1 54 41 d7 a3
#>  [9241] 70 3d 17 b6 1b 41 00 00 00 40 ff a1 54 41 9a 99 99 99 1b b6 1b 41 b8 1e
#>  [9265] 85 2b ff a1 54 41 9a 99 99 99 2c b6 1b 41 71 3d 0a 17 ff a1 54 41 d7 a3
#>  [9289] 70 3d 45 b6 1b 41 ec 51 b8 0e ff a1 54 41 e1 7a 14 ae 4f b6 1b 41 b8 1e
#>  [9313] 85 3b ff a1 54 41 b8 1e 85 eb c8 b6 1b 41 0a d7 a3 90 02 a2 54 41 33 33
#>  [9337] 33 33 d9 b6 1b 41 9a 99 99 e9 02 a2 54 41 ec 51 b8 1e 41 b7 1b 41 52 b8
#>  [9361] 1e e5 04 a2 54 41 48 e1 7a 14 4c b7 1b 41 66 66 66 26 05 a2 54 41 5c 8f
#>  [9385] c2 f5 93 ba 1b 41 a4 70 3d fa 1d a2 54 41 48 e1 7a 14 bb ba 1b 41 f6 28
#>  [9409] 5c 2f 1f a2 54 41 5c 8f c2 f5 1d bb 1b 41 9a 99 99 19 23 a2 54 41 3d 0a
#>  [9433] d7 a3 36 bb 1b 41 48 e1 7a d4 23 a2 54 41 71 3d 0a d7 42 bb 1b 41 29 5c
#>  [9457] 8f 42 24 a2 54 41 a4 70 3d 0a c7 bb 1b 41 d7 a3 70 1d 2b a2 54 41 d7 a3
#>  [9481] 70 3d e1 bb 1b 41 85 eb 51 d8 2c a2 54 41 f6 28 5c 8f 03 bc 1b 41 5c 8f
#>  [9505] c2 45 2f a2 54 41 3d 0a d7 a3 09 bc 1b 41 3d 0a d7 c3 2f a2 54 41 48 e1
#>  [9529] 7a 14 40 bc 1b 41 29 5c 8f 92 35 a2 54 41 ec 51 b8 1e 32 bc 1b 41 14 ae
#>  [9553] 47 21 37 a2 54 41 00 00 00 00 2b bc 1b 41 d7 a3 70 1d 38 a2 54 41 5c 8f
#>  [9577] c2 f5 1d bc 1b 41 14 ae 47 81 3b a2 54 41 ae 47 e1 7a 17 bc 1b 41 14 ae
#>  [9601] 47 31 3d a2 54 41 33 33 33 33 06 bc 1b 41 cd cc cc 0c 40 a2 54 41 ec 51
#>  [9625] b8 1e 01 bc 1b 41 0a d7 a3 10 41 a2 54 41 48 e1 7a 14 ff bb 1b 41 cd cc
#>  [9649] cc fc 41 a2 54 41 1f 85 eb 51 fe bb 1b 41 b8 1e 85 bb 43 a2 54 41 ec 51
#>  [9673] b8 1e 00 bc 1b 41 66 66 66 66 44 a2 54 41 d7 a3 70 3d 08 bc 1b 41 52 b8
#>  [9697] 1e 35 46 a2 54 41 1f 85 eb 51 0b bc 1b 41 ae 47 e1 ba 46 a2 54 41 33 33
#>  [9721] 33 33 12 bc 1b 41 66 66 66 86 47 a2 54 41 0a d7 a3 70 3e bc 1b 41 85 eb
#>  [9745] 51 08 4c a2 54 41 8f c2 f5 28 3c bc 1b 41 ec 51 b8 ae 4c a2 54 41 9a 99
#>  [9769] 99 99 38 bc 1b 41 1f 85 eb 61 4e a2 54 41 d7 a3 70 3d 32 bc 1b 41 f6 28
#>  [9793] 5c cf 4f a2 54 41 71 3d 0a d7 30 bc 1b 41 b8 1e 85 0b 50 a2 54 41 b8 1e
#>  [9817] 85 eb 19 bc 1b 41 33 33 33 d3 53 a2 54 41 71 3d 0a d7 15 bc 1b 41 14 ae
#>  [9841] 47 61 54 a2 54 41 52 b8 1e 85 00 bc 1b 41 f6 28 5c bf 56 a2 54 41 14 ae
#>  [9865] 47 e1 f8 bb 1b 41 a4 70 3d 7a 57 a2 54 41 71 3d 0a d7 e8 bb 1b 41 33 33
#>  [9889] 33 b3 58 a2 54 41 0a d7 a3 70 dd bb 1b 41 5c 8f c2 65 59 a2 54 41 ec 51
#>  [9913] b8 1e d8 bb 1b 41 c3 f5 28 fc 59 a2 54 41 29 5c 8f c2 d0 bb 1b 41 1f 85
#>  [9937] eb 71 5a a2 54 41 52 b8 1e 85 92 bb 1b 41 85 eb 51 e8 5d a2 54 41 29 5c
#>  [9961] 8f c2 83 bb 1b 41 ae 47 e1 aa 5e a2 54 41 1f 85 eb 51 78 bb 1b 41 cd cc
#>  [9985] cc 1c 5f a2 54 41 e1 7a 14 ae 12 bb 1b 41 b8 1e 85 ab 62 a2 54 41 cd cc
#> [10009] cc cc 09 bb 1b 41 52 b8 1e 15 63 a2 54 41 9a 99 99 99 ed ba 1b 41 b8 1e
#> [10033] 85 ab 64 a2 54 41 0a d7 a3 70 e4 ba 1b 41 8f c2 f5 08 65 a2 54 41 85 eb
#> [10057] 51 b8 d5 ba 1b 41 e1 7a 14 2e 66 a2 54 41 3d 0a d7 a3 c1 ba 1b 41 9a 99
#> [10081] 99 29 67 a2 54 41 71 3d 0a d7 be ba 1b 41 3d 0a d7 93 67 a2 54 41 d7 a3
#> [10105] 70 3d b7 ba 1b 41 5c 8f c2 25 68 a2 54 41 85 eb 51 b8 77 ba 1b 41 9a 99
#> [10129] 99 29 6c a2 54 41 9a 99 99 99 71 ba 1b 41 00 00 00 d0 6c a2 54 41 00 00
#> [10153] 00 00 5b ba 1b 41 66 66 66 46 6e a2 54 41 b8 1e 85 eb 55 ba 1b 41 cd cc
#> [10177] cc dc 6e a2 54 41 14 ae 47 e1 43 ba 1b 41 9a 99 99 09 70 a2 54 41 cd cc
#> [10201] cc cc 3e ba 1b 41 3d 0a d7 a3 70 a2 54 41 85 eb 51 b8 38 ba 1b 41 5c 8f
#> [10225] c2 15 71 a2 54 41 9a 99 99 99 31 ba 1b 41 48 e1 7a c4 71 a2 54 41 b8 1e
#> [10249] 85 eb 24 ba 1b 41 3d 0a d7 a3 72 a2 54 41 c3 f5 28 5c 21 ba 1b 41 d7 a3
#> [10273] 70 0d 73 a2 54 41 7b 14 ae 47 1c ba 1b 41 33 33 33 73 73 a2 54 41 66 66
#> [10297] 66 66 15 ba 1b 41 a4 70 3d 2a 74 a2 54 41 48 e1 7a 14 d1 b9 1b 41 29 5c
#> [10321] 8f 42 78 a2 54 41 d7 a3 70 3d 3f b9 1b 41 cd cc cc 9c 7e a2 54 41 ae 47
#> [10345] e1 7a 2f b9 1b 41 3d 0a d7 63 7f a2 54 41 0a d7 a3 70 1c b9 1b 41 00 00
#> [10369] 00 70 80 a2 54 41 ec 51 b8 1e 18 b9 1b 41 8f c2 f5 98 80 a2 54 41 5c 8f
#> [10393] c2 f5 0f b9 1b 41 14 ae 47 c1 80 a2 54 41 66 66 66 66 0b b9 1b 41 a4 70
#> [10417] 3d ea 80 a2 54 41 c3 f5 28 5c ea b8 1b 41 ae 47 e1 0a 83 a2 54 41 d7 a3
#> [10441] 70 3d a3 b8 1b 41 d7 a3 70 cd 87 a2 54 41 48 e1 7a 14 8a b8 1b 41 33 33
#> [10465] 33 33 89 a2 54 41 85 eb 51 b8 53 b8 1b 41 8f c2 f5 98 8b a2 54 41 3d 0a
#> [10489] d7 a3 d3 b7 1b 41 7b 14 ae 47 90 a2 54 41 00 00 00 00 ac b7 1b 41 ae 47
#> [10513] e1 ea 91 a2 54 41 14 ae 47 e1 a3 b7 1b 41 48 e1 7a 54 92 a2 54 41 8f c2
#> [10537] f5 28 95 b7 1b 41 48 e1 7a 54 93 a2 54 41 9a 99 99 99 90 b7 1b 41 ae 47
#> [10561] e1 da 93 a2 54 41 ae 47 e1 7a 89 b7 1b 41 7b 14 ae 07 95 a2 54 41 33 33
#> [10585] 33 33 87 b7 1b 41 66 66 66 b6 95 a2 54 41 5c 8f c2 f5 86 b7 1b 41 d7 a3
#> [10609] 70 8d 9b a2 54 41 7b 14 ae 47 8a b7 1b 41 7b 14 ae 27 9c a2 54 41 d7 a3
#> [10633] 70 3d 97 b7 1b 41 a4 70 3d ea 9c a2 54 41 00 00 00 00 c4 b7 1b 41 ec 51
#> [10657] b8 ee 9f a2 54 41 0a d7 a3 70 fb b7 1b 41 a4 70 3d ea a2 a2 54 41 8f c2
#> [10681] f5 28 09 b8 1b 41 52 b8 1e 95 a3 a2 54 41 0a d7 a3 70 29 b8 1b 41 14 ae
#> [10705] 47 b1 a4 a2 54 41 f6 28 5c 8f 23 b9 1b 41 52 b8 1e e5 ac a2 54 41 a4 70
#> [10729] 3d 0a ca b9 1b 41 ae 47 e1 6a b1 a2 54 41 ae 47 e1 7a 2e ba 1b 41 cd cc
#> [10753] cc 1c b5 a2 54 41 52 b8 1e 85 4f ba 1b 41 a4 70 3d 8a b6 a2 54 41 85 eb
#> [10777] 51 b8 97 ba 1b 41 8f c2 f5 38 b9 a2 54 41 ae 47 e1 7a b7 ba 1b 41 0a d7
#> [10801] a3 30 ba a2 54 41 66 66 66 66 75 bb 1b 41 d7 a3 70 ad c2 a2 54 41 ae 47
#> [10825] e1 7a 10 bc 1b 41 f6 28 5c 2f c8 a2 54 41 b8 1e 85 eb 1c bc 1b 41 14 ae
#> [10849] 47 c1 c8 a2 54 41 71 3d 0a d7 71 bc 1b 41 1f 85 eb 21 cd a2 54 41 ec 51
#> [10873] b8 1e a1 bc 1b 41 8f c2 f5 d8 cf a2 54 41 85 eb 51 b8 b7 bc 1b 41 ae 47
#> [10897] e1 5a d1 a2 54 41 00 00 00 00 d6 bc 1b 41 1f 85 eb 31 d3 a2 54 41 85 eb
#> [10921] 51 b8 e2 bc 1b 41 5c 8f c2 15 d4 a2 54 41 e1 7a 14 ae ef bc 1b 41 d7 a3
#> [10945] 70 0d d5 a2 54 41 f6 28 5c 8f 14 bd 1b 41 a4 70 3d 2a d8 a2 54 41 f6 28
#> [10969] 5c 8f 32 bd 1b 41 71 3d 0a 97 db a2 54 41 c3 f5 28 5c 38 bd 1b 41 d7 a3
#> [10993] 70 ad dc a2 54 41 00 00 00 00 3d bd 1b 41 b8 1e 85 bb de a2 54 41 cd cc
#> [11017] cc cc 3e bd 1b 41 d7 a3 70 1d e3 a2 54 41 1f 85 eb 51 3e bd 1b 41 ae 47
#> [11041] e1 8a e3 a2 54 41 d7 a3 70 3d 3c bd 1b 41 9a 99 99 29 e4 a2 54 41 d7 a3
#> [11065] 70 3d 2a bd 1b 41 3d 0a d7 d3 e7 a2 54 41 b8 1e 85 eb 25 bd 1b 41 a4 70
#> [11089] 3d 6a e8 a2 54 41 7b 14 ae 47 0c bd 1b 41 e1 7a 14 5e eb a2 54 41 8f c2
#> [11113] f5 28 05 bd 1b 41 cd cc cc 0c ec a2 54 41 7b 14 ae 47 ed bc 1b 41 b8 1e
#> [11137] 85 9b ed a2 54 41 33 33 33 33 e8 bc 1b 41 5c 8f c2 25 ee a2 54 41 00 00
#> [11161] 00 00 6e bc 1b 41 ae 47 e1 6a f5 a2 54 41 52 b8 1e 85 5f bc 1b 41 9a 99
#> [11185] 99 29 f6 a2 54 41 ec 51 b8 1e 37 bc 1b 41 0a d7 a3 d0 f7 a2 54 41 c3 f5
#> [11209] 28 5c 27 bc 1b 41 ec 51 b8 4e f8 a2 54 41 9a 99 99 99 e6 bb 1b 41 d7 a3
#> [11233] 70 fd f9 a2 54 41 b8 1e 85 eb bb bb 1b 41 ec 51 b8 2e fb a2 54 41 00 00
#> [11257] 00 00 eb ba 1b 41 9a 99 99 e9 01 a3 54 41 29 5c 8f c2 da ba 1b 41 00 00
#> [11281] 00 80 02 a3 54 41 7b 14 ae 47 22 ba 1b 41 f6 28 5c 0f 0a a3 54 41 e1 7a
#> [11305] 14 ae 1a ba 1b 41 3d 0a d7 23 0a a3 54 41 cd cc cc cc f5 b9 1b 41 c3 f5
#> [11329] 28 2c 0a a3 54 41 b8 1e 85 eb cd b9 1b 41 14 ae 47 71 0a a3 54 41 ae 47
#> [11353] e1 7a c3 b9 1b 41 5c 8f c2 b5 0a a3 54 41 f6 28 5c 8f a7 b9 1b 41 e1 7a
#> [11377] 14 be 0b a3 54 41 b8 1e 85 eb 9d b9 1b 41 3d 0a d7 33 0c a3 54 41 b8 1e
#> [11401] 85 eb 7f b9 1b 41 cd cc cc 6c 0e a3 54 41 1f 85 eb 51 79 b9 1b 41 7b 14
#> [11425] ae 07 0f a3 54 41 33 33 33 33 72 b9 1b 41 0a d7 a3 50 10 a3 54 41 8f c2
#> [11449] f5 28 70 b9 1b 41 7b 14 ae 17 11 a3 54 41 0a d7 a3 70 71 b9 1b 41 ec 51
#> [11473] b8 be 11 a3 54 41 66 66 66 66 7b b9 1b 41 9a 99 99 79 13 a3 54 41 33 33
#> [11497] 33 33 80 b9 1b 41 ae 47 e1 ea 13 a3 54 41 66 66 66 66 8b b9 1b 41 1f 85
#> [11521] eb a1 14 a3 54 41 5c 8f c2 f5 6e b9 1b 41 b8 1e 85 2b 17 a3 54 41 f6 28
#> [11545] 5c 8f 46 b9 1b 41 29 5c 8f e2 19 a3 54 41 d7 a3 70 3d 41 b9 1b 41 0a d7
#> [11569] a3 50 1a a3 54 41 e1 7a 14 ae 3d b9 1b 41 29 5c 8f c2 1a a3 54 41 9a 99
#> [11593] 99 99 fa b8 1b 41 3d 0a d7 03 20 a3 54 41 52 b8 1e 85 c6 b8 1b 41 52 b8
#> [11617] 1e 75 23 a3 54 41 d7 a3 70 3d c4 b8 1b 41 29 5c 8f d2 23 a3 54 41 e1 7a
#> [11641] 14 ae bf b8 1b 41 c3 f5 28 2c 24 a3 54 41 d7 a3 70 3d 77 b8 1b 41 d7 a3
#> [11665] 70 5d 28 a3 54 41 7b 14 ae 47 39 b8 1b 41 e1 7a 14 6e 2b a3 54 41 7b 14
#> [11689] ae 47 8d b7 1b 41 0a d7 a3 10 35 a3 54 41 ae 47 e1 7a 89 b7 1b 41 a4 70
#> [11713] 3d 5a 35 a3 54 41 b8 1e 85 eb 85 b7 1b 41 7b 14 ae c7 35 a3 54 41 e1 7a
#> [11737] 14 ae 47 b7 1b 41 1f 85 eb 31 3a a3 54 41 48 e1 7a 14 42 b7 1b 41 0a d7
#> [11761] a3 e0 3a a3 54 41 ae 47 e1 7a 1c b7 1b 41 00 00 00 c0 3d a3 54 41 ae 47
#> [11785] e1 7a 0c b7 1b 41 66 66 66 76 3f a3 54 41 cd cc cc cc 00 b7 1b 41 48 e1
#> [11809] 7a d4 40 a3 54 41 85 eb 51 b8 fb b6 1b 41 b8 1e 85 9b 41 a3 54 41 33 33
#> [11833] 33 33 e9 b6 1b 41 d7 a3 70 1d 47 a3 54 41 66 66 66 66 e5 b6 1b 41 0a d7
#> [11857] a3 f0 47 a3 54 41 66 66 66 66 d4 b6 1b 41 7b 14 ae c7 4a a3 54 41 c3 f5
#> [11881] 28 5c d1 b6 1b 41 33 33 33 93 4b a3 54 41 c3 f5 28 5c d1 b6 1b 41 cd cc
#> [11905] cc ec 4b a3 54 41 c3 f5 28 5c d2 b6 1b 41 e1 7a 14 3e 4c a3 54 41 3d 0a
#> [11929] d7 a3 d4 b6 1b 41 ec 51 b8 7e 4c a3 54 41 48 e1 7a 14 e0 b6 1b 41 9a 99
#> [11953] 99 29 4d a3 54 41 b8 1e 85 eb e3 b6 1b 41 ae 47 e1 8a 4d a3 54 41 8f c2
#> [11977] f5 28 e5 b6 1b 41 8f c2 f5 18 4e a3 54 41 3d 0a d7 a3 e3 b6 1b 41 cd cc
#> [12001] cc ec 4e a3 54 41 52 b8 1e 85 d8 b6 1b 41 c3 f5 28 dc 51 a3 54 41 d7 a3
#> [12025] 70 3d d8 b6 1b 41 9a 99 99 29 52 a3 54 41 a4 70 3d 0a d9 b6 1b 41 ec 51
#> [12049] b8 5e 52 a3 54 41 14 ae 47 e1 de b6 1b 41 85 eb 51 d8 52 a3 54 41 29 5c
#> [12073] 8f c2 e6 b6 1b 41 5c 8f c2 25 53 a3 54 41 b8 1e 85 eb 0f b7 1b 41 cd cc
#> [12097] cc fc 53 a3 54 41 9a 99 99 99 19 b7 1b 41 52 b8 1e 45 54 a3 54 41 66 66
#> [12121] 66 66 1d b7 1b 41 ec 51 b8 8e 54 a3 54 41 5c 8f c2 f5 1f b7 1b 41 7b 14
#> [12145] ae e7 54 a3 54 41 33 33 33 33 21 b7 1b 41 14 ae 47 41 55 a3 54 41 5c 8f
#> [12169] c2 f5 20 b7 1b 41 ec 51 b8 9e 55 a3 54 41 d7 a3 70 3d 12 b7 1b 41 14 ae
#> [12193] 47 41 58 a3 54 41 0a d7 a3 70 0f b7 1b 41 c3 f5 28 fc 58 a3 54 41 5c 8f
#> [12217] c2 f5 0f b7 1b 41 a4 70 3d 6a 59 a3 54 41 71 3d 0a d7 15 b7 1b 41 3d 0a
#> [12241] d7 c3 5a a3 54 41 48 e1 7a 14 17 b7 1b 41 c3 f5 28 ec 5b a3 54 41 ec 51
#> [12265] b8 1e 19 b7 1b 41 85 eb 51 f8 5d a3 54 41 3d 0a d7 a3 18 b7 1b 41 52 b8
#> [12289] 1e 25 60 a3 54 41 8f c2 f5 28 1b b7 1b 41 f6 28 5c cf 60 a3 54 41 b8 1e
#> [12313] 85 eb 21 b7 1b 41 00 00 00 e0 61 a3 54 41 0a d7 a3 70 92 b7 1b 41 e1 7a
#> [12337] 14 6e 67 a3 54 41 8f c2 f5 28 9f b7 1b 41 d7 a3 70 5d 68 a3 54 41 33 33
#> [12361] 33 33 bd b7 1b 41 71 3d 0a c7 69 a3 54 41 33 33 33 33 e9 b7 1b 41 cd cc
#> [12385] cc 4c 6c a3 54 41 8f c2 f5 28 f7 b7 1b 41 f6 28 5c ff 6c a3 54 41 f6 28
#> [12409] 5c 8f 47 b8 1b 41 85 eb 51 88 6f a3 54 41 85 eb 51 b8 8d b8 1b 41 f6 28
#> [12433] 5c 7f 71 a3 54 41 66 66 66 66 96 b8 1b 41 0a d7 a3 d0 71 a3 54 41 29 5c
#> [12457] 8f c2 9e b8 1b 41 29 5c 8f 52 72 a3 54 41 1f 85 eb 51 e9 b8 1b 41 9a 99
#> [12481] 99 f9 77 a3 54 41 cd cc cc cc f5 b8 1b 41 14 ae 47 d1 78 a3 54 41 33 33
#> [12505] 33 33 ff b8 1b 41 5c 8f c2 25 79 a3 54 41 8f c2 f5 28 0a b9 1b 41 29 5c
#> [12529] 8f 72 79 a3 54 41 cd cc cc cc 50 b9 1b 41 ec 51 b8 be 7a a3 54 41 33 33
#> [12553] 33 33 59 b9 1b 41 71 3d 0a d7 7a a3 54 41 66 66 66 66 65 b9 1b 41 ae 47
#> [12577] e1 da 7a a3 54 41 48 e1 7a 14 9d b9 1b 41 9a 99 99 b9 7a a3 54 41 14 ae
#> [12601] 47 e1 b0 b9 1b 41 48 e1 7a 94 7a a3 54 41 c3 f5 28 5c cf b9 1b 41 66 66
#> [12625] 66 36 7a a3 54 41 9a 99 99 99 dc b9 1b 41 71 3d 0a 77 7a a3 54 41 71 3d
#> [12649] 0a d7 ea b9 1b 41 85 eb 51 d8 7a a3 54 41 52 b8 1e 85 3e ba 1b 41 3d 0a
#> [12673] d7 e3 7d a3 54 41 e1 7a 14 ae 64 ba 1b 41 3d 0a d7 c3 80 a3 54 41 0a d7
#> [12697] a3 70 82 ba 1b 41 52 b8 1e 45 82 a3 54 41 48 e1 7a 14 8a ba 1b 41 66 66
#> [12721] 66 96 82 a3 54 41 5c 8f c2 f5 f9 ba 1b 41 8f c2 f5 98 86 a3 54 41 85 eb
#> [12745] 51 b8 08 bb 1b 41 ae 47 e1 0a 87 a3 54 41 f6 28 5c 8f 69 bb 1b 41 71 3d
#> [12769] 0a 67 89 a3 54 41 e1 7a 14 ae 8c bb 1b 41 5c 8f c2 35 8a a3 54 41 9a 99
#> [12793] 99 99 98 bb 1b 41 29 5c 8f 62 8a a3 54 41 ae 47 e1 7a fa bb 1b 41 d7 a3
#> [12817] 70 7d 8b a3 54 41 00 00 00 00 65 bc 1b 41 52 b8 1e c5 8d a3 54 41 1f 85
#> [12841] eb 51 75 bc 1b 41 1f 85 eb 11 8e a3 54 41 00 00 00 00 f2 bc 1b 41 66 66
#> [12865] 66 a6 8f a3 54 41 7b 14 ae 47 1b bd 1b 41 3d 0a d7 33 90 a3 54 41 f6 28
#> [12889] 5c 8f 2d bd 1b 41 0a d7 a3 80 90 a3 54 41 14 ae 47 e1 97 bd 1b 41 7b 14
#> [12913] ae 97 93 a3 54 41 48 e1 7a 14 bf bd 1b 41 1f 85 eb 21 96 a3 54 41 d7 a3
#> [12937] 70 3d c9 bd 1b 41 b8 1e 85 ab 96 a3 54 41 a4 70 3d 0a f8 bd 1b 41 5c 8f
#> [12961] c2 55 98 a3 54 41 48 e1 7a 14 08 be 1b 41 7b 14 ae d7 98 a3 54 41 ae 47
#> [12985] e1 7a 12 be 1b 41 48 e1 7a 14 99 a3 54 41 85 eb 51 b8 7a be 1b 41 9a 99
#> [13009] 99 b9 9a a3 54 41 9a 99 99 99 93 be 1b 41 29 5c 8f 12 9b a3 54 41 d7 a3
#> [13033] 70 3d aa be 1b 41 71 3d 0a 47 9b a3 54 41 3d 0a d7 a3 f1 be 1b 41 b8 1e
#> [13057] 85 9b 9b a3 54 41 52 b8 1e 85 07 bf 1b 41 85 eb 51 e8 9b a3 54 41 66 66
#> [13081] 66 66 2c bf 1b 41 ec 51 b8 ae 9c a3 54 41 29 5c 8f c2 33 bf 1b 41 ec 51
#> [13105] b8 be 9c a3 54 41 66 66 66 66 4c bf 1b 41 ec 51 b8 be 9c a3 54 41 33 33
#> [13129] 33 33 50 bf 1b 41 e1 7a 14 ae 9c a3 54 41 85 eb 51 b8 51 bf 1b 41 d7 a3
#> [13153] 70 8d 9c a3 54 41 d7 a3 70 3d 52 bf 1b 41 00 00 00 30 9c a3 54 41 e1 7a
#> [13177] 14 ae 4e bf 1b 41 5c 8f c2 95 9b a3 54 41 7b 14 ae 47 37 bf 1b 41 5c 8f
#> [13201] c2 a5 99 a3 54 41 0a d7 a3 70 32 bf 1b 41 7b 14 ae 27 99 a3 54 41 1f 85
#> [13225] eb 51 2d bf 1b 41 33 33 33 23 98 a3 54 41 0a d7 a3 70 27 bf 1b 41 0a d7
#> [13249] a3 80 96 a3 54 41 52 b8 1e 85 3a bf 1b 41 a4 70 3d fa 96 a3 54 41 e1 7a
#> [13273] 14 ae ac bf 1b 41 cd cc cc 0c 99 a3 54 41 66 66 66 66 bb bf 1b 41 5c 8f
#> [13297] c2 55 99 a3 54 41 ec 51 b8 1e c6 bf 1b 41 0a d7 a3 f0 9a a3 54 41 d7 a3
#> [13321] 70 3d cc bf 1b 41 66 66 66 76 9b a3 54 41 85 eb 51 b8 d9 bf 1b 41 a4 70
#> [13345] 3d 6a 9c a3 54 41 48 e1 7a 14 e1 bf 1b 41 85 eb 51 d8 9c a3 54 41 29 5c
#> [13369] 8f c2 eb bf 1b 41 9a 99 99 39 9d a3 54 41 3d 0a d7 a3 1e c0 1b 41 1f 85
#> [13393] eb a1 9e a3 54 41 1f 85 eb 51 2a c0 1b 41 71 3d 0a e7 9e a3 54 41 29 5c
#> [13417] 8f c2 35 c0 1b 41 71 3d 0a f7 9e a3 54 41 00 00 00 00 46 c0 1b 41 66 66
#> [13441] 66 e6 9e a3 54 41 f6 28 5c 8f 49 c0 1b 41 5c 8f c2 c5 9e a3 54 41 66 66
#> [13465] 66 66 4d c0 1b 41 00 00 00 60 9e a3 54 41 f6 28 5c 8f 5a c0 1b 41 85 eb
#> [13489] 51 88 9c a3 54 41 cd cc cc cc 76 c0 1b 41 14 ae 47 d1 9c a3 54 41 a4 70
#> [13513] 3d 0a b2 c0 1b 41 66 66 66 46 9d a3 54 41 a4 70 3d 0a c5 c0 1b 41 29 5c
#> [13537] 8f 82 9d a3 54 41 b8 1e 85 eb c9 c0 1b 41 33 33 33 b3 9e a3 54 41 f6 28
#> [13561] 5c 8f d0 c0 1b 41 0a d7 a3 00 a0 a3 54 41 9a 99 99 99 d3 c0 1b 41 a4 70
#> [13585] 3d 5a a0 a3 54 41 1f 85 eb 51 ed c0 1b 41 29 5c 8f 72 a2 a3 54 41 3d 0a
#> [13609] d7 a3 f1 c0 1b 41 f6 28 5c af a2 a3 54 41 ae 47 e1 7a f6 c0 1b 41 c3 f5
#> [13633] 28 dc a2 a3 54 41 5c 8f c2 f5 6d c1 1b 41 8f c2 f5 68 a5 a3 54 41 d7 a3
#> [13657] 70 3d 7e c1 1b 41 e1 7a 14 ce a5 a3 54 41 66 66 66 66 87 c1 1b 41 b8 1e
#> [13681] 85 3b a6 a3 54 41 ae 47 e1 7a 9b c1 1b 41 85 eb 51 68 a7 a3 54 41 9a 99
#> [13705] 99 99 a0 c1 1b 41 66 66 66 d6 a7 a3 54 41 3d 0a d7 a3 b0 c1 1b 41 9a 99
#> [13729] 99 b9 a9 a3 54 41 ae 47 e1 7a b5 c1 1b 41 3d 0a d7 23 aa a3 54 41 48 e1
#> [13753] 7a 14 bd c1 1b 41 48 e1 7a 74 aa a3 54 41 ec 51 b8 1e cf c1 1b 41 9a 99
#> [13777] 99 e9 aa a3 54 41 85 eb 51 b8 d4 c1 1b 41 a4 70 3d fa aa a3 54 41 71 3d
#> [13801] 0a d7 36 c2 1b 41 7b 14 ae d7 aa a3 54 41 29 5c 8f c2 8e c2 1b 41 71 3d
#> [13825] 0a 27 ab a3 54 41 3d 0a d7 a3 95 c2 1b 41 b8 1e 85 3b ab a3 54 41 3d 0a
#> [13849] d7 a3 2b c3 1b 41 0a d7 a3 10 ae a3 54 41 a4 70 3d 0a 35 c3 1b 41 52 b8
#> [13873] 1e 25 ae a3 54 41 e1 7a 14 ae 4c c3 1b 41 85 eb 51 28 ae a3 54 41 66 66
#> [13897] 66 66 5a c3 1b 41 c3 f5 28 1c ae a3 54 41 b8 1e 85 eb 69 c3 1b 41 ec 51
#> [13921] b8 ce ad a3 54 41 00 00 00 00 80 c3 1b 41 3d 0a d7 43 ad a3 54 41 29 5c
#> [13945] 8f c2 90 c3 1b 41 a4 70 3d ea ac a3 54 41 a4 70 3d 0a a2 c3 1b 41 52 b8
#> [13969] 1e d5 ac a3 54 41 8f c2 f5 28 f4 c3 1b 41 71 3d 0a a7 ac a3 54 41 14 ae
#> [13993] 47 e1 31 c4 1b 41 b8 1e 85 2b ac a3 54 41 d7 a3 70 3d 65 c4 1b 41 00 00
#> [14017] 00 80 ac a3 54 41 7b 14 ae 47 b4 c4 1b 41 a4 70 3d 5a ad a3 54 41 48 e1
#> [14041] 7a 14 81 c7 1b 41 52 b8 1e f5 b2 a3 54 41 a4 70 3d 0a d9 c7 1b 41 9a 99
#> [14065] 99 79 b3 a3 54 41 33 33 33 33 4f c8 1b 41 5c 8f c2 05 b5 a3 54 41 33 33
#> [14089] 33 33 23 c9 1b 41 0a d7 a3 a0 b5 a3 54 41 ae 47 e1 7a 34 c9 1b 41 8f c2
#> [14113] f5 d8 b5 a3 54 41 cd cc cc cc fd c9 1b 41 9a 99 99 d9 b8 a3 54 41 7b 14
#> [14137] ae 47 0b ca 1b 41 e1 7a 14 fe b8 a3 54 41 d7 a3 70 3d 1d ca 1b 41 ec 51
#> [14161] b8 0e b9 a3 54 41 3d 0a d7 a3 33 ca 1b 41 33 33 33 23 b9 a3 54 41 48 e1
#> [14185] 7a 14 5e ca 1b 41 ae 47 e1 4a b9 a3 54 41 cd cc cc cc f5 cb 1b 41 48 e1
#> [14209] 7a c4 ba a3 54 41 00 00 00 00 a7 cc 1b 41 71 3d 0a 27 bc a3 54 41 14 ae
#> [14233] 47 e1 2f cd 1b 41 e1 7a 14 3e bd a3 54 41 ae 47 e1 7a 5f cd 1b 41 e1 7a
#> [14257] 14 9e bd a3 54 41 71 3d 0a d7 86 cd 1b 41 a4 70 3d ba bd a3 54 41 d7 a3
#> [14281] 70 3d 21 ce 1b 41 ae 47 e1 1a bf a3 54 41 33 33 33 33 64 ce 1b 41 e1 7a
#> [14305] 14 be bf a3 54 41 1f 85 eb 51 76 ce 1b 41 b8 1e 85 3b c0 a3 54 41 a4 70
#> [14329] 3d 0a 82 ce 1b 41 14 ae 47 a1 c0 a3 54 41 d7 a3 70 3d 8c ce 1b 41 7b 14
#> [14353] ae 37 c1 a3 54 41 33 33 33 33 a8 ce 1b 41 71 3d 0a 47 c3 a3 54 41 f6 28
#> [14377] 5c 8f ae ce 1b 41 e1 7a 14 ee c3 a3 54 41 85 eb 51 b8 b6 ce 1b 41 66 66
#> [14401] 66 16 c5 a3 54 41 a4 70 3d 0a ba ce 1b 41 d7 a3 70 bd c5 a3 54 41 c3 f5
#> [14425] 28 5c bd ce 1b 41 ae 47 e1 1a c7 a3 54 41 33 33 33 33 be ce 1b 41 5c 8f
#> [14449] c2 55 cc a3 54 41 5c 8f c2 f5 bc ce 1b 41 d7 a3 70 4d cd a3 54 41 5c 8f
#> [14473] c2 f5 94 ce 1b 41 8f c2 f5 58 d7 a3 54 41 a4 70 3d 0a 84 ce 1b 41 8f c2
#> [14497] f5 28 dd a3 54 41 33 33 33 33 3e ce 1b 41 85 eb 51 a8 ee a3 54 41 f6 28
#> [14521] 5c 8f 34 ce 1b 41 7b 14 ae 37 f0 a3 54 41 3d 0a d7 a3 02 ce 1b 41 ae 47
#> [14545] e1 7a f7 a3 54 41 7b 14 ae 47 da cd 1b 41 7b 14 ae 57 fb a3 54 41 8f c2
#> [14569] f5 28 44 cd 1b 41 3d 0a d7 a3 0d a4 54 41 33 33 33 33 38 cd 1b 41 c3 f5
#> [14593] 28 bc 0e a4 54 41 85 eb 51 b8 20 cd 1b 41 14 ae 47 e1 11 a4 54 41 48 e1
#> [14617] 7a 14 0a cd 1b 41 b8 1e 85 eb 13 a4 54 41 66 66 66 66 fe cc 1b 41 d7 a3
#> [14641] 70 3d 15 a4 54 41 71 3d 0a d7 d8 cc 1b 41 14 ae 47 f1 19 a4 54 41 d7 a3
#> [14665] 70 3d d2 cc 1b 41 9a 99 99 e9 1a a4 54 41 c3 f5 28 5c c6 cc 1b 41 ec 51
#> [14689] b8 fe 1c a4 54 41 52 b8 1e 85 af cc 1b 41 cd cc cc 1c 20 a4 54 41 85 eb
#> [14713] 51 b8 56 cc 1b 41 ec 51 b8 be 29 a4 54 41 ae 47 e1 7a 21 cc 1b 41 5c 8f
#> [14737] c2 e5 31 a4 54 41 d7 a3 70 3d fd cb 1b 41 33 33 33 b3 36 a4 54 41 48 e1
#> [14761] 7a 14 df cb 1b 41 1f 85 eb 11 3a a4 54 41 5c 8f c2 f5 c6 cb 1b 41 52 b8
#> [14785] 1e 85 3c a4 54 41 00 00 00 00 86 cb 1b 41 0a d7 a3 e0 41 a4 54 41 66 66
#> [14809] 66 66 60 cb 1b 41 8f c2 f5 a8 44 a4 54 41 1f 85 eb 51 59 cb 1b 41 0a d7
#> [14833] a3 70 45 a4 54 41 14 ae 47 e1 17 cb 1b 41 f6 28 5c 6f 4d a4 54 41 cd cc
#> [14857] cc cc 12 cb 1b 41 5c 8f c2 f5 4d a4 54 41 d7 a3 70 3d 0d cb 1b 41 33 33
#> [14881] 33 63 4e a4 54 41 9a 99 99 99 04 cb 1b 41 00 00 00 60 4f a4 54 41 48 e1
#> [14905] 7a 14 e1 ca 1b 41 c3 f5 28 0c 54 a4 54 41 00 00 00 00 da ca 1b 41 3d 0a
#> [14929] d7 e3 54 a4 54 41 1f 85 eb 51 44 ca 1b 41 33 33 33 53 64 a4 54 41 8f c2
#> [14953] f5 28 3a ca 1b 41 7b 14 ae 37 65 a4 54 41 ec 51 b8 1e 04 ca 1b 41 8f c2
#> [14977] f5 18 69 a4 54 41 66 66 66 66 f6 c9 1b 41 9a 99 99 29 6a a4 54 41 cd cc
#> [15001] cc cc bc c9 1b 41 00 00 00 20 70 a4 54 41 66 66 66 66 b2 c9 1b 41 f6 28
#> [15025] 5c df 70 a4 54 41 e1 7a 14 ae 71 c9 1b 41 1f 85 eb f1 74 a4 54 41 29 5c
#> [15049] 8f c2 66 c9 1b 41 a4 70 3d ea 75 a4 54 41 48 e1 7a 14 48 c9 1b 41 c3 f5
#> [15073] 28 0c 79 a4 54 41 0a d7 a3 70 40 c9 1b 41 ec 51 b8 9e 79 a4 54 41 9a 99
#> [15097] 99 99 2a c9 1b 41 85 eb 51 08 7b a4 54 41 e1 7a 14 ae 1e c9 1b 41 0a d7
#> [15121] a3 f0 7b a4 54 41 29 5c 8f c2 02 c9 1b 41 cd cc cc cc 7d a4 54 41 e1 7a
#> [15145] 14 ae fe c8 1b 41 ae 47 e1 2a 7e a4 54 41 b8 1e 85 eb fb c8 1b 41 cd cc
#> [15169] cc ac 7e a4 54 41 cd cc cc cc d2 c8 1b 41 8f c2 f5 58 82 a4 54 41 c3 f5
#> [15193] 28 5c c6 c8 1b 41 14 ae 47 41 83 a4 54 41 14 ae 47 e1 a4 c8 1b 41 cd cc
#> [15217] cc bc 85 a4 54 41 00 00 00 00 7c c8 1b 41 52 b8 1e a5 89 a4 54 41 cd cc
#> [15241] cc cc 6e c8 1b 41 9a 99 99 99 8a a4 54 41 d7 a3 70 3d 5e c8 1b 41 e1 7a
#> [15265] 14 8e 8b a4 54 41 f6 28 5c 8f 4f c8 1b 41 5c 8f c2 65 8c a4 54 41 00 00
#> [15289] 00 00 4c c8 1b 41 71 3d 0a a7 8c a4 54 41 5c 8f c2 f5 47 c8 1b 41 14 ae
#> [15313] 47 21 8d a4 54 41 00 00 00 00 38 c8 1b 41 8f c2 f5 d8 8f a4 54 41 00 00
#> [15337] 00 00 37 c8 1b 41 71 3d 0a 57 90 a4 54 41 7b 14 ae 47 37 c8 1b 41 33 33
#> [15361] 33 23 93 a4 54 41 ae 47 e1 7a 40 c8 1b 41 29 5c 8f f2 95 a4 54 41 cd cc
#> [15385] cc cc 4e c8 1b 41 0a d7 a3 50 99 a4 54 41 cd cc cc cc 4d c8 1b 41 c3 f5
#> [15409] 28 2c 9a a4 54 41 f6 28 5c 8f 4e c8 1b 41 33 33 33 d3 9a a4 54 41 29 5c
#> [15433] 8f c2 57 c8 1b 41 5c 8f c2 75 9d a4 54 41 3d 0a d7 a3 5d c8 1b 41 e1 7a
#> [15457] 14 7e 9e a4 54 41 7b 14 ae 47 73 c8 1b 41 33 33 33 c3 a1 a4 54 41 00 00
#> [15481] 00 00 81 c8 1b 41 29 5c 8f 92 a3 a4 54 41 52 b8 1e 85 fb c8 1b 41 85 eb
#> [15505] 51 a8 b2 a4 54 41 14 ae 47 e1 00 c9 1b 41 71 3d 0a 67 b3 a4 54 41 ec 51
#> [15529] b8 1e 1a c9 1b 41 ae 47 e1 2a b8 a4 54 41 9a 99 99 99 25 c9 1b 41 29 5c
#> [15553] 8f 22 bb a4 54 41 1f 85 eb 51 2e c9 1b 41 cd cc cc 6c be a4 54 41 ec 51
#> [15577] b8 1e 32 c9 1b 41 0a d7 a3 70 bf a4 54 41 66 66 66 66 41 c9 1b 41 14 ae
#> [15601] 47 81 c1 a4 54 41 7b 14 ae 47 47 c9 1b 41 3d 0a d7 23 c2 a4 54 41 e1 7a
#> [15625] 14 ae 5e c9 1b 41 71 3d 0a e7 c3 a4 54 41 8f c2 f5 28 6c c9 1b 41 66 66
#> [15649] 66 d6 c4 a4 54 41 9a 99 99 99 75 c9 1b 41 00 00 00 40 c5 a4 54 41 d7 a3
#> [15673] 70 3d 8e c9 1b 41 ec 51 b8 0e c6 a4 54 41 c3 f5 28 5c a4 c9 1b 41 52 b8
#> [15697] 1e a5 c6 a4 54 41 d7 a3 70 3d 7e ca 1b 41 9a 99 99 89 cb a4 54 41 14 ae
#> [15721] 47 e1 b4 ca 1b 41 7b 14 ae 37 cd a4 54 41 0a d7 a3 70 d5 ca 1b 41 85 eb
#> [15745] 51 78 ce a4 54 41 cd cc cc cc db ca 1b 41 a4 70 3d fa ce a4 54 41 d7 a3
#> [15769] 70 3d 3a cb 1b 41 14 ae 47 b1 d7 a4 54 41 3d 0a d7 a3 41 cb 1b 41 9a 99
#> [15793] 99 b9 d8 a4 54 41 d7 a3 70 3d 6f cb 1b 41 0a d7 a3 70 e0 a4 54 41 9a 99
#> [15817] 99 99 72 cb 1b 41 7b 14 ae 27 e1 a4 54 41 66 66 66 66 7f cb 1b 41 1f 85
#> [15841] eb a1 e5 a4 54 41 7b 14 ae 47 87 cb 1b 41 66 66 66 66 e8 a4 54 41 a4 70
#> [15865] 3d 0a 95 cb 1b 41 5c 8f c2 55 eb a4 54 41 33 33 33 33 9b cb 1b 41 ec 51
#> [15889] b8 5e ed a4 54 41 71 3d 0a d7 9f cb 1b 41 33 33 33 43 f1 a4 54 41 cd cc
#> [15913] cc cc 98 cb 1b 41 c3 f5 28 7c 04 a5 54 41 29 5c 8f c2 95 cb 1b 41 8f c2
#> [15937] f5 98 05 a5 54 41 0a d7 a3 70 77 cb 1b 41 33 33 33 a3 0e a5 54 41 3d 0a
#> [15961] d7 a3 75 cb 1b 41 00 00 00 d0 0f a5 54 41 d7 a3 70 3d 76 cb 1b 41 00 00
#> [15985] 00 90 23 a5 54 41 c3 f5 28 5c 79 cb 1b 41 8f c2 f5 88 25 a5 54 41 14 ae
#> [16009] 47 e1 83 cb 1b 41 85 eb 51 48 2c a5 54 41 b8 1e 85 eb 85 cb 1b 41 b8 1e
#> [16033] 85 1b 2d a5 54 41 d7 a3 70 3d 8a cb 1b 41 29 5c 8f c2 2d a5 54 41 d7 a3
#> [16057] 70 3d 97 cb 1b 41 c3 f5 28 2c 2f a5 54 41 14 ae 47 e1 a0 cb 1b 41 71 3d
#> [16081] 0a f7 2f a5 54 41 9a 99 99 99 bb cb 1b 41 33 33 33 f3 31 a5 54 41 52 b8
#> [16105] 1e 85 c4 cb 1b 41 14 ae 47 71 32 a5 54 41 52 b8 1e 85 d1 cb 1b 41 ec 51
#> [16129] b8 ee 32 a5 54 41 8f c2 f5 28 eb cb 1b 41 a4 70 3d 0a 34 a5 54 41 cd cc
#> [16153] cc cc 0f cc 1b 41 b8 1e 85 4b 36 a5 54 41 71 3d 0a d7 1f cc 1b 41 00 00
#> [16177] 00 60 37 a5 54 41 7b 14 ae 47 62 cc 1b 41 0a d7 a3 90 3d a5 54 41 66 66
#> [16201] 66 66 69 cc 1b 41 52 b8 1e 95 3e a5 54 41 a4 70 3d 0a c0 cc 1b 41 b8 1e
#> [16225] 85 0b 4c a5 54 41 d7 a3 70 3d 29 cd 1b 41 3d 0a d7 f3 59 a5 54 41 48 e1
#> [16249] 7a 14 8a cd 1b 41 c3 f5 28 8c 69 a5 54 41 d7 a3 70 3d a1 cd 1b 41 33 33
#> [16273] 33 33 6c a5 54 41 e1 7a 14 ae a2 ce 1b 41 1f 85 eb 91 88 a5 54 41 48 e1
#> [16297] 7a 14 b8 ce 1b 41 ec 51 b8 fe 8a a5 54 41 9a 99 99 99 fd ce 1b 41 0a d7
#> [16321] a3 20 95 a5 54 41 52 b8 1e 85 2f cf 1b 41 33 33 33 c3 9b a5 54 41 5c 8f
#> [16345] c2 f5 37 cf 1b 41 85 eb 51 f8 9c a5 54 41 00 00 00 00 3a cf 1b 41 ae 47
#> [16369] e1 7a 9d a5 54 41 ae 47 e1 7a 49 cf 1b 41 e1 7a 14 de a2 a5 54 41 d7 a3
#> [16393] 70 3d 53 cf 1b 41 3d 0a d7 43 a6 a5 54 41 cd cc cc cc 7a cf 1b 41 b8 1e
#> [16417] 85 cb a5 a5 54 41 d7 a3 70 3d af d9 1b 41 3d 0a d7 a3 86 a5 54 41 1f 85
#> [16441] eb 51 ba dc 1b 41 7b 14 ae e7 7c a5 54 41 85 eb 51 b8 09 dd 1b 41 ae 47
#> [16465] e1 ea 7b a5 54 41 52 b8 1e 85 4a dd 1b 41 b8 1e 85 1b 7b a5 54 41 0a d7
#> [16489] a3 70 99 dd 1b 41 ec 51 b8 1e 7a a5 54 41 a4 70 3d 0a 5e e3 1b 41 cd cc
#> [16513] cc ac 67 a5 54 41 ec 51 b8 1e d8 e2 1b 41 f6 28 5c 8f 33 a5 54 41 48 e1
#> [16537] 7a 14 5b e2 1b 41 48 e1 7a e4 02 a5 54 41 7b 14 ae 47 aa e2 1b 41 f6 28
#> [16561] 5c ff 01 a5 54 41 ec 51 b8 1e 66 e4 1b 41 3d 0a d7 d3 fc a4 54 41 14 ae
#> [16585] 47 e1 19 ea 1b 41 d7 a3 70 ad eb a4 54 41 52 b8 1e 85 bc ec 1b 41 14 ae
#> [16609] 47 c1 e3 a4 54 41 00 00 00 00 a7 f3 1b 41 66 66 66 06 d0 a4 54 41 7b 14
#> [16633] ae 47 47 f3 1b 41 85 eb 51 78 ab a4 54 41 e1 7a 14 ae c1 f2 1b 41 b8 1e
#> [16657] 85 6b 78 a4 54 41 ae 47 e1 7a 87 f2 1b 41 33 33 33 33 62 a4 54 41 8f c2
#> [16681] f5 28 07 f2 1b 41 66 66 66 56 31 a4 54 41 0a d7 a3 70 52 f1 1b 41 48 e1
#> [16705] 7a 24 ef a3 54 41 a4 70 3d 0a e2 f3 1b 41 7b 14 ae b7 e6 a3 54 41 66 66
#> [16729] 66 66 8c f8 1b 41 e1 7a 14 5e d7 a3 54 41 7b 14 ae 47 76 f8 1b 41 9a 99
#> [16753] 99 f9 cf a3 54 41 0a d7 a3 70 2d f8 1b 41 f6 28 5c 5f b9 a3 54 41 01 03
#> [16777] 00 00 00 01 00 00 00 05 00 00 00 9a 99 99 99 78 e2 1b 41 c3 f5 28 bc 53
#> [16801] 9d 54 41 7b 14 ae 47 f7 f0 1b 41 85 eb 51 18 51 9d 54 41 9a 99 99 99 78
#> [16825] e2 1b 41 5c 8f c2 b5 53 9d 54 41 f6 28 5c 8f 66 e2 1b 41 8f c2 f5 e8 54
#> [16849] 9d 54 41 9a 99 99 99 78 e2 1b 41 c3 f5 28 bc 53 9d 54 41 01 03 00 00 00
#> [16873] 01 00 00 00 05 00 00 00 cd cc cc cc 96 dd 1b 41 ec 51 b8 ee 59 9d 54 41
#> [16897] 5c 8f c2 f5 5e dd 1b 41 52 b8 1e 35 59 9d 54 41 3d 0a d7 a3 3d dd 1b 41
#> [16921] ec 51 b8 9e 59 9d 54 41 b8 1e 85 eb 5e dd 1b 41 c3 f5 28 3c 59 9d 54 41
#> [16945] cd cc cc cc 96 dd 1b 41 ec 51 b8 ee 59 9d 54 41 01 03 00 00 00 01 00 00
#> [16969] 00 09 00 00 00 85 eb 51 b8 7a de 1b 41 5c 8f c2 55 5e 9d 54 41 48 e1 7a
#> [16993] 14 f5 dd 1b 41 29 5c 8f 92 5b 9d 54 41 e1 7a 14 ae 7a de 1b 41 29 5c 8f
#> [17017] 62 5e 9d 54 41 f6 28 5c 8f 4d df 1b 41 c3 f5 28 3c 64 9d 54 41 b8 1e 85
#> [17041] eb ac df 1b 41 ae 47 e1 fa 66 9d 54 41 f6 28 5c 8f c7 df 1b 41 9a 99 99
#> [17065] 89 67 9d 54 41 5c 8f c2 f5 ac df 1b 41 5c 8f c2 f5 66 9d 54 41 9a 99 99
#> [17089] 99 4d df 1b 41 52 b8 1e 35 64 9d 54 41 85 eb 51 b8 7a de 1b 41 5c 8f c2
#> [17113] 55 5e 9d 54 41 01 03 00 00 00 01 00 00 00 05 00 00 00 66 66 66 66 a7 d1
#> [17137] 1b 41 d7 a3 70 ad 6e 9d 54 41 b8 1e 85 eb ea d1 1b 41 d7 a3 70 1d 6e 9d
#> [17161] 54 41 9a 99 99 99 24 d2 1b 41 9a 99 99 99 6e 9d 54 41 b8 1e 85 eb ea d1
#> [17185] 1b 41 48 e1 7a 14 6e 9d 54 41 66 66 66 66 a7 d1 1b 41 d7 a3 70 ad 6e 9d
#> [17209] 54 41 01 03 00 00 00 01 00 00 00 09 00 00 00 71 3d 0a d7 67 dc 1b 41 33
#> [17233] 33 33 03 5f 9d 54 41 9a 99 99 99 99 dc 1b 41 0a d7 a3 50 5d 9d 54 41 71
#> [17257] 3d 0a d7 67 dc 1b 41 5c 8f c2 f5 5e 9d 54 41 ae 47 e1 7a 78 db 1b 41 52
#> [17281] b8 1e 15 68 9d 54 41 f6 28 5c 8f 0c db 1b 41 5c 8f c2 35 6d 9d 54 41 66
#> [17305] 66 66 66 e8 da 1b 41 5c 8f c2 c5 6e 9d 54 41 f6 28 5c 8f 0c db 1b 41 9a
#> [17329] 99 99 39 6d 9d 54 41 ae 47 e1 7a 78 db 1b 41 ec 51 b8 1e 68 9d 54 41 71
#> [17353] 3d 0a d7 67 dc 1b 41 33 33 33 03 5f 9d 54 41 01 03 00 00 00 01 00 00 00
#> [17377] 05 00 00 00 d7 a3 70 3d 66 d1 1b 41 71 3d 0a 17 70 9d 54 41 d7 a3 70 3d
#> [17401] 96 d1 1b 41 48 e1 7a 04 6f 9d 54 41 7b 14 ae 47 66 d1 1b 41 48 e1 7a 14
#> [17425] 70 9d 54 41 33 33 33 33 59 d1 1b 41 a4 70 3d 7a 70 9d 54 41 d7 a3 70 3d
#> [17449] 66 d1 1b 41 71 3d 0a 17 70 9d 54 41 01 03 00 00 00 01 00 00 00 07 00 00
#> [17473] 00 66 66 66 66 e2 d8 1b 41 b8 1e 85 3b 72 9d 54 41 8f c2 f5 28 40 d9 1b
#> [17497] 41 66 66 66 56 70 9d 54 41 66 66 66 66 47 d9 1b 41 85 eb 51 58 70 9d 54
#> [17521] 41 33 33 33 33 40 d9 1b 41 52 b8 1e 55 70 9d 54 41 1f 85 eb 51 e2 d8 1b
#> [17545] 41 52 b8 1e 35 72 9d 54 41 5c 8f c2 f5 ab d8 1b 41 48 e1 7a 04 73 9d 54
#> [17569] 41 66 66 66 66 e2 d8 1b 41 b8 1e 85 3b 72 9d 54 41 01 03 00 00 00 01 00
#> [17593] 00 00 09 00 00 00 5c 8f c2 f5 21 da 1b 41 9a 99 99 99 73 9d 54 41 48 e1
#> [17617] 7a 14 64 da 1b 41 00 00 00 40 73 9d 54 41 e1 7a 14 ae 8f da 1b 41 85 eb
#> [17641] 51 28 72 9d 54 41 48 e1 7a 14 64 da 1b 41 52 b8 1e 35 73 9d 54 41 b8 1e
#> [17665] 85 eb 21 da 1b 41 52 b8 1e 95 73 9d 54 41 e1 7a 14 ae e3 d9 1b 41 52 b8
#> [17689] 1e 55 72 9d 54 41 cd cc cc cc c3 d9 1b 41 85 eb 51 a8 71 9d 54 41 3d 0a
#> [17713] d7 a3 e3 d9 1b 41 d7 a3 70 5d 72 9d 54 41 5c 8f c2 f5 21 da 1b 41 9a 99
#> [17737] 99 99 73 9d 54 41 01 03 00 00 00 01 00 00 00 05 00 00 00 d7 a3 70 3d 9c
#> [17761] d0 1b 41 e1 7a 14 fe 77 9d 54 41 14 ae 47 e1 ba d0 1b 41 ec 51 b8 5e 76
#> [17785] 9d 54 41 7b 14 ae 47 9c d0 1b 41 52 b8 1e f5 77 9d 54 41 ae 47 e1 7a 4b
#> [17809] d0 1b 41 9a 99 99 59 7b 9d 54 41 d7 a3 70 3d 9c d0 1b 41 e1 7a 14 fe 77
#> [17833] 9d 54 41 01 03 00 00 00 01 00 00 00 0f 00 00 00 1f 85 eb 51 34 d5 1b 41
#> [17857] 3d 0a d7 83 79 9d 54 41 71 3d 0a d7 18 d6 1b 41 8f c2 f5 78 78 9d 54 41
#> [17881] 71 3d 0a d7 90 d6 1b 41 8f c2 f5 b8 78 9d 54 41 33 33 33 33 e4 d6 1b 41
#> [17905] 8f c2 f5 f8 77 9d 54 41 5c 8f c2 f5 5d d7 1b 41 85 eb 51 38 76 9d 54 41
#> [17929] 0a d7 a3 70 89 d7 1b 41 33 33 33 03 76 9d 54 41 5c 8f c2 f5 5d d7 1b 41
#> [17953] 52 b8 1e 35 76 9d 54 41 8f c2 f5 28 e4 d6 1b 41 52 b8 1e f5 77 9d 54 41
#> [17977] cd cc cc cc 90 d6 1b 41 5c 8f c2 b5 78 9d 54 41 cd cc cc cc 18 d6 1b 41
#> [18001] 52 b8 1e 75 78 9d 54 41 7b 14 ae 47 34 d5 1b 41 5c 8f c2 75 79 9d 54 41
#> [18025] a4 70 3d 0a bb d4 1b 41 52 b8 1e 15 7b 9d 54 41 d7 a3 70 3d 5e d4 1b 41
#> [18049] c3 f5 28 ec 7c 9d 54 41 48 e1 7a 14 bb d4 1b 41 ae 47 e1 1a 7b 9d 54 41
#> [18073] 1f 85 eb 51 34 d5 1b 41 3d 0a d7 83 79 9d 54 41 01 03 00 00 00 01 00 00
#> [18097] 00 07 00 00 00 52 b8 1e 85 72 d3 1b 41 52 b8 1e 55 78 9d 54 41 71 3d 0a
#> [18121] d7 46 d3 1b 41 3d 0a d7 63 76 9d 54 41 f6 28 5c 8f 72 d3 1b 41 b8 1e 85
#> [18145] 5b 78 9d 54 41 0a d7 a3 70 e1 d3 1b 41 00 00 00 a0 7c 9d 54 41 3d 0a d7
#> [18169] a3 00 d4 1b 41 7b 14 ae 07 7d 9d 54 41 66 66 66 66 e1 d3 1b 41 52 b8 1e
#> [18193] 95 7c 9d 54 41 52 b8 1e 85 72 d3 1b 41 52 b8 1e 55 78 9d 54 41 01 03 00
#> [18217] 00 00 01 00 00 00 09 00 00 00 33 33 33 33 c2 cf 1b 41 71 3d 0a 97 7f 9d
#> [18241] 54 41 3d 0a d7 a3 d0 cf 1b 41 f6 28 5c 4f 7f 9d 54 41 7b 14 ae 47 c2 cf
#> [18265] 1b 41 52 b8 1e 95 7f 9d 54 41 d7 a3 70 3d 49 cf 1b 41 52 b8 1e 95 80 9d
#> [18289] 54 41 3d 0a d7 a3 dd ce 1b 41 52 b8 1e d5 81 9d 54 41 66 66 66 66 a3 ce
#> [18313] 1b 41 33 33 33 53 82 9d 54 41 85 eb 51 b8 dd ce 1b 41 71 3d 0a d7 81 9d
#> [18337] 54 41 d7 a3 70 3d 49 cf 1b 41 0a d7 a3 a0 80 9d 54 41 33 33 33 33 c2 cf
#> [18361] 1b 41 71 3d 0a 97 7f 9d 54 41 01 03 00 00 00 01 00 00 00 05 00 00 00 14
#> [18385] ae 47 e1 c6 cc 1b 41 52 b8 1e 75 83 9d 54 41 29 5c 8f c2 b5 cc 1b 41 52
#> [18409] b8 1e 75 83 9d 54 41 5c 8f c2 f5 8c cc 1b 41 1f 85 eb c1 83 9d 54 41 cd
#> [18433] cc cc cc b5 cc 1b 41 7b 14 ae 77 83 9d 54 41 14 ae 47 e1 c6 cc 1b 41 52
#> [18457] b8 1e 75 83 9d 54 41 01 03 00 00 00 01 00 00 00 07 00 00 00 f6 28 5c 8f
#> [18481] 95 cb 1b 41 8f c2 f5 f8 84 9d 54 41 71 3d 0a d7 b2 cb 1b 41 b8 1e 85 eb
#> [18505] 84 9d 54 41 ae 47 e1 7a 95 cb 1b 41 52 b8 1e f5 84 9d 54 41 ae 47 e1 7a
#> [18529] 47 cb 1b 41 48 e1 7a 94 86 9d 54 41 48 e1 7a 14 04 cb 1b 41 71 3d 0a 57
#> [18553] 8a 9d 54 41 52 b8 1e 85 47 cb 1b 41 3d 0a d7 a3 86 9d 54 41 f6 28 5c 8f
#> [18577] 95 cb 1b 41 8f c2 f5 f8 84 9d 54 41 01 03 00 00 00 01 00 00 00 05 00 00
#> [18601] 00 66 66 66 66 e0 ae 1b 41 0a d7 a3 a0 9e 9d 54 41 66 66 66 66 a4 af 1b
#> [18625] 41 7b 14 ae 37 9c 9d 54 41 c3 f5 28 5c e0 ae 1b 41 48 e1 7a 94 9e 9d 54
#> [18649] 41 d7 a3 70 3d 9e ae 1b 41 ae 47 e1 8a 9f 9d 54 41 66 66 66 66 e0 ae 1b
#> [18673] 41 0a d7 a3 a0 9e 9d 54 41 01 03 00 00 00 01 00 00 00 0d 00 00 00 3d 0a
#> [18697] d7 a3 a0 b2 1b 41 3d 0a d7 13 97 9d 54 41 29 5c 8f c2 fe b1 1b 41 3d 0a
#> [18721] d7 d3 96 9d 54 41 00 00 00 00 77 b1 1b 41 1f 85 eb 91 97 9d 54 41 85 eb
#> [18745] 51 b8 fe b1 1b 41 f6 28 5c df 96 9d 54 41 e1 7a 14 ae a0 b2 1b 41 71 3d
#> [18769] 0a 17 97 9d 54 41 3d 0a d7 a3 90 b3 1b 41 7b 14 ae d7 99 9d 54 41 71 3d
#> [18793] 0a d7 88 b4 1b 41 f6 28 5c 3f 9e 9d 54 41 7b 14 ae 47 2b b5 1b 41 8f c2
#> [18817] f5 18 a2 9d 54 41 f6 28 5c 8f 49 b5 1b 41 ec 51 b8 8e a3 9d 54 41 7b 14
#> [18841] ae 47 2b b5 1b 41 3d 0a d7 13 a2 9d 54 41 29 5c 8f c2 88 b4 1b 41 48 e1
#> [18865] 7a 34 9e 9d 54 41 3d 0a d7 a3 90 b3 1b 41 3d 0a d7 d3 99 9d 54 41 3d 0a
#> [18889] d7 a3 a0 b2 1b 41 3d 0a d7 13 97 9d 54 41 01 03 00 00 00 01 00 00 00 07
#> [18913] 00 00 00 52 b8 1e 85 ee bb 1b 41 48 e1 7a 54 a8 9d 54 41 cd cc cc cc 84
#> [18937] bb 1b 41 3d 0a d7 d3 a6 9d 54 41 66 66 66 66 65 bb 1b 41 52 b8 1e 55 a7
#> [18961] 9d 54 41 cd cc cc cc 84 bb 1b 41 f6 28 5c df a6 9d 54 41 f6 28 5c 8f ee
#> [18985] bb 1b 41 85 eb 51 58 a8 9d 54 41 85 eb 51 b8 06 bc 1b 41 52 b8 1e 25 a9
#> [19009] 9d 54 41 52 b8 1e 85 ee bb 1b 41 48 e1 7a 54 a8 9d 54 41 01 03 00 00 00
#> [19033] 01 00 00 00 05 00 00 00 cd cc cc cc c3 c8 1b 41 29 5c 8f 62 a9 9d 54 41
#> [19057] 8f c2 f5 28 5e c9 1b 41 ec 51 b8 0e a4 9d 54 41 71 3d 0a d7 c3 c8 1b 41
#> [19081] 52 b8 1e 55 a9 9d 54 41 29 5c 8f c2 33 c8 1b 41 3d 0a d7 e3 ad 9d 54 41
#> [19105] cd cc cc cc c3 c8 1b 41 29 5c 8f 62 a9 9d 54 41 01 03 00 00 00 01 00 00
#> [19129] 00 09 00 00 00 cd cc cc cc 43 bd 1b 41 29 5c 8f a2 ae 9d 54 41 48 e1 7a
#> [19153] 14 af bd 1b 41 48 e1 7a 74 ad 9d 54 41 e1 7a 14 ae b6 bd 1b 41 00 00 00
#> [19177] 60 ad 9d 54 41 a4 70 3d 0a af bd 1b 41 3d 0a d7 73 ad 9d 54 41 cd cc cc
#> [19201] cc 43 bd 1b 41 3d 0a d7 93 ae 9d 54 41 b8 1e 85 eb d1 bc 1b 41 3d 0a d7
#> [19225] 33 ae 9d 54 41 00 00 00 00 96 bc 1b 41 48 e1 7a 14 ad 9d 54 41 b8 1e 85
#> [19249] eb d1 bc 1b 41 1f 85 eb 41 ae 9d 54 41 cd cc cc cc 43 bd 1b 41 29 5c 8f
#> [19273] a2 ae 9d 54 41 01 03 00 00 00 01 00 00 00 0d 00 00 00 a4 70 3d 0a d9 b5
#> [19297] 1b 41 48 e1 7a 54 a9 9d 54 41 a4 70 3d 0a cf b5 1b 41 5c 8f c2 f5 a8 9d
#> [19321] 54 41 a4 70 3d 0a d9 b5 1b 41 52 b8 1e 55 a9 9d 54 41 d7 a3 70 3d 02 b6
#> [19345] 1b 41 71 3d 0a f7 ab 9d 54 41 00 00 00 00 1e b6 1b 41 cd cc cc 7c ae 9d
#> [19369] 54 41 b8 1e 85 eb 34 b6 1b 41 ec 51 b8 de b1 9d 54 41 00 00 00 00 46 b6
#> [19393] 1b 41 7b 14 ae f7 b3 9d 54 41 48 e1 7a 14 5a b6 1b 41 33 33 33 d3 b5 9d
#> [19417] 54 41 00 00 00 00 46 b6 1b 41 3d 0a d7 f3 b3 9d 54 41 14 ae 47 e1 34 b6
#> [19441] 1b 41 3d 0a d7 d3 b1 9d 54 41 a4 70 3d 0a 1e b6 1b 41 3d 0a d7 73 ae 9d
#> [19465] 54 41 d7 a3 70 3d 02 b6 1b 41 3d 0a d7 f3 ab 9d 54 41 a4 70 3d 0a d9 b5
#> [19489] 1b 41 48 e1 7a 54 a9 9d 54 41 01 03 00 00 00 01 00 00 00 05 00 00 00 b8
#> [19513] 1e 85 eb 1b c7 1b 41 29 5c 8f e2 b5 9d 54 41 0a d7 a3 70 55 c7 1b 41 48
#> [19537] e1 7a a4 b4 9d 54 41 5c 8f c2 f5 1b c7 1b 41 52 b8 1e d5 b5 9d 54 41 14
#> [19561] ae 47 e1 cf c6 1b 41 ae 47 e1 ba b7 9d 54 41 b8 1e 85 eb 1b c7 1b 41 29
#> [19585] 5c 8f e2 b5 9d 54 41 01 03 00 00 00 01 00 00 00 07 00 00 00 85 eb 51 b8
#> [19609] ef c5 1b 41 48 e1 7a 94 b8 9d 54 41 85 eb 51 b8 68 c5 1b 41 48 e1 7a 94
#> [19633] b7 9d 54 41 66 66 66 66 13 c5 1b 41 e1 7a 14 ce b7 9d 54 41 85 eb 51 b8
#> [19657] 68 c5 1b 41 d7 a3 70 9d b7 9d 54 41 85 eb 51 b8 ef c5 1b 41 48 e1 7a a4
#> [19681] b8 9d 54 41 29 5c 8f c2 2e c6 1b 41 e1 7a 14 1e b9 9d 54 41 85 eb 51 b8
#> [19705] ef c5 1b 41 48 e1 7a 94 b8 9d 54 41 01 03 00 00 00 01 00 00 00 07 00 00
#> [19729] 00 00 00 00 00 70 b6 1b 41 3d 0a d7 93 b7 9d 54 41 5c 8f c2 f5 5b b6 1b
#> [19753] 41 a4 70 3d fa b5 9d 54 41 00 00 00 00 70 b6 1b 41 5c 8f c2 95 b7 9d 54
#> [19777] 41 8f c2 f5 28 87 b6 1b 41 f6 28 5c ff b8 9d 54 41 d7 a3 70 3d 95 b6 1b
#> [19801] 41 5c 8f c2 a5 b9 9d 54 41 8f c2 f5 28 87 b6 1b 41 3d 0a d7 f3 b8 9d 54
#> [19825] 41 00 00 00 00 70 b6 1b 41 3d 0a d7 93 b7 9d 54 41 01 03 00 00 00 01 00
#> [19849] 00 00 09 00 00 00 f6 28 5c 8f 17 bb 1b 41 5c 8f c2 15 ad 9d 54 41 29 5c
#> [19873] 8f c2 2d bb 1b 41 f6 28 5c 8f aa 9d 54 41 f6 28 5c 8f 17 bb 1b 41 3d 0a
#> [19897] d7 13 ad 9d 54 41 66 66 66 66 eb ba 1b 41 48 e1 7a 94 b4 9d 54 41 cd cc
#> [19921] cc cc 9f ba 1b 41 3d 0a d7 b3 b9 9d 54 41 ec 51 b8 1e 8e ba 1b 41 0a d7
#> [19945] a3 80 ba 9d 54 41 29 5c 8f c2 9f ba 1b 41 71 3d 0a b7 b9 9d 54 41 0a d7
#> [19969] a3 70 eb ba 1b 41 52 b8 1e 95 b4 9d 54 41 f6 28 5c 8f 17 bb 1b 41 5c 8f
#> [19993] c2 15 ad 9d 54 41 01 03 00 00 00 01 00 00 00 07 00 00 00 7b 14 ae 47 2d
#> [20017] c0 1b 41 48 e1 7a 94 bd 9d 54 41 b8 1e 85 eb ea bf 1b 41 e1 7a 14 7e ba
#> [20041] 9d 54 41 c3 f5 28 5c 2d c0 1b 41 85 eb 51 98 bd 9d 54 41 ec 51 b8 1e 5a
#> [20065] c0 1b 41 cd cc cc bc be 9d 54 41 8f c2 f5 28 9a c0 1b 41 33 33 33 73 bf
#> [20089] 9d 54 41 a4 70 3d 0a 5a c0 1b 41 48 e1 7a b4 be 9d 54 41 7b 14 ae 47 2d
#> [20113] c0 1b 41 48 e1 7a 94 bd 9d 54 41 01 03 00 00 00 01 00 00 00 0d 00 00 00
#> [20137] 1f 85 eb 51 c0 c1 1b 41 ae 47 e1 3a bf 9d 54 41 85 eb 51 b8 26 c2 1b 41
#> [20161] 3d 0a d7 c3 be 9d 54 41 cd cc cc cc f0 c2 1b 41 85 eb 51 98 bb 9d 54 41
#> [20185] 14 ae 47 e1 6d c3 1b 41 00 00 00 e0 b9 9d 54 41 d7 a3 70 3d b3 c3 1b 41
#> [20209] e1 7a 14 5e b9 9d 54 41 71 3d 0a d7 6d c3 1b 41 48 e1 7a d4 b9 9d 54 41
#> [20233] 71 3d 0a d7 f0 c2 1b 41 48 e1 7a 94 bb 9d 54 41 85 eb 51 b8 26 c2 1b 41
#> [20257] 48 e1 7a b4 be 9d 54 41 1f 85 eb 51 c0 c1 1b 41 48 e1 7a 34 bf 9d 54 41
#> [20281] 33 33 33 33 7e c1 1b 41 48 e1 7a 34 bf 9d 54 41 3d 0a d7 a3 c4 c0 1b 41
#> [20305] 66 66 66 86 bf 9d 54 41 d7 a3 70 3d 7e c1 1b 41 b8 1e 85 3b bf 9d 54 41
#> [20329] 1f 85 eb 51 c0 c1 1b 41 ae 47 e1 3a bf 9d 54 41 01 03 00 00 00 01 00 00
#> [20353] 00 07 00 00 00 7b 14 ae 47 c6 b6 1b 41 3d 0a d7 b3 bb 9d 54 41 c3 f5 28
#> [20377] 5c bd b6 1b 41 ec 51 b8 5e bb 9d 54 41 33 33 33 33 c6 b6 1b 41 9a 99 99
#> [20401] b9 bb 9d 54 41 14 ae 47 e1 70 b7 1b 41 8f c2 f5 d8 c0 9d 54 41 a4 70 3d
#> [20425] 0a ca b7 1b 41 85 eb 51 a8 c3 9d 54 41 14 ae 47 e1 70 b7 1b 41 48 e1 7a
#> [20449] d4 c0 9d 54 41 7b 14 ae 47 c6 b6 1b 41 3d 0a d7 b3 bb 9d 54 41 01 03 00
#> [20473] 00 00 01 00 00 00 05 00 00 00 33 33 33 33 a0 b8 1b 41 1f 85 eb c1 c7 9d
#> [20497] 54 41 ae 47 e1 7a d9 b8 1b 41 8f c2 f5 f8 c6 9d 54 41 7b 14 ae 47 a0 b8
#> [20521] 1b 41 3d 0a d7 b3 c7 9d 54 41 ae 47 e1 7a 78 b8 1b 41 ec 51 b8 8e c7 9d
#> [20545] 54 41 33 33 33 33 a0 b8 1b 41 1f 85 eb c1 c7 9d 54 41
dirigible:::gdal_read_fids_all(f, 0, "", 0)
#>  [1]  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
#> [26] 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50
#> [51] 51 52 53 54 55 56 57 58
dirigible:::gdal_read_fids_ij(f, 0, "", 0, c(5, 10))
#> [1]  6  7  8  9 10 11
dirigible:::gdal_read_fids_ij(f, 0, "", 0, c(57, 58))
#> Warning in dirigible:::gdal_read_fids_ij(f, 0, "", 0, c(57, 58)): not as many
#> FIDs as requested
#> [1] 58 NA

dirigible:::gdal_read_fids_ia(f, 0, "", 0, c(55:58))
#> Warning in dirigible:::gdal_read_fids_ia(f, 0, "", 0, c(55:58)): not all FIDS
#> found
#> [1] 56 57 58 NA


str(bin <- dirigible:::gdal_dsn_read_geom_all(f, 0, "", 0, "geometry"))
#> List of 58
#>  $ : raw [1:19485] 01 03 00 00 ...
#>  $ : raw [1:20562] 01 06 00 00 ...
#>  $ : raw [1:11293] 01 03 00 00 ...
#>  $ : raw [1:17773] 01 03 00 00 ...
#>  $ : raw [1:10227] 01 06 00 00 ...
#>  $ : raw [1:1565] 01 03 00 00 ...
#>  $ : raw [1:1219] 01 06 00 00 ...
#>  $ : raw [1:7293] 01 03 00 00 ...
#>  $ : raw [1:29709] 01 03 00 00 ...
#>  $ : raw [1:44269] 01 03 00 00 ...
#>  $ : raw [1:13677] 01 03 00 00 ...
#>  $ : raw [1:10125] 01 03 00 00 ...
#>  $ : raw [1:2941] 01 03 00 00 ...
#>  $ : raw [1:3008] 01 06 00 00 ...
#>  $ : raw [1:3357] 01 03 00 00 ...
#>  $ : raw [1:46896] 01 06 00 00 ...
#>  $ : raw [1:16877] 01 03 00 00 ...
#>  $ : raw [1:5677] 01 03 00 00 ...
#>  $ : raw [1:7501] 01 03 00 00 ...
#>  $ : raw [1:36461] 01 03 00 00 ...
#>  $ : raw [1:17453] 01 03 00 00 ...
#>  $ : raw [1:10301] 01 03 00 00 ...
#>  $ : raw [1:7677] 01 03 00 00 ...
#>  $ : raw [1:6877] 01 03 00 00 ...
#>  $ : raw [1:18509] 01 03 00 00 ...
#>  $ : raw [1:2445] 01 03 00 00 ...
#>  $ : raw [1:6029] 01 03 00 00 ...
#>  $ : raw [1:15373] 01 03 00 00 ...
#>  $ : raw [1:4317] 01 03 00 00 ...
#>  $ : raw [1:5677] 01 03 00 00 ...
#>  $ : raw [1:9229] 01 03 00 00 ...
#>  $ : raw [1:5933] 01 03 00 00 ...
#>  $ : raw [1:13949] 01 03 00 00 ...
#>  $ : raw [1:2573] 01 03 00 00 ...
#>  $ : raw [1:9277] 01 03 00 00 ...
#>  $ : raw [1:10349] 01 03 00 00 ...
#>  $ : raw [1:6541] 01 03 00 00 ...
#>  $ : raw [1:6557] 01 03 00 00 ...
#>  $ : raw [1:2013] 01 03 00 00 ...
#>  $ : raw [1:2269] 01 03 00 00 ...
#>  $ : raw [1:19517] 01 03 00 00 ...
#>  $ : raw [1:1443] 01 06 00 00 ...
#>  $ : raw [1:67517] 01 03 00 00 ...
#>  $ : raw [1:22877] 01 03 00 00 ...
#>  $ : raw [1:611] 01 06 00 00 ...
#>  $ : raw [1:3581] 01 03 00 00 ...
#>  $ : raw [1:5037] 01 03 00 00 ...
#>  $ : raw [1:11741] 01 03 00 00 ...
#>  $ : raw [1:16941] 01 03 00 00 ...
#>  $ : raw [1:120253] 01 03 00 00 ...
#>  $ : raw [1:106204] 01 06 00 00 ...
#>  $ : raw [1:20525] 01 03 00 00 ...
#>  $ : raw [1:9357] 01 03 00 00 ...
#>  $ : raw [1:2429] 01 03 00 00 ...
#>  $ : raw [1:451] 01 06 00 00 ...
#>  $ : raw [1:1661] 01 03 00 00 ...
#>  $ : raw [1:1917] 01 03 00 00 ...
#>  $ : raw [1:4029] 01 03 00 00 ...

str(bin <- dirigible:::gdal_dsn_read_geom_ij(f, 0, "", 0, "geometry", c(1, 3)))
#> List of 3
#>  $ : raw [1:20562] 01 06 00 00 ...
#>  $ : raw [1:11293] 01 03 00 00 ...
#>  $ : raw [1:17773] 01 03 00 00 ...
str(bin <- dirigible:::gdal_dsn_read_geom_ij(f, 0, "", 0, "geometry", c(56, 57)))
#> List of 2
#>  $ : raw [1:1917] 01 03 00 00 ...
#>  $ : raw [1:4029] 01 03 00 00 ...

str(bin <- dirigible:::gdal_dsn_read_geom_ia(f, 0, "", 0, "geometry", c(1, 3)))
#> List of 2
#>  $ : raw [1:20562] 01 06 00 00 ...
#>  $ : raw [1:17773] 01 03 00 00 ...
str(bin <- dirigible:::gdal_dsn_read_geom_ia(f, 0, "", 0, "geometry", c(50, 57)))
#> List of 2
#>  $ : raw [1:106204] 01 06 00 00 ...
#>  $ : raw [1:4029] 01 03 00 00 ...

## by FID
str(bin <- dirigible:::gdal_dsn_read_geom_fa(f, 0, "", 0, "geometry", c(58, 1)))
#> List of 2
#>  $ : raw [1:4029] 01 03 00 00 ...
#>  $ : raw [1:19485] 01 03 00 00 ...
str(bin <- dirigible:::gdal_dsn_read_geom_fa(f, 0, "", 0, "geometry", c(1, 58, 58, 1)))
#> List of 4
#>  $ : raw [1:19485] 01 03 00 00 ...
#>  $ : raw [1:4029] 01 03 00 00 ...
#>  $ : raw [1:4029] 01 03 00 00 ...
#>  $ : raw [1:19485] 01 03 00 00 ...

## read fields

dirigible:::gdal_dsn_read_fields_all(f, 0, "", 0, fid_column_name = character())
#> $LOCAL_ID
#>  [1] 100422 100366 100337 100308 100263 100311 100377 100407 100429 100423
#> [11] 100387 100397 100328 100269 100416 100437 100357 100331 100370 100421
#> [21] 100460 100401 100398 100425 100325 100412 100383 100324 100378 100355
#> [31] 100373 100351 100326 100419 100360 100364 100400 100420 100415 100363
#> [41] 100428 100335 100418 100396 100346 100403 100388 100411 100424 100446
#> [51] 100430 100440 100385 100417 100393 100394 100389 100381
#> 
#> $NAME
#>  [1] "Caveside"           "Weegena"            "Kimberley"         
#>  [4] "Parkham"            "Frankford"          "Bridgenorth"       
#>  [7] "Summerhill"         "Mayberry"           "Western Creek"     
#> [10] "Meander"            "Deloraine"          "Carrick"           
#> [13] "Rosevale"           "Sassafras"          "Dairy Plains"      
#> [16] "Liffey"             "Westbury"           "Selbourne"         
#> [19] "Blackstone Heights" "Cradle Mountain"    "Lake St Clair"     
#> [22] "Chudleigh"          "Red Hills"          "Golden Valley"     
#> [25] "Elizabeth Town"     "Whitemore"          "Hagley"            
#> [28] "Birralee"           "Quamby Bend"        "Trevallyn"         
#> [31] "Mount Roland"       "Moltema"            "Reedy Marsh"       
#> [34] "Cluan"              "Westwood"           "Middlesex"         
#> [37] "Liena"              "Montana"            "Quamby Brook"      
#> [40] "Weetah"             "Bracknell"          "Riverside"         
#> [43] "Mersey Forest"      "Mole Creek"         "Lower Beulah"      
#> [46] "Needles"            "Exton"              "Osmaston"          
#> [49] "Bishopsbourne"      "Walls of Jerusalem" "Central Plateau"   
#> [52] "Jackeys Marsh"      "Dunorlan"           "Oaks"              
#> [55] "Longford"           "Hadspen"            "Travellers Rest"   
#> [58] "Prospect Vale"     
#> 
#> $POSTCODE
#>  [1] 7304 7304 7304 7304 7275 7277 7250 7304 7304 7304 7304 7291 7292 7307 7304
#> [16] 7301 7303 7292 7250 7306 7140 7304 7304 7304 7304 7303 7292 7303 7292 7250
#> [31] 7306 7304 7304 7303 7292 7306 7304 7304 7304 7304 7302 7250 7304 7304 7306
#> [46] 7304 7303 7303 7301 7304 7304 7304 7304 7303 7301 7290 7250 7250
#> 
#> $PLAN_REF
#>  [1] "CPR5322" "CPR5327" "CPR5361" "CPR5327" "CPR5142" "CPR5140" "CPR5150"
#>  [8] "CPR5321" "CPR5322" "CPR5322" "CPR5327" "CPR5329" "CPR5329" "CPR5238"
#> [15] "CPR5322" "CPR5323" "CPR5328" "CPR5329" "CPR5326" "CPR5363" "CPR5570"
#> [22] "CPR5322" "CPR5322" "CPR5323" "CPR5327" "CPR5323" "CPR5329" "CPR5328"
#> [29] "CPR5329" "CPR5155" "CPR5359" "CPR5327" "CPR5328" "CPR5323" "CPR5329"
#> [36] "CPR5362" "CPR5321" "CPR5322" "CPR5323" "CPR5327" "CPR5323" "CPR5140"
#> [43] "CPR5321" "CPR5321" "CPR5361" "CPR5322" "CPR5328" "CPR5323" "CPR5044"
#> [50] "CPR5571" "CPR5569" "CPR5322" "CPR5327" "CPR5323" "CPR5044" "CPR5329"
#> [57] "CPR5326" "CPR5325"
#> 
#> $GAZ_DATE
#>  [1] "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01"
#>  [6] "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01"
#> [11] "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01"
#> [16] "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01"
#> [21] "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01"
#> [26] "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01"
#> [31] "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01"
#> [36] "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01"
#> [41] "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01"
#> [46] "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01"
#> [51] "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01"
#> [56] "1970-01-01" "1970-01-01" "1970-01-01"
#> 
#> $NOM_REG_NO
#>  [1] "947L"   "1300M"  "1063T"  "1179Y"  "1003Q"  "925X"   "4806G"  "1116M" 
#>  [9] "1304X"  "1117P"  "268G"   "943C"   "1219E"  "1231T"  "38109M" "1090C" 
#> [17] "1303T"  "1630F"  "38108A" "38113D" "39197M" "951C"   "2399W"  "1018H" 
#> [25] "280X"   "1311T"  "1030Y"  "909X"   "7650E"  "4807J"  "38115E" "1124M" 
#> [33] "1628T"  "2828H"  "1265S"  "38114Q" "1088Q"  "1126R"  "1196B"  "599C"  
#> [41] "919C"   "25L"    "38110N" "1123K"  "907R"   "1150R"  "281A"   "2839P" 
#> [49] "910F"   "39206M" "38974G" "1053P"  "983W"   "1162C"  "348G"   "1029P" 
#> [57] "38111C" "58H"   
#> 
#> $UFI
#>  [1] "{4a5db4da-ca19-41a0-8dd4-c28a14bbee18}"
#>  [2] "{253b676e-2791-469c-ac5e-9cb3a95cc158}"
#>  [3] "{75f60a99-4c58-4d3e-911d-bbaa9a04164c}"
#>  [4] "{b008d456-4e80-4237-80f6-a26c03817e3c}"
#>  [5] "{953f4006-6397-4d03-af97-507eab170862}"
#>  [6] "{5cf8e2a3-631c-4d52-a79c-0ce475f76848}"
#>  [7] "{03a9def3-2f58-43da-b4ec-bc8f2e9a2eab}"
#>  [8] "{d71e82ff-7416-4a0b-b803-8764b5bcb451}"
#>  [9] "{a3f54d77-bcb7-4809-940d-040f14d8640f}"
#> [10] "{128ae910-e6d0-4420-a3d6-daf031f44e67}"
#> [11] "{b1b54795-ded8-49ee-8567-cf286545dc8f}"
#> [12] "{566f5fdb-7f84-4ff6-8dd6-63e4454e1fd3}"
#> [13] "{2eeb3ff9-a32b-4715-87c9-8da102fe60b3}"
#> [14] "{4872b9bc-bc96-49a4-a765-88774bdfe098}"
#> [15] "{88725292-d307-405d-8de3-12d3c0a77ab6}"
#> [16] "{73c660ab-fdc7-4b31-b22d-78cc71770cbc}"
#> [17] "{86c19cc3-cd18-44e3-a5b5-ecaf79780571}"
#> [18] "{2a0859b4-4656-4f6a-aa2b-cb933bd108de}"
#> [19] "{922d7c01-85d5-4a4f-ab3c-da7b0382861b}"
#> [20] "{e17d940e-0240-4a12-acef-d2377386439d}"
#> [21] "{a87b5dc0-98c5-44fa-9fe5-50701096785e}"
#> [22] "{184bddd2-e6c2-4e2a-99be-2c690d31c97b}"
#> [23] "{b392f80d-2922-4d91-8a77-a5278ca7c787}"
#> [24] "{c33654f5-41be-43a8-b506-80416a2bda25}"
#> [25] "{f421c569-e274-488b-a195-6c487648053c}"
#> [26] "{2349bffa-a979-49ae-8a43-b69aa0f24370}"
#> [27] "{69c3c915-fc8d-4001-8b15-40042f3e7117}"
#> [28] "{fda6b877-69b3-42c8-abc5-d92cf1a98b1e}"
#> [29] "{69eb3d26-614d-464b-b59b-aa9bc36d8a54}"
#> [30] "{887d42bf-4f02-42f1-9135-2f5b8d78e8cc}"
#> [31] "{a00cfee1-a76c-47c6-bdb0-61408c34037b}"
#> [32] "{9e283e9b-eebb-430b-b4b5-0f129ded465d}"
#> [33] "{7f74079d-0b95-40f1-a53d-94902d5e3e09}"
#> [34] "{9353c745-9bd5-4182-8cf2-01791a6b2d64}"
#> [35] "{86abde4b-6458-4d12-8d3c-1af5f1f123a7}"
#> [36] "{4347282e-7616-451b-b421-e53c36c7e064}"
#> [37] "{60767eb6-79ad-490a-bcf4-7f6f532c13c5}"
#> [38] "{cee33f2c-c615-4b4a-bf07-9e4578e6a5b9}"
#> [39] "{e74609a2-b8af-47b6-b5c1-480668f3192f}"
#> [40] "{7b954d94-6f77-45e8-8693-dfdca8d47d52}"
#> [41] "{1effaa60-8abe-4cba-8c26-249aad2046d6}"
#> [42] "{79cec965-7882-406a-83d0-13787ce055c7}"
#> [43] "{0f06fa94-73cf-42ae-a2d2-144ee7c07a68}"
#> [44] "{a5cf8425-3496-492f-9878-22ccb4af7267}"
#> [45] "{50845792-6f5d-4d0d-b85c-ec95993e7097}"
#> [46] "{6f6b41b7-555f-4f31-a37f-4e424167d9f1}"
#> [47] "{a3aa7385-0fbb-4dc2-b13b-9910b2507e64}"
#> [48] "{ef412a61-9aae-48ca-8257-df0af892956d}"
#> [49] "{39413160-812c-463d-820d-d8f4c1aa82c8}"
#> [50] "{5a0bd757-9a8b-45c2-b497-6edaaec7052d}"
#> [51] "{24b22df7-c905-406c-a19e-cc77fd294507}"
#> [52] "{5c4c3d24-ca65-40cd-a11e-e501248f9f65}"
#> [53] "{a1990658-8a33-4175-aace-9a5b6a5e5962}"
#> [54] "{f671bc53-71cb-4d47-9b86-2a5afa7bbf99}"
#> [55] "{84d3f931-7ef9-4d7e-9bd9-f47e095511a0}"
#> [56] "{4820bc13-ab68-4dd9-a1c5-2856e7af44b3}"
#> [57] "{a3960c6a-0b2b-43a7-9a6d-dbfc1a189966}"
#> [58] "{6af4252a-c213-4052-9baf-f4315240075f}"
#> 
#> $CREATED_ON
#>  [1] "2016-03-04 10:42:37" "2015-06-19 13:46:50" "2016-09-16 10:54:56"
#>  [4] "2014-06-06 16:50:22" "2014-12-08 09:07:12" "2015-05-11 10:49:09"
#>  [7] "2017-01-12 13:04:10" "2014-02-26 14:44:53" "2014-06-06 16:50:24"
#> [10] "2016-03-03 16:18:06" "2017-05-10 14:26:15" "2017-01-12 13:04:10"
#> [13] "2016-11-15 12:54:45" "2017-05-10 14:26:15" "2014-06-06 16:50:26"
#> [16] "2016-03-03 16:18:06" "2017-04-03 12:24:18" "2016-11-15 12:54:45"
#> [19] "2017-05-10 14:26:15" "2014-02-26 14:44:53" "2014-02-26 14:45:03"
#> [22] "2017-04-03 12:24:18" "2014-06-06 16:50:25" "2015-12-14 08:46:42"
#> [25] "2016-11-15 12:54:45" "2017-04-03 12:24:18" "2016-11-15 12:54:45"
#> [28] "2014-12-08 09:07:14" "2015-10-28 11:28:12" "2015-07-31 12:13:03"
#> [31] "2014-12-08 09:07:15" "2015-06-19 13:46:51" "2014-06-06 16:50:23"
#> [34] "2017-04-03 12:24:18" "2017-01-12 13:04:10" "2016-03-04 10:42:37"
#> [37] "2014-12-08 09:07:18" "2014-06-06 16:50:24" "2017-05-10 14:26:15"
#> [40] "2014-06-06 16:50:22" "2016-03-03 16:18:05" "2015-12-14 08:46:42"
#> [43] "2016-03-02 12:05:14" "2017-04-03 12:58:01" "2017-04-03 12:58:01"
#> [46] "2017-04-03 12:24:18" "2012-03-08 14:02:08" "2012-02-03 11:37:53"
#> [49] "2017-05-22 15:05:14" "2015-10-28 11:28:09" "2017-05-10 14:26:15"
#> [52] "2016-03-03 16:18:05" "2017-01-12 13:04:10" "2014-06-17 14:08:36"
#> [55] "2017-05-22 15:05:14" "2017-01-12 13:04:10" "2015-04-08 11:29:27"
#> [58] "2017-05-10 14:26:15"
#> 
#> $LIST_GUID
#>  [1] "{839edd46-01a7-4a45-9d97-499962fa952b}"
#>  [2] "{de35ebd4-0ac0-4299-947d-87d07c69426a}"
#>  [3] "{73ced9ad-ee9a-41d5-a5bc-c95c4ab948d9}"
#>  [4] "{37f17d1f-d2a0-4b78-ba0d-e5f62f216658}"
#>  [5] "{47f3a313-913f-4f83-8dc1-021907f9ee80}"
#>  [6] "{425d01cc-223b-4348-b965-384a98fd7999}"
#>  [7] "{b67a3c2b-08cb-42d4-825b-e40fcab2fb5e}"
#>  [8] "{60ba6d69-2021-462e-936f-544ee7c2595b}"
#>  [9] "{fe452b60-7ef7-44f2-8741-71126b4fb0a3}"
#> [10] "{876e5283-2a5d-46aa-92da-4aad5e02cb8e}"
#> [11] "{f29cc705-df86-4627-9725-f022c7a7665a}"
#> [12] "{b4dfcc34-6125-419a-9219-eb2563696623}"
#> [13] "{0371b2e7-c72f-447a-991e-451ec78dfb62}"
#> [14] "{6495b253-047d-40be-a842-694907a4700e}"
#> [15] "{3d221f2f-2af7-4150-8e11-89e612c9620a}"
#> [16] "{90808a7b-5134-4352-a0a0-9eb941c1c6fc}"
#> [17] "{28538e58-ea0a-4876-ba28-77fb91fb4c56}"
#> [18] "{9f9bf307-bb1d-4a44-9997-f4ef0ccb771a}"
#> [19] "{53d25ef5-ee06-4040-93e4-f40adc31452d}"
#> [20] "{d53dd2a5-ff7a-4c6f-98b4-a8bc463a83ea}"
#> [21] "{d508dd6c-c05a-4085-9043-52342bec9b42}"
#> [22] "{1fe385b9-753d-4abb-a485-5dc8811c47a0}"
#> [23] "{ec7a8d40-af0f-431e-8f42-313f68b59546}"
#> [24] "{24f934f6-c9e2-4856-82dc-6794e4c3e2a8}"
#> [25] "{fca48545-c9e9-4971-8146-52a5e625e38e}"
#> [26] "{3c7aea1e-2c10-4e0d-a437-2af95b984aac}"
#> [27] "{69d4b471-8414-4ee0-b8ee-37a54df00d26}"
#> [28] "{51b7f399-0e8f-4d16-91d9-b64e89e06e5c}"
#> [29] "{e61f737f-2531-4beb-b13e-e929c56c0302}"
#> [30] "{6b478510-78fa-4fba-afa2-3ef85190cee4}"
#> [31] "{c72f1ca0-11f8-4481-94c4-4ec11cb7fa88}"
#> [32] "{14b518dd-04b4-4edc-a566-a0712e4f81ab}"
#> [33] "{a91a9168-5623-4c27-9bf5-921d3b2fad55}"
#> [34] "{91390c0f-8706-41b7-b85c-d776be118f8c}"
#> [35] "{f5402dd6-8236-4165-aad2-df6510f5b45b}"
#> [36] "{e6e6878b-d4bd-470d-85d3-3a0266644bb0}"
#> [37] "{172a69bd-6ab6-4125-8db7-b28cf447e58a}"
#> [38] "{c517afd0-d363-4ee6-9daf-b0b92859754e}"
#> [39] "{5222cf87-76e7-42d5-885d-05adfd833113}"
#> [40] "{632db980-62f5-4676-bb10-ca6c8636b2ca}"
#> [41] "{036a25e8-5ae6-4758-b047-8e2a48f7078a}"
#> [42] "{7aab64fc-9092-430d-adf5-a6187ccac639}"
#> [43] "{d8b29014-d1f0-4537-90b0-2fc69409fa8f}"
#> [44] "{28abf88d-446c-4396-a0d1-5a8f487d626d}"
#> [45] "{630d7192-57a9-4655-8ff2-732c5d617bdf}"
#> [46] "{48a6c36b-7cd7-44b3-a051-8331a10469bd}"
#> [47] "{d6a63e2d-6c78-45f1-8f78-c97a7a0f9798}"
#> [48] "{c256e5de-6bb6-49c2-a9b7-eb1cc4a4358a}"
#> [49] "{36d9a9de-c587-4d06-8cc5-a40d735ea929}"
#> [50] "{028ad03f-fd0f-41e7-88ff-288a65388203}"
#> [51] "{725932a9-aec5-4aad-b083-589b5650cc31}"
#> [52] "{f8ee3df0-d971-424e-8620-d7dc1b664dbd}"
#> [53] "{4281ae3b-465d-4a74-9ffe-f3255e3d0c25}"
#> [54] "{428423a1-6297-44f6-824c-938c1a7d7877}"
#> [55] "{7c0633e1-2e04-4d20-8b8c-1c5dc688ec12}"
#> [56] "{f65f7d94-39aa-46a4-8a39-019f0b6cd468}"
#> [57] "{e8392030-0ef6-4617-8506-74ee4e19b36b}"
#> [58] "{0e788275-bc03-4b2d-b9b0-2b9d13793154}"
#> 
#> $SHAPE_AREA
#>  [1]   -9999   -9999   -9999   -9999   -9999   -9999 2302643   -9999   -9999
#> [10]   -9999   -9999   -9999   -9999   -9999   -9999   -9999   -9999   -9999
#> [19] 7832529   -9999   -9999   -9999   -9999   -9999   -9999   -9999   -9999
#> [28]   -9999 8486736 9852836   -9999   -9999   -9999   -9999   -9999   -9999
#> [37]   -9999   -9999   -9999   -9999   -9999   -9999   -9999   -9999   -9999
#> [46]   -9999   -9999   -9999   -9999   -9999   -9999   -9999   -9999   -9999
#> [55]   -9999   -9999 6136801   -9999
#> 
#> $SHAPE_LEN
#>  [1]  39785.880  31587.543  35693.318  67614.515  70140.783  38156.704
#>  [7]   9517.766  38379.277  42948.737  94270.024  68696.703  49516.829
#> [13]  37643.704  83539.932  28668.151  69478.598  65351.499  48579.511
#> [19]  12135.978 200297.904 209224.675  38611.683  34268.901  56009.364
#> [25]  55266.161  43353.208  48065.647  40268.776  19106.809  16050.466
#> [31]  49329.223  28920.182  71731.265  36651.119  54107.685 114901.874
#> [37]  40650.548  26196.383  27696.797  26183.768  53843.912  44108.040
#> [43] 152351.454  75018.037  41961.578  24157.077  28578.244  22549.288
#> [49]  31002.635 166213.126 474887.643  43304.587  29551.180  20044.890
#> [55]  94995.922  14480.093  12276.269  16297.430

dirigible:::gdal_dsn_read_fields_ij(f, 0, "", 0, fid_column_name = character(), 1:2)
#> $LOCAL_ID
#> [1] 100366 100337
#> 
#> $NAME
#> [1] "Weegena"   "Kimberley"
#> 
#> $POSTCODE
#> [1] 7304 7304
#> 
#> $PLAN_REF
#> [1] "CPR5327" "CPR5361"
#> 
#> $GAZ_DATE
#> [1] "1970-01-01" "1970-01-01"
#> 
#> $NOM_REG_NO
#> [1] "1300M" "1063T"
#> 
#> $UFI
#> [1] "{253b676e-2791-469c-ac5e-9cb3a95cc158}"
#> [2] "{75f60a99-4c58-4d3e-911d-bbaa9a04164c}"
#> 
#> $CREATED_ON
#> [1] "2015-06-19 13:46:50" "2016-09-16 10:54:56"
#> 
#> $LIST_GUID
#> [1] "{de35ebd4-0ac0-4299-947d-87d07c69426a}"
#> [2] "{73ced9ad-ee9a-41d5-a5bc-c95c4ab948d9}"
#> 
#> $SHAPE_AREA
#> [1] -9999 -9999
#> 
#> $SHAPE_LEN
#> [1] 31587.54 35693.32


dirigible:::gdal_dsn_read_fields_ia(f, 0, "", 0, fid_column_name = character(), c(1, 34, 57))
#> $LOCAL_ID
#> [1] 100366 100360 100381
#> 
#> $NAME
#> [1] "Weegena"       "Westwood"      "Prospect Vale"
#> 
#> $POSTCODE
#> [1] 7304 7292 7250
#> 
#> $PLAN_REF
#> [1] "CPR5327" "CPR5329" "CPR5325"
#> 
#> $GAZ_DATE
#> [1] "1970-01-01" "1970-01-01" "1970-01-01"
#> 
#> $NOM_REG_NO
#> [1] "1300M" "1265S" "58H"  
#> 
#> $UFI
#> [1] "{253b676e-2791-469c-ac5e-9cb3a95cc158}"
#> [2] "{86abde4b-6458-4d12-8d3c-1af5f1f123a7}"
#> [3] "{6af4252a-c213-4052-9baf-f4315240075f}"
#> 
#> $CREATED_ON
#> [1] "2015-06-19 13:46:50" "2017-01-12 13:04:10" "2017-05-10 14:26:15"
#> 
#> $LIST_GUID
#> [1] "{de35ebd4-0ac0-4299-947d-87d07c69426a}"
#> [2] "{f5402dd6-8236-4165-aad2-df6510f5b45b}"
#> [3] "{0e788275-bc03-4b2d-b9b0-2b9d13793154}"
#> 
#> $SHAPE_AREA
#> [1] -9999 -9999 -9999
#> 
#> $SHAPE_LEN
#> [1] 31587.54 54107.69 16297.43

dirigible:::gdal_dsn_read_fields_fa(f, 0, "", 0, fid_column_name = character(), c(58, 1, 0, 5))
#> Warning in dirigible:::gdal_dsn_read_fields_fa(f, 0, "", 0, fid_column_name =
#> character(), : FID not found 0
#> $LOCAL_ID
#> [1] 100381 100422      0 100263
#> 
#> $NAME
#> [1] "Prospect Vale" "Caveside"      ""              "Frankford"    
#> 
#> $POSTCODE
#> [1] 7250 7304    0 7275
#> 
#> $PLAN_REF
#> [1] "CPR5325" "CPR5322" ""        "CPR5142"
#> 
#> $GAZ_DATE
#> [1] "1970-01-01" "1970-01-01" "1970-01-01" "1970-01-01"
#> 
#> $NOM_REG_NO
#> [1] "58H"   "947L"  ""      "1003Q"
#> 
#> $UFI
#> [1] "{6af4252a-c213-4052-9baf-f4315240075f}"
#> [2] "{4a5db4da-ca19-41a0-8dd4-c28a14bbee18}"
#> [3] ""                                      
#> [4] "{953f4006-6397-4d03-af97-507eab170862}"
#> 
#> $CREATED_ON
#> [1] "2017-05-10 14:26:15" "2016-03-04 10:42:37" ""                   
#> [4] "2014-12-08 09:07:12"
#> 
#> $LIST_GUID
#> [1] "{0e788275-bc03-4b2d-b9b0-2b9d13793154}"
#> [2] "{839edd46-01a7-4a45-9d97-499962fa952b}"
#> [3] ""                                      
#> [4] "{47f3a313-913f-4f83-8dc1-021907f9ee80}"
#> 
#> $SHAPE_AREA
#> [1] -9999 -9999     0 -9999
#> 
#> $SHAPE_LEN
#> [1] 16297.43 39785.88     0.00 70140.78






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

### Warping in memory

Very WIP, needs a lot of thought around UX.

See <https://github.com/mdsumner/dirigible/issues/4>

-----

## Code of Conduct

Please note that the dirigible project is released with a [Contributor
Code of
Conduct](https://contributor-covenant.org/version/2/0/CODE_OF_CONDUCT.html).
By contributing to this project, you agree to abide by its terms.
