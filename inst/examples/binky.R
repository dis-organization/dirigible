gcpfile <- system.file("extdata/gcps/volcano_gcp.tif", package = "vapour", mustWork = TRUE)
f <- system.file("extdata/tab/list_locality_postcode_meander_valley.tab", package = "vapour")

tab <- dirigible:::gh_GDALOpenEx(f)
gcp <- dirigible:::gh_GDALOpenEx(gcpfile)
dirigible:::gh_GDALClose(tab)
dirigible:::gh_GDALClose(gcp)

dm <- dirigible:::gh_GetGDALDriverManager()
dirigible:::gh_GetDriverCount(dm)


## GetLayer, getNextFeature
ds <- dirigible:::gh_GDALOpenEx(f)
lyr <- dirigible:::gh_ExecuteSQL(ds, "SELECT * FROM list_locality_postcode_meander_valley", 0)
lyr <- dirigible:::gh_GetLayer(ds, 0L)
#dirigible:::gh_GetNextFeature(lyr)  ## after 58 they are <pointer: (nil)>
# BUT a memory leaker crash in-waiting

## so, but no this is dangerous and doesn't work either

# feat <-   dirigible:::gh_GetNextFeature(lyr)
# str(wkb <- dirigible:::gh_exportToWkb(dirigible:::gh_getGeometryRef(feat)))
# dirigible:::gh_DestroyFeature(feat)


