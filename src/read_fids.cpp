#include <Rcpp.h>
#include "gdalgeometry/gdalgeometry.h"
using namespace Rcpp;


// [[Rcpp::export]]
NumericVector gdal_read_fids_all(CharacterVector dsn, IntegerVector layer,
                                 CharacterVector sql, NumericVector ex) {

  return gdalgeometry::dsn_read_fids_all(dsn, layer, sql, ex);
}
// [[Rcpp::export]]
NumericVector gdal_read_fids_ij(CharacterVector dsn, IntegerVector layer,
                                CharacterVector sql, NumericVector ex, NumericVector ij) {
  return gdalgeometry::dsn_read_fids_ij(dsn, layer, sql, ex, ij);
}
// [[Rcpp::export]]
NumericVector gdal_read_fids_ia(CharacterVector dsn, IntegerVector layer,
                                    CharacterVector sql, NumericVector ex, NumericVector ia) {

  return gdalgeometry::dsn_read_fids_ia(dsn, layer, sql, ex, ia);
}


// [[Rcpp::export]]
List gdal_dsn_read_geom_all(CharacterVector dsn, IntegerVector layer,
                            CharacterVector sql, NumericVector ex, CharacterVector format) {

  return gdalgeometry::dsn_read_geom_all(dsn, layer, sql, ex, format);
}

// [[Rcpp::export]]
List gdal_dsn_read_geom_ij(CharacterVector dsn, IntegerVector layer,
                           CharacterVector sql, NumericVector ex,
                           CharacterVector format, NumericVector ij) {
   return gdalgeometry::dsn_read_geom_ij(dsn, layer, sql, ex, format, ij);
}



// [[Rcpp::export]]
List gdal_dsn_read_geom_ia(CharacterVector dsn, IntegerVector layer,
                           CharacterVector sql, NumericVector ex,
                           CharacterVector format, NumericVector ia) {
  return gdalgeometry::dsn_read_geom_ia(dsn, layer, sql, ex, format, ia);
}

// [[Rcpp::export]]
List gdal_dsn_read_geom_fa(CharacterVector dsn, IntegerVector layer,
                           CharacterVector sql, NumericVector ex,
                           CharacterVector format, NumericVector fa) {
  return gdalgeometry::dsn_read_geom_fa(dsn, layer, sql, ex, format, fa);
}
