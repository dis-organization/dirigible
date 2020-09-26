
## absolutely minimal rasterio and warp
f <- system.file("extdata/bluemarble.tif", package = "dirigible", mustWork = TRUE)



## wrap up the setup

warp <- function(filename, target, ..., band = NA_integer_, src_wkt = "") {
  if (!is.character(filename) || nchar(filename) < 1) {
    stop("'filename' must be a valid file path, or url, or GDAL data source string")
  }
  gt <- affinity:::raster_to_gt(target)
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

maps::map(add = TRUE)



pmap <- function(prj, add = TRUE, sub = 10, ...) {
  xy <- do.call(cbind, maps::map(plot = FALSE)[1:2])
  if (sub > 1) {
    xy <- xy[seq(1, nrow(xy), by = sub), ]
    if (nrow(xy) < 2) stop("sub too high, try 'sub = 20'")
  }

  if (!missing(prj)) {
    if (!inherits(prj, "character")) {
      prj <- crsmeta::crs_proj(prj)
    }
    if (!is.na(prj)) {
      #browser()
      xy <- reproj::reproj(xy, prj, source = "+proj=longlat +datum=WGS84")[,1:2]
    }
  }
  if (add) {
    usr <- spex::spex()
    usr <- spex::buffer_extent(usr, mean(c(diff(spex::xlim(usr)), diff(spex::ylim(usr))))/10)
    asub <- xy[,1L] >= raster::xmin(usr) & xy[,1L] <= raster::xmax(usr) &
      xy[,2L] >= raster::ymin(usr) & xy[,2L] <= raster::ymax(usr)
    xy <- xy[asub, ]
  }
  if (nrow(xy) < 2) stop("sub too high, try 'sub = 20'")

  if (add) lines(xy, ...) else plot(xy, type = "l", ...)
}

plot(w2)
pmap(w2, col = "red", lwd = 2, add = T)
