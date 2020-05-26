#include <Rcpp.h>
#include "gdalvector/gdalvector.h"
using namespace Rcpp;


// [[Rcpp::export]]
LogicalVector register_gdal() {
  gdalvector::gdal_register();
  LogicalVector out(1);
  out[0] = true;
  return out;
}

// [[Rcpp::export]]
CharacterVector driver_gdal(CharacterVector dsn) {
  return gdalvector::gdal_driver(dsn);
}
