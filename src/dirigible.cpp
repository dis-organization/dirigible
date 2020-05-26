#include <Rcpp.h>
#include "gdalheaders/gdalheaders.h"
using namespace Rcpp;


// [[Rcpp::export]]
LogicalVector register_gdal() {
  gdalheaders::gdal_register_all();
  gdalheaders::ogr_register_all();

  LogicalVector out(1);
  out[0] = true;
  return out;
}

// [[Rcpp::export]]
LogicalVector cleanup_gdal() {
  gdalheaders::ogr_cleanup_all();
  gdalheaders::osr_cleanup();

  LogicalVector out(1);
  out[0] = true;
  return out;
}
// [[Rcpp::export]]
CharacterVector driver_gdal(CharacterVector dsn) {
  return gdalheaders::gdal_driver(dsn);
}
// [[Rcpp::export]]
CharacterVector layer_names_gdal(CharacterVector dsn) {
  return gdalheaders::gdal_layer_names(dsn);
}
