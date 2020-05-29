gcpfile <- system.file("extdata/gcps/volcano_gcp.tif", package = "vapour", mustWork = TRUE)
f <- system.file("extdata/tab/list_locality_postcode_meander_valley.tab", package = "vapour")

tab <- dirigible:::gh_GDALOpenEx(f)
gcp <- dirigible:::gh_GDALOpenEx(gcpfile)
dirigible:::gh_GDALClose(tab)
dirigible:::gh_GDALClose(gcp)

dm <- dirigible:::gh_GetGDALDriverManager()
dirigible:::gh_GetDriverCount(dm)
