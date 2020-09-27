
## absolutely minimal rasterio and warp
f <- system.file("extdata/bluemarble.tif", package = "dirigible", mustWork = TRUE)



## wrap up the setup

warp <- function(filename, target, ..., band = NA_integer_, src_wkt = "") {
  if (!is.character(filename) || any(nchar(filename) < 1)) {
    stop("'filename' must be a valid file path, or url, or GDAL data source string")
  }
  gt <- affinity:::raster_to_gt(target)   ## hypertidy/affinity
  dm <- dim(target)[2:1]
  wkt <- sf::st_crs(raster::projection(target))$wkt
  if (anyNA(band)) {
    info <- dirigible:::raster_info_gdal_cpp(filename, min_max = FALSE)
    band <- seq_len(info$bands)
  }
  dirigible:::warp_in_memory_gdal_cpp(filename, source_WKT = src_wkt,
                                      target_geotransform = gt,
                                      target_dim = dm,
                                      target_WKT = wkt,
                                      band = band)
}


## no warp required just RasterIO
r1 <- raster(extent(120, 160, -55, -30), nrows = 300, ncols = 400, crs = "+proj=longlat +datum=WGS84")
## warp
r2 <- raster(extent(c(-1, 1, -1, 1) * 1e6), nrows = 300, ncols = 400, crs = "+proj=laea +lon_0=120 +lat_0=30 +datum=WGS84")

set_raster <- function(x, v) {
  out <- setValues(raster::brick(replicate(length(v), x, simplify = FALSE)), do.call(cbind, v))
  if (nlayers(out) < 2) out <- out[[1L]]
  out
}
w1 <- set_raster(r1, warp(f, r1))
w2 <- set_raster(r2, warp(f, r2))


## select only green
r0 <- set_raster(r1, warp(f, r1, band = 1))
g0 <- set_raster(r1, warp(f, r1, band = 2))
b0 <- set_raster(r1, warp(f, r1, band = 3))
plotRGB(brick(r0, g0, b0))

plotRGB(w1)
plotRGB(w2)


## multiple input files?

ff <- raadfiles::rema_8m_files()

files <- c("/rdsi/PUBLIC/raad/data/ftp.data.pgc.umn.edu/elev/dem/setsm/REMA/mosaic/v1.0/8m/37_23/37_23_8m_dem.tif",
  "/rdsi/PUBLIC/raad/data/ftp.data.pgc.umn.edu/elev/dem/setsm/REMA/mosaic/v1.0/8m/37_24/37_24_8m_dem.tif"
)

ofiles <- c(tempfile(fileext = ".tif"), tempfile(fileext = ".tif"))
writeRaster(as_raster(lazyraster(files[1])), ofiles[1])
writeRaster(as_raster(lazyraster(files[2])), ofiles[2])


library(raster)
ex <- raster::union(extent(raster(files[1])), extent(raster(files[2])))
rr <- raster(ex, nrows = 200, ncols = 800, crs = projection(raster(files[1])))
a <- warp(ofiles, rr)
