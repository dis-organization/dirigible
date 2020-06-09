#ifndef GDALGEOMETRY_H
#define GDALGEOMETRY_H
#include <Rcpp.h>
#include "ogrsf_frmts.h"
//#include "ogr_api.h"
#include "gdal_priv.h"
// #include "ogr_spatialref.h" // for OGRSpatialReference
// #include "cpl_conv.h" // for CPLFree()

#include "gdalheaders/gdalheaders.h"
namespace gdalgeometry {
using namespace Rcpp;

inline IntegerVector limit_skip_n_to_start_end_len(IntegerVector skip_n, IntegerVector limit_n, IntegerVector n) {
  int start = 0;
  int end = n[0] - 1;
  if (skip_n[0] > 0) {  // silently ignore negative values
    start = skip_n[0];
  }
  if (limit_n[0] > 0) { // silently ignore negative values
    end = start + limit_n[0] - 1;
  }
  if (start >= n[0]) {
    Rcpp::stop("skip_n skips all available features");
  }
  if (end > n[0]) {
    if (start > 0) {
      Rcpp::warning("limit_n is greater than the number of available features (given 'skip_n')");
    } else {
      Rcpp::warning("limit_n is greater than the number of available features");
    }
    end = n[0] - 1;
  }
  int len = end - start + 1;
  IntegerVector out(3);
  out[0] = start; out[1] = end; out[2] = len;
  return  out;
}


/// READ FIDS ----------------------------------------------------------------------------
inline NumericVector layer_read_fids_all(OGRLayer *poLayer) {
  double   nFeature = gdalheaders::force_layer_feature_count(poLayer);

  NumericVector fids(nFeature);
  std::fill( fids.begin(), fids.end(), NumericVector::get_na() );
  OGRFeature *poFeature;
  double ii = 0;
  while( (poFeature = poLayer->GetNextFeature()) != NULL ) {
   fids[ii] = poFeature->GetFID();
   OGRFeature::DestroyFeature(poFeature);
   ii++;
  }
  return fids;
}

inline NumericVector dsn_read_fids_all(CharacterVector dsn, IntegerVector layer,
                                       CharacterVector sql, NumericVector ex) {

  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer *poLayer = gdalheaders::gdal_layer(poDS, layer, sql = sql, ex =  ex);
 NumericVector out = layer_read_fids_all(poLayer);
 GDALClose(poDS);
 return out;
}

inline NumericVector layer_read_fids_ij(OGRLayer *poLayer, NumericVector ij) {
 // double   nFeature = gdalheaders::force_layer_feature_count(poLayer);

  NumericVector fids(ij[1] - ij[0] + 1);
  std::fill( fids.begin(), fids.end(), NumericVector::get_na() );
  OGRFeature *poFeature;
  double cnt = 0;
  double ii = 0;
  while( (poFeature = poLayer->GetNextFeature()) != NULL ) {
    if (ii == ij[0] || (ii > ij[0] && ii <= ij[1])) {
      fids[cnt] = poFeature->GetFID();
      cnt++;
    }
    ii++;
    OGRFeature::DestroyFeature(poFeature);
  }
  if (cnt < fids.length()) {
    Rcpp::warning("not as many FIDs as requested");
  }


  return fids;
}

inline NumericVector dsn_read_fids_ij(CharacterVector dsn, IntegerVector layer,
                                       CharacterVector sql, NumericVector ex,
                                       NumericVector ij) {

  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer *poLayer = gdalheaders::gdal_layer(poDS, layer, sql = sql, ex =  ex);
  NumericVector out = layer_read_fids_ij(poLayer, ij);
  GDALClose(poDS);
  return out;
}


inline NumericVector layer_read_fids_ia(OGRLayer *poLayer, NumericVector ia) {
  // double   nFeature = gdalheaders::force_layer_feature_count(poLayer);

  NumericVector fids(ia.length());
  std::fill( fids.begin(), fids.end(), NumericVector::get_na() );
  OGRFeature *poFeature;
  double ii = 0;
  double cnt = 0;
  while( (poFeature = poLayer->GetNextFeature()) != NULL ) {
    if (ii == ia[cnt]) {
      fids[cnt] = poFeature->GetFID();
      cnt++;
    }
    ii++;
    OGRFeature::DestroyFeature(poFeature);
  }
  if (cnt < fids.length()) {
    Rcpp::warning("not all FIDS found");
  }
  return fids;
}

inline NumericVector dsn_read_fids_ia(CharacterVector dsn, IntegerVector layer,
                                      CharacterVector sql, NumericVector ex,
                                      NumericVector ia) {

  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer *poLayer = gdalheaders::gdal_layer(poDS, layer, sql = sql, ex =  ex);
  NumericVector out = layer_read_fids_ia(poLayer, ia);
  GDALClose(poDS);
  return out;
}



inline CharacterVector gdal_proj_to_wkt(CharacterVector proj_str) {
  OGRSpatialReference oSRS;
  char *pszWKT = NULL;
  oSRS.importFromProj4(proj_str[0]);
  oSRS.exportToWkt(&pszWKT);
  CharacterVector out =  Rcpp::CharacterVector::create(pszWKT);
  CPLFree(pszWKT);

  return out;
}

//FIXME
// o add cast capability?
// o add ExportToGEOS?
// o boundary, buffer, centroid, convexhull, delaunaytriangulation, polygonize, segmentize,
//   simplify(preservetopology), totriangulatedsurface
// o various To<type> casts,
// o allow control over wkbNDR and wkbVariant
// see here for the constants for the format variants
// http://www.gdal.org/ogr__core_8h.html#a6716bd3399c31e7bc8b0fd94fd7d9ba6a7459e8d11fa69e89271771c8d0f265d8
inline RawVector gdal_geometry_raw(OGRFeature *poFeature) {
  Rcpp::RawVector raw(poFeature->GetGeometryRef()->WkbSize());
  poFeature->GetGeometryRef()->exportToWkb(wkbNDR, &(raw[0]), wkbVariantIso);
  return raw;
}
inline CharacterVector gdal_geometry_wkt(OGRFeature *poFeature) {
  char *pszGEOM_WKT = NULL;
  poFeature->GetGeometryRef()->exportToWkt(&pszGEOM_WKT, wkbVariantIso );
  CharacterVector wkt(1);
  wkt[0] = pszGEOM_WKT;
  CPLFree( pszGEOM_WKT );
  return wkt;
}
inline CharacterVector gdal_geometry_txt(OGRFeature *poFeature, CharacterVector format) {
  char *export_txt = NULL;
  CharacterVector txt(1);
  if (format[0] == "gml") {
    export_txt = poFeature->GetGeometryRef()->exportToGML();
  }
  if (format[0] == "json") {
    export_txt = poFeature->GetGeometryRef()->exportToJson();
  }
  if (format[0] == "kml") {
    export_txt = poFeature->GetGeometryRef()->exportToKML();
  }
  txt[0] = export_txt;
  CPLFree(export_txt);
  return txt;
}
inline NumericVector gdal_geometry_extent(OGRFeature *poFeature) {
  OGREnvelope env;
  OGR_G_GetEnvelope(poFeature->GetGeometryRef(), &env);
  // if geometry is empty, set the envelope to undefined (otherwise all 0s)
  double minx, maxx, miny, maxy;
  if (poFeature->GetGeometryRef()->IsEmpty()) {
    minx = NA_REAL;
    maxx = NA_REAL;
    miny = NA_REAL;
    maxy = NA_REAL;
  } else {
    minx = env.MinX;
    maxx = env.MaxX;
    miny = env.MinY;
    maxy = env.MaxY;
  }
  NumericVector extent = NumericVector::create(minx, maxx, miny, maxy);
 return extent;
}
inline List gdal_geometry_(OGRLayer *poLayer,
                               IntegerVector fid,
                               CharacterVector format)
{

  OGRFeature *poFeature;
  OGRGeometry *poGeometry;
  poLayer->ResetReading();
  int len = fid.length();
  List feature_xx(len);
  int iFeature = 0;
  int lFeature = 0;
  int nFeature = gdalheaders::force_layer_feature_count(poLayer);
  bool was_bad = false;
  for (int iter = 0; iter < fid.length(); iter++) {
         GIntBig feature_id = (GIntBig)fid[iter];
         poFeature = poLayer->GetFeature(feature_id);
         if (!was_bad & (NULL == poFeature)) {
           was_bad = true;
         } else {
           // work through format
           // FIXME: get rid of "geometry"
           if (format[0] == "wkb" | format[0] == "geometry") {
             feature_xx[iter] = gdal_geometry_raw(poFeature);
           }
           if (format[0] == "wkt") {
             feature_xx[iter] = gdal_geometry_wkt(poFeature);
           }
           // FIXME: maybe call it envelope not extent
           if (format[0] == "extent") {
             feature_xx[iter] = gdal_geometry_extent(poFeature);
           }
           // these are all just text variants (wkt uses a different mech)
           if (format[0] == "gml" | format[0] == "json" | format[0] == "kml") {
             feature_xx[iter] = gdal_geometry_txt(poFeature, format);
           }

          OGRFeature::DestroyFeature(poFeature);
         }


   }
   if (was_bad) {
     Rprintf("(at least one) fid value not found, or Feature invalid: some elements NULL\n");

   }
   return feature_xx;
  }




}

#endif
