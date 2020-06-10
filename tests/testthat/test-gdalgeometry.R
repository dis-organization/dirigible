test_that("multiplication works", {
  library(dirigible)
  f <- system.file("extdata/tab/list_locality_postcode_meander_valley.tab", package = "vapour")
  dirigible:::gdal_read_fids_all(f, 0, "", 0)
  dirigible:::gdal_read_fids_ij(f, 0, "", 0, c(5, 10))
  dirigible:::gdal_read_fids_ij(f, 0, "", 0, c(57, 58))

  dirigible:::gdal_read_fids_ia(f, 0, "", 0, c(55:58))


  str(bin <- dirigible:::gdal_dsn_read_geom_all(f, 0, "", 0, "geometry"))

  str(bin <- dirigible:::gdal_dsn_read_geom_ij(f, 0, "", 0, "geometry", c(1, 3)))
  str(bin <- dirigible:::gdal_dsn_read_geom_ij(f, 0, "", 0, "geometry", c(56, 57)))

  str(bin <- dirigible:::gdal_dsn_read_geom_ia(f, 0, "", 0, "geometry", c(1, 3)))
  str(bin <- dirigible:::gdal_dsn_read_geom_ia(f, 0, "", 0, "geometry", c(50, 57)))

  ## by FID
  str(bin <- dirigible:::gdal_dsn_read_geom_fa(f, 0, "", 0, "geometry", c(58, 1)))
  str(bin <- dirigible:::gdal_dsn_read_geom_fa(f, 0, "", 0, "geometry", c(1, 58, 58, 1)))

  ## read fields

  dirigible:::gdal_dsn_read_fields_all(f, 0, "", 0, fid_column_name = character())

  dirigible:::gdal_dsn_read_fields_ij(f, 0, "", 0, fid_column_name = character(), 1:2)


  dirigible:::gdal_dsn_read_fields_ia(f, 0, "", 0, fid_column_name = character(), c(1, 34, 57))

  dirigible:::gdal_dsn_read_fields_fa(f, 0, "", 0, fid_column_name = character(), c(58, 1, 0, 5))

})
