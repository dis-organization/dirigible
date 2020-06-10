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



// low level geometry converters from feature ----------------------------------------------
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

// --------------------------------------------------------------------------------------

/// READ FIDS ----------------------------------------------------------------------------
inline NumericVector layer_read_fids_all(OGRLayer *poLayer) {
  double   nFeature = gdalheaders::force_layer_feature_count(poLayer);

  NumericVector out(nFeature);
  std::fill( out.begin(), out.end(), NumericVector::get_na() );
  OGRFeature *poFeature;
  double ii = 0;
  while( (poFeature = poLayer->GetNextFeature()) != NULL ) {
   out[ii] = poFeature->GetFID();
   OGRFeature::DestroyFeature(poFeature);
   ii++;
  }
  return out;
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
 // clean up if SQL was used https://www.gdal.org/classGDALDataset.html#ab2c2b105b8f76a279e6a53b9b4a182e0
 if (sql[0] != "") {
   poDS->ReleaseResultSet(poLayer);
 }
 GDALClose(poDS);
 return out;
}

inline NumericVector layer_read_fids_ij(OGRLayer *poLayer, NumericVector ij) {
  NumericVector out(ij[1] - ij[0] + 1);
  std::fill( out.begin(), out.end(), NumericVector::get_na() );
  OGRFeature *poFeature;
  double cnt = 0;
  double ii = 0;
  while( (poFeature = poLayer->GetNextFeature()) != NULL ) {
    if (ii == ij[0] || (ii > ij[0] && ii <= ij[1])) {
      out[cnt] = poFeature->GetFID();
      cnt++;
    }
    ii++;
    OGRFeature::DestroyFeature(poFeature);
  }
  if (cnt < out.length()) {
    Rcpp::warning("not as many FIDs as requested");
  }


  return out;
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
  // clean up if SQL was used https://www.gdal.org/classGDALDataset.html#ab2c2b105b8f76a279e6a53b9b4a182e0
  if (sql[0] != "") {
    poDS->ReleaseResultSet(poLayer);
  }
  GDALClose(poDS);
  return out;
}


inline NumericVector layer_read_fids_ia(OGRLayer *poLayer, NumericVector ia) {

  NumericVector out(ia.length());
  std::fill( out.begin(), out.end(), NumericVector::get_na() );
  OGRFeature *poFeature;
  double ii = 0;
  double cnt = 0;
  while( (poFeature = poLayer->GetNextFeature()) != NULL ) {
    if (ii == ia[cnt]) {
      out[cnt] = poFeature->GetFID();
      cnt++;
    }
    ii++;
    OGRFeature::DestroyFeature(poFeature);
  }
  if (cnt < out.length()) {
    Rcpp::warning("not all FIDS found");
  }
  return out;
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
  // clean up if SQL was used https://www.gdal.org/classGDALDataset.html#ab2c2b105b8f76a279e6a53b9b4a182e0
  if (sql[0] != "") {
    poDS->ReleaseResultSet(poLayer);
  }
  GDALClose(poDS);
  return out;
}

// -----------------------------------------------------------------------------




/// READ GEOMS ----------------------------------------------------------------------------
inline List layer_read_geom_all(OGRLayer *poLayer, CharacterVector format) {
  OGRFeature *poFeature;
  poLayer->ResetReading();
  int nFeature = gdalheaders::force_layer_feature_count(poLayer);

  List out(nFeature);
  double ii = 0;
  while( (poFeature = poLayer->GetNextFeature()) != NULL ) {
    // work through format
    // FIXME: get rid of "geometry"
    if (format[0] == "wkb" | format[0] == "geometry") {
      out[ii] = gdal_geometry_raw(poFeature);
    }
    if (format[0] == "wkt") {
      out[ii] = gdal_geometry_wkt(poFeature);
    }
    // FIXME: maybe call it envelope not extent
    if (format[0] == "extent") {
      out[ii] = gdal_geometry_extent(poFeature);
    }
    // these are all just text variants (wkt uses a different mech)
    if (format[0] == "gml" | format[0] == "json" | format[0] == "kml") {
      out[ii] = gdal_geometry_txt(poFeature, format);
    }

    OGRFeature::DestroyFeature(poFeature);
    ii++;
  }
  return out;
}

inline List dsn_read_geom_all(CharacterVector dsn, IntegerVector layer,
                                       CharacterVector sql, NumericVector ex,
                                       CharacterVector format) {

  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer *poLayer = gdalheaders::gdal_layer(poDS, layer, sql = sql, ex =  ex);
  List out = layer_read_geom_all(poLayer, format);
  // clean up if SQL was used https://www.gdal.org/classGDALDataset.html#ab2c2b105b8f76a279e6a53b9b4a182e0
  if (sql[0] != "") {
    poDS->ReleaseResultSet(poLayer);
  }
  GDALClose(poDS);
  return out;
}





inline List layer_read_geom_ij(OGRLayer *poLayer, CharacterVector format, NumericVector ij) {
  OGRFeature *poFeature;

  poLayer->ResetReading();
  int nFeature = ij[1] - ij[0] + 1;
  List out(nFeature);
  double ii = 0;
  double cnt = 0;
  while( (poFeature = poLayer->GetNextFeature()) != NULL ) {
    if (ii == ij[0] || (ii > ij[0] && ii <= ij[1])) {

      // work through format
      // FIXME: get rid of "geometry"
      if (format[0] == "wkb" | format[0] == "geometry") {
        out[cnt] = gdal_geometry_raw(poFeature);
      }
      if (format[0] == "wkt") {
        out[cnt] = gdal_geometry_wkt(poFeature);
      }
      // FIXME: maybe call it envelope not extent
      if (format[0] == "extent") {
        out[cnt] = gdal_geometry_extent(poFeature);
      }
      // these are all just text variants (wkt uses a different mech)
      if (format[0] == "gml" | format[0] == "json" | format[0] == "kml") {
        out[cnt] = gdal_geometry_txt(poFeature, format);
      }
      cnt++;

    }
    ii++;
    OGRFeature::DestroyFeature(poFeature);
  }
  return out;
}

inline List dsn_read_geom_ij(CharacterVector dsn, IntegerVector layer,
                              CharacterVector sql, NumericVector ex,
                              CharacterVector format, NumericVector ij) {

  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer *poLayer = gdalheaders::gdal_layer(poDS, layer, sql = sql, ex =  ex);
  List out = layer_read_geom_ij(poLayer, format, ij);
  // clean up if SQL was used https://www.gdal.org/classGDALDataset.html#ab2c2b105b8f76a279e6a53b9b4a182e0
  if (sql[0] != "") {
    poDS->ReleaseResultSet(poLayer);
  }
  GDALClose(poDS);
  return out;
}




inline List layer_read_geom_ia(OGRLayer *poLayer, CharacterVector format, NumericVector ia) {
  OGRFeature *poFeature;

  poLayer->ResetReading();

  List out(ia.length());
  double ii = 0;
  double cnt = 0;

  while( (poFeature = poLayer->GetNextFeature()) != NULL ) {
    if (ii == ia[cnt]) {

      // work through format
      // FIXME: get rid of "geometry"
      if (format[0] == "wkb" | format[0] == "geometry") {
        out[cnt] = gdal_geometry_raw(poFeature);
      }
      if (format[0] == "wkt") {
        out[cnt] = gdal_geometry_wkt(poFeature);
      }
      // FIXME: maybe call it envelope not extent
      if (format[0] == "extent") {
        out[cnt] = gdal_geometry_extent(poFeature);
      }
      // these are all just text variants (wkt uses a different mech)
      if (format[0] == "gml" | format[0] == "json" | format[0] == "kml") {
        out[cnt] = gdal_geometry_txt(poFeature, format);
      }
      cnt++;

    }
    ii++;
    OGRFeature::DestroyFeature(poFeature);
  }
  return out;
}

inline List dsn_read_geom_ia(CharacterVector dsn, IntegerVector layer,
                             CharacterVector sql, NumericVector ex,
                             CharacterVector format, NumericVector ia) {

  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer *poLayer = gdalheaders::gdal_layer(poDS, layer, sql = sql, ex =  ex);
  List out = layer_read_geom_ia(poLayer, format, ia);
  // clean up if SQL was used https://www.gdal.org/classGDALDataset.html#ab2c2b105b8f76a279e6a53b9b4a182e0
  if (sql[0] != "") {
    poDS->ReleaseResultSet(poLayer);
  }
  GDALClose(poDS);
  return out;
}



inline List layer_read_geom_fa(OGRLayer *poLayer, CharacterVector format, NumericVector fa) {
  OGRFeature *poFeature;
  //poLayer->ResetReading();

  List out(fa.length());

  for (double ii = 0; ii < fa.length(); ii++) {
    GIntBig feature_id = (GIntBig)fa[ii];
      poFeature = poLayer->GetFeature(feature_id);
      // work through format
      // FIXME: get rid of "geometry"
      if (format[0] == "wkb" | format[0] == "geometry") {
        out[ii] = gdal_geometry_raw(poFeature);
      }
      if (format[0] == "wkt") {
        out[ii] = gdal_geometry_wkt(poFeature);
      }
      // FIXME: maybe call it envelope not extent
      if (format[0] == "extent") {
        out[ii] = gdal_geometry_extent(poFeature);
      }
      // these are all just text variants (wkt uses a different mech)
      if (format[0] == "gml" | format[0] == "json" | format[0] == "kml") {
        out[ii] = gdal_geometry_txt(poFeature, format);
      }

    OGRFeature::DestroyFeature(poFeature);
  }
  return out;
}

inline List dsn_read_geom_fa(CharacterVector dsn, IntegerVector layer,
                             CharacterVector sql, NumericVector ex,
                             CharacterVector format, NumericVector fa) {

  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer *poLayer = gdalheaders::gdal_layer(poDS, layer, sql = sql, ex =  ex);
  List out = layer_read_geom_fa(poLayer, format, fa);
  // clean up if SQL was used https://www.gdal.org/classGDALDataset.html#ab2c2b105b8f76a279e6a53b9b4a182e0
  if (sql[0] != "") {
    poDS->ReleaseResultSet(poLayer);
  }
  GDALClose(poDS);
  return out;
}

/// --------------------------------------------------------------------------------------


/// READ FIELDS ----------------------------------------------------------------------------
inline List layer_read_fields_all(OGRLayer *poLayer, CharacterVector fid_column_name) {
  double   nFeature = gdalheaders::force_layer_feature_count(poLayer);

  OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
  bool int64_as_string = false;
  List out = gdalheaders::allocate_fields_list(poFDefn, nFeature, int64_as_string, fid_column_name);

  OGRFeature *poFeature;
  double ii = 0;
  int iField;

  while( (poFeature = poLayer->GetNextFeature()) != NULL ) {

    for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
    {
      OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );
      if( poFieldDefn->GetType() == OFTInteger   ) {
        IntegerVector nv;
        nv = out[iField];
        nv[ii] = poFeature->GetFieldAsInteger( iField );
      }

      if( poFieldDefn->GetType() == OFTReal || poFieldDefn->GetType() == OFTInteger64) {
        NumericVector nv;
        nv = out[iField];
        nv[ii] = poFeature->GetFieldAsDouble( iField );
      }

      if( poFieldDefn->GetType() == OFTString || poFieldDefn->GetType() == OFTDate || poFieldDefn->GetType() == OFTTime || poFieldDefn->GetType() == OFTDateTime) {
        CharacterVector nv;
        nv = out[iField];
        nv[ii] = poFeature->GetFieldAsString( iField );

      }
    }

    OGRFeature::DestroyFeature(poFeature);
    ii++;
  }
  return out;
}

inline List dsn_read_fields_all(CharacterVector dsn, IntegerVector layer,
                                       CharacterVector sql, NumericVector ex,
                                       CharacterVector fid_column_name) {

  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer *poLayer = gdalheaders::gdal_layer(poDS, layer, sql = sql, ex =  ex);
  List out = layer_read_fields_all(poLayer, fid_column_name);
  // clean up if SQL was used https://www.gdal.org/classGDALDataset.html#ab2c2b105b8f76a279e6a53b9b4a182e0
  if (sql[0] != "") {
    poDS->ReleaseResultSet(poLayer);
  }
  GDALClose(poDS);
  return out;
}
// -----------------------------------------------------------------------------------------

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
