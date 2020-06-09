// #include <Rcpp.h>
// #include "gdalheaders/gdalheaders.h"
// using namespace Rcpp;
//
// // this is for Grant Williamson for us to explore
// // [[Rcpp::export]]
// List raster_io_gdal_io_array(CharacterVector sources,
//                         IntegerVector window,
//                         IntegerVector band,
//                         CharacterVector resample) {
//   // sources is a set of files, assumed to all be the same and only one sds etc., or actual sds strings
//
//   int len = sources.length();
//   List out(len);
//   CharacterVector dsn(1);
//   for (int i = 0; i < len; i++) {
//     dsn[0] = sources[i];
//     out[i] = gdalheaders::gdal_raster_io(dsn, window, band, resample)[0];
//   }
//   return out;
// }
