## 2020-05-08 GDAL up/down stuff cribbed from sf
.dirigible_cache <- new.env(FALSE, parent=globalenv())


.onLoad = function(libname, pkgname) {
  dirigible_load_gdal()
}

.onUnload = function(libname, pkgname) {
  dirigible_unload_gdal()
}

.onAttach = function(libname, pkgname) {

}

dirigible_load_gdal <- function() {
  ## only on windows because tools/winlibs.R
  if (file.exists(system.file("proj/nad.lst", package = "dirigible")[1L])) {
    #prj = system.file("proj", package = "sf")[1]
    #if (! CPL_set_data_dir(prj)) { # if TRUE, uses C API to set path, leaving PROJ_LIB alone
    #  assign(".dirigible.PROJ_LIB", Sys.getenv("PROJ_LIB"), envir=.dirigible_cache)
    #  Sys.setenv("PROJ_LIB" = prj)
    #}
    #CPL_use_proj4_init_rules(1L)
    assign(".dirigible.GDAL_DATA", Sys.getenv("GDAL_DATA"), envir=.dirigible_cache)
    gdl = system.file("gdal", package = "dirigible")[1]
    Sys.setenv("GDAL_DATA" = gdl)
  }
  register_gdal_cpp();
}
# todo
dirigible_unload_gdal <- function() {
  cleanup_gdal_cpp();
  if (file.exists(system.file("proj/nad.lst", package = "dirigible")[1L])) {
    #if (! CPL_set_data_dir(system.file("proj", package = "sf")[1])) # set back:
    #  Sys.setenv("PROJ_LIB"=get(".sf.PROJ_LIB", envir=.sf_cache))
    #
    Sys.setenv("GDAL_DATA"=get(".dirigible.GDAL_DATA", envir=.dirigible_cache))
  }
}
