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
// [[Rcpp::export]]
DoubleVector feature_count_gdal(CharacterVector dsn,  // double, could be a lot of features
                     IntegerVector layer,
                     LogicalVector iterate) {
 return gdalheaders::gdal_feature_count(dsn, layer, iterate);
}

// [[Rcpp::export]]
List read_fields_gdal(Rcpp::CharacterVector dsource,
                      Rcpp::IntegerVector layer,
                      Rcpp::CharacterVector sql,
                      Rcpp::IntegerVector limit_n,
                      Rcpp::IntegerVector skip_n,
                      Rcpp::NumericVector ex,
                      Rcpp::CharacterVector fid_column_name) {
  return gdalheaders::gdal_read_fields(dsource, layer, sql, limit_n, skip_n, ex, fid_column_name);
}
