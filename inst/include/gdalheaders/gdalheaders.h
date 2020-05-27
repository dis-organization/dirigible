#ifndef GDALHEADERS_H
#define GDALHEADERS_H
#include <Rcpp.h>
#include "ogrsf_frmts.h"
//#include "ogr_api.h"
#include "gdal_priv.h"
#include "CollectorList.h"
// #include "ogr_spatialref.h" // for OGRSpatialReference
// #include "cpl_conv.h" // for CPLFree()

namespace gdalheaders {
using namespace Rcpp;

constexpr int MAX_INT =  std::numeric_limits<int>::max ();

inline void gdal_register_all() {
  GDALAllRegister();
}
inline void ogr_register_all() {
  OGRRegisterAll();
}
inline void ogr_cleanup_all() {
  OGRCleanupAll();
}
inline void osr_cleanup() {
  OSRCleanup();
}


inline CharacterVector gdal_version()
{
  Rcpp::CharacterVector out(1);

  out[0] = GDALVersionInfo("--version");
  return out;
}


inline List gdal_list_drivers()
{
  GDALAllRegister();

  int n = GetGDALDriverManager()->GetDriverCount();

  Rcpp::CharacterVector sname(n);
  Rcpp::CharacterVector lname(n);
  Rcpp::LogicalVector isvector(n);
  Rcpp::LogicalVector israster(n);
  Rcpp::LogicalVector iscopy(n);
  Rcpp::LogicalVector iscreate(n);
  Rcpp::LogicalVector isvirt(n);
  for (int idriver = 0; idriver < n; idriver++) {
    GDALDriver *dr = GetGDALDriverManager()->GetDriver(idriver);
    sname(idriver) = GDALGetDriverShortName(dr);
    lname(idriver) = GDALGetDriverLongName(dr);
    isvector(idriver) = (dr->GetMetadataItem(GDAL_DCAP_VECTOR) != NULL);
    israster(idriver) = (dr->GetMetadataItem(GDAL_DCAP_RASTER) != NULL);
    iscopy(idriver) = (dr->GetMetadataItem(GDAL_DCAP_CREATECOPY) != NULL);
    iscreate(idriver) = (dr->GetMetadataItem(GDAL_DCAP_CREATE) != NULL);
    isvirt(idriver) = (dr->GetMetadataItem(GDAL_DCAP_VIRTUALIO) != NULL);


  }
  Rcpp::List out = Rcpp::List::create(Rcpp::Named("driver") = sname,
                                      Rcpp::Named("name") = lname,
                                      Rcpp::Named("vector") = isvector,
                                      Rcpp::Named("raster") = israster,
                                      Rcpp::Named("create") = iscreate,
                                      Rcpp::Named("copy") = iscopy,
                                      Rcpp::Named("virtual") = isvirt);
  return out;
}

// allocate_fields_list: stolen from allocate_out_list
// allocate_out_list: this is the non-geometry part of sf allocate_out_list by Edzer Pebsema
// https://github.com/r-spatial/sf/blob/cc7fba3c5a34ec1c545a4ab82369f33f47c3745f/src/gdal_read.cpp#L12-L65
// originally used in vapour at allocate_attributes
// to update this copy the source of allocate_fields_list from sf, delete the geom stuff at the end
// (between  names[i] = poFieldDefn->GetNameRef();
// and out.attr("names") = names;
// and remove the GetGeomFieldCount from the sum of n fields
inline Rcpp::List allocate_fields_list(OGRFeatureDefn *poFDefn, int n_features, bool int64_as_string,
                                       Rcpp::CharacterVector fid_column) {

  if (fid_column.size() > 1)
    Rcpp::stop("FID column name should be a length 1 character vector"); // #nocov

  // modified MDS
  //int n = poFDefn->GetFieldCount() + poFDefn->GetGeomFieldCount() + fid_column.size();
  int n = poFDefn->GetFieldCount() + fid_column.size();

  Rcpp::List out(n);
  Rcpp::CharacterVector names(n);
  for (int i = 0; i < poFDefn->GetFieldCount(); i++) {
    OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn(i);
    switch (poFieldDefn->GetType()) {
    case OFTInteger: {
      if (poFieldDefn->GetSubType() == OFSTBoolean)
        out[i] = Rcpp::LogicalVector(n_features);
      else
        out[i] = Rcpp::IntegerVector(n_features);
    }
      break;
    case OFTDate: {
      Rcpp::NumericVector ret(n_features);
      ret.attr("class") = "Date";
      out[i] = ret;
    } break;
    case OFTDateTime: {
      Rcpp::NumericVector ret(n_features);
      Rcpp::CharacterVector cls(2);
      cls(0) = "POSIXct";
      cls(1) = "POSIXt";
      ret.attr("class") = cls;
      out[i] = ret;
    } break;
    case OFTInteger64: // fall through: converts Int64 -> double
      if (int64_as_string)
        out[i] = Rcpp::CharacterVector(n_features);
      else
        out[i] = Rcpp::NumericVector(n_features);
      break;
    case OFTReal:
      out[i] = Rcpp::NumericVector(n_features);
      break;
    case OFTStringList:
    case OFTRealList:
    case OFTIntegerList:
    case OFTInteger64List:
      out[i] = Rcpp::List(n_features);
      break;
    case OFTString:
    default:
      out[i] = Rcpp::CharacterVector(n_features);
      break;
    }
    names[i] = poFieldDefn->GetNameRef();
  }


  out.attr("names") = names;
  return out;
}



inline List gdal_read_fields(CharacterVector dsn,
                      IntegerVector layer,
                      CharacterVector sql,
                      IntegerVector limit_n,
                      IntegerVector skip_n,
                      NumericVector ex,
                      CharacterVector fid_column_name)
{
  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer  *poLayer;
  OGRPolygon poly;
  OGRLinearRing ring;

  if (ex.length() == 4) {
    ring.addPoint(ex[0], ex[2]); //xmin, ymin
    ring.addPoint(ex[0], ex[3]); //xmin, ymax
    ring.addPoint(ex[1], ex[3]); //xmax, ymax
    ring.addPoint(ex[1], ex[2]); //xmax, ymin
    ring.closeRings();
    poly.addRing(&ring);
  }

  if (sql[0] != "") {
    if (ex.length() == 4) {
      poLayer =  poDS->ExecuteSQL(sql[0],
                                  &poly,
                                  NULL );
    } else {
      poLayer =  poDS->ExecuteSQL(sql[0],
                                  NULL,
                                  NULL );
    }

    if (poLayer == NULL) {
      Rcpp::stop("SQL execution failed.\n");
    }

  } else {
    poLayer =  poDS->GetLayer(layer[0]);
  }
  if (poLayer == NULL) {
    Rcpp::stop("Layer open failed.\n");
  }

  OGRFeature *poFeature;
  poLayer->ResetReading();
  double nFeature = (double)poLayer->GetFeatureCount();
  nFeature = -1;
  if (nFeature == -1) {
    nFeature = 0;
    // we have to find out first because this driver doesn't support GetFeatureCount
    // https://trac.osgeo.org/gdal/wiki/rfc66_randomlayerreadwrite
    // FIXME: should use gdal_feature_count()
    while( (poFeature = poLayer->GetNextFeature()) != NULL )
    {
      nFeature++;
      OGRFeature::DestroyFeature( poFeature );
    }

    poLayer->ResetReading();

  }

  if (nFeature > MAX_INT)
    Rcpp::stop("Number of features exceeds maximal number able to be read");

  // this is poorly laid out but works, check twice to avoid
  // over allocating as per #60
  if (limit_n[0] > 0) {
    if (limit_n[0] < nFeature) {
      nFeature = nFeature - skip_n[0];
      if (limit_n[0] < nFeature) {
        nFeature = limit_n[0];
      }
    }
  }

  if (nFeature < 1) {
    if (skip_n[0] > 0) {
      Rcpp::stop("no features to be read (is 'skip_n' set too high?");
    }
    Rcpp::stop("no features to be read");
  }

  OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
  bool int64_as_string = false;
  List out = allocate_fields_list(poFDefn, nFeature, int64_as_string, fid_column_name);
  int iFeature = 0;  // always increment iFeature, it is position through the loop
  int lFeature = 0; // keep a count of the features we actually send out
  while((poFeature = poLayer->GetNextFeature()) != NULL)
  {

    if (lFeature >= nFeature) {
      break;
    }
    OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
    // only increment lFeature if we actually keep this one
    if (iFeature >= skip_n[0]) {  // we are at skip_n

      int iField;
      for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
      {
        OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );
        if( poFieldDefn->GetType() == OFTInteger   ) {
          IntegerVector nv;
          nv = out[iField];
          nv[lFeature] = poFeature->GetFieldAsInteger( iField );
        }

        if( poFieldDefn->GetType() == OFTReal || poFieldDefn->GetType() == OFTInteger64) {
          NumericVector nv;
          nv = out[iField];
          nv[lFeature] = poFeature->GetFieldAsDouble( iField );
        }

        if( poFieldDefn->GetType() == OFTString || poFieldDefn->GetType() == OFTDate || poFieldDefn->GetType() == OFTTime || poFieldDefn->GetType() == OFTDateTime) {
          CharacterVector nv;
          nv = out[iField];
          nv[lFeature] = poFeature->GetFieldAsString( iField );

        }
      }
      // so we start counting
      lFeature = lFeature + 1;
    }
    // always increment iFeature, it's position through the loop
    iFeature = iFeature + 1;
    OGRFeature::DestroyFeature( poFeature );
  }
  // clean up if SQL was used https://www.gdal.org/classGDALDataset.html#ab2c2b105b8f76a279e6a53b9b4a182e0
  if (sql[0] != "") {
    poDS->ReleaseResultSet(poLayer);
  }
  GDALClose( poDS );
  if (lFeature < 1) {
    if (skip_n[0] > 0) {
      Rcpp::stop("no features to be read (is 'skip_n' set too high?");
    }
    Rcpp::stop("no features to be read");
  }
  return(out);
}


inline DoubleVector gdal_feature_count(CharacterVector dsn,
                                       IntegerVector layer,
                                       LogicalVector iterate) {
  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer  *aLayer;
  int nlayer = poDS->GetLayerCount();
  if (layer[0] >= nlayer) {
    Rprintf("layer count: %i\n", nlayer);
    Rprintf("layer index: %i\n", layer[0]);
    Rcpp::stop("layer index exceeds layer count");
  }
  aLayer =  poDS->GetLayer(layer[0]);
  OGRFeature *aFeature;
  aLayer->ResetReading();
  double nFeature = (double)aLayer->GetFeatureCount();
  if (iterate) {
    nFeature = 0;
    while( (aFeature = aLayer->GetNextFeature()) != NULL )
    {
      nFeature++;
      OGRFeature::DestroyFeature(aFeature);
    }

  }
  GDALClose( poDS );

  DoubleVector out(1);
  out[0] = nFeature;
  return(out);
}

inline CharacterVector gdal_driver(CharacterVector dsn)
{

  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  CharacterVector dname(1);
  dname[0] = poDS->GetDriverName();
  GDALClose(poDS);
  return(dname);
} // gdal_driver

inline CharacterVector gdal_layer_names(CharacterVector dsn,
                                              CharacterVector sql = "")
{
  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer  *poLayer;
  if (sql[0] != "") {
    poLayer =  poDS->ExecuteSQL(sql[0],
                                NULL,
                                NULL);
    if (poLayer == NULL) {
      Rcpp::stop("SQL execution failed.\n");
    }
    // clean up if SQL was used https://www.gdal.org/classGDALDataset.html#ab2c2b105b8f76a279e6a53b9b4a182e0
    poDS->ReleaseResultSet(poLayer);

  }
  int nlayer = poDS->GetLayerCount();
  CharacterVector lnames = CharacterVector(nlayer);
  for (int ilayer = 0; ilayer < nlayer; ilayer++) {
    poLayer = poDS->GetLayer(ilayer);
    lnames[ilayer] = poLayer->GetName();
  }
  GDALClose(poDS);
  return(lnames);
} // gdal_layer_names




inline List gdal_read_geometry(CharacterVector dsn,
                                     IntegerVector layer,
                                     CharacterVector sql,
                                     CharacterVector what,
                                     CharacterVector textformat,
                                     IntegerVector limit_n,
                                     IntegerVector skip_n,
                                     NumericVector ex)
{
  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer  *poLayer;


  OGRPolygon poly;
  OGRLinearRing ring;

  if (ex.length() == 4) {

    ring.addPoint(ex[0], ex[2]); //xmin, ymin
    ring.addPoint(ex[0], ex[3]); //xmin, ymax
    ring.addPoint(ex[1], ex[3]); //xmax, ymax
    ring.addPoint(ex[1], ex[2]); //xmax, ymin

    ring.closeRings();
    poly.addRing(&ring);
  }

  if (sql[0] != "") {
    if (ex.length() == 4) {
      poLayer =  poDS->ExecuteSQL(sql[0],
                                  &poly,
                                  NULL );
    } else {
      poLayer =  poDS->ExecuteSQL(sql[0],
                                  NULL,
                                  NULL );
    }

    if (poLayer == NULL) {
      Rcpp::stop("SQL execution failed.\n");
    }

  } else {
    poLayer =  poDS->GetLayer(layer[0]);
  }
  if (poLayer == NULL) {
    Rcpp::stop("Layer open failed.\n");
  }
  OGRFeature *poFeature;
  poLayer->ResetReading();

  //OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
  CollectorList feature_xx;
  int iFeature = 0;
  int lFeature = 0;
  int nFeature = (int)poLayer->GetFeatureCount();
  if (nFeature > MAX_INT)
    Rcpp::stop("Number of features exceeds maximal number able to be read");
  if (nFeature == -1) {
    nFeature = 0;
    // we have to find out first because this driver doesn't support GetFeatureCount
    // https://trac.osgeo.org/gdal/wiki/rfc66_randomlayerreadwrite
    while( (poFeature = poLayer->GetNextFeature()) != NULL )
    {
      nFeature++;
      OGRFeature::DestroyFeature( poFeature );
    }
    poLayer->ResetReading();

  }


  if (limit_n[0] > 0) {
    if (limit_n[0] < nFeature) {
      nFeature = limit_n[0];
    }
  }
  if (nFeature < 1) {
    Rcpp::stop("no features to be read");
  }


  int warncount = 0;
  while( (poFeature = poLayer->GetNextFeature()) != NULL )
  {

    if (iFeature >= skip_n[0]) {  // we are at skip_n

      OGRGeometry *poGeometry;
      poGeometry = poFeature->GetGeometryRef();
      if (poGeometry == NULL) {
        warncount++;
        feature_xx.push_back(R_NilValue);
        //if (warncount == 1) Rcpp::warning("at least one geometry is NULL, perhaps the 'sql' argument excludes the native geometry?\n(use 'SELECT * FROM ..') ");
      } else {
        // GEOMETRY
        // geometry native binary
        // text     various text forms
        // extent   simple bbox
        if (what[0] == "geometry") {
          //https://github.com/r-spatial/sf/blob/798068d3044a65797c52bf3b42bc4a5d83b45e9a/src/gdal.cpp#L207
          Rcpp::RawVector raw(poGeometry->WkbSize());

          //todo we probably need better err handling see sf handle_error
          poGeometry->exportToWkb(wkbNDR, &(raw[0]), wkbVariantIso);
          feature_xx.push_back(raw);

        }
        if (what[0] == "text") {
          CharacterVector txt(1);
          if (textformat[0] == "json") {
            char *export_txt = NULL;
            export_txt = poGeometry->exportToJson();
            txt[0] = export_txt;
            CPLFree(export_txt);
          }
          if (textformat[0] == "gml") {
            char *export_txt = NULL;

            export_txt = poGeometry->exportToGML();
            txt[0] = export_txt;
            CPLFree(export_txt);

          }
          if (textformat[0] == "kml") {
            char *export_txt = NULL;

            export_txt = poGeometry->exportToKML();
            txt[0] = export_txt;
            CPLFree(export_txt);

          }
          if (textformat[0] == "wkt") {
            //     // see buffer handling for SRS here which is where
            //     // I got inspiration from : http://www.gdal.org/gdal_tutorial.html
            char *pszGEOM_WKT = NULL;
            //     // see here for the constants for the format variants
            //     // http://www.gdal.org/ogr__core_8h.html#a6716bd3399c31e7bc8b0fd94fd7d9ba6a7459e8d11fa69e89271771c8d0f265d8
            poGeometry->exportToWkt(&pszGEOM_WKT, wkbVariantIso );
            txt[0] = pszGEOM_WKT;
            CPLFree( pszGEOM_WKT );

          }
          feature_xx.push_back(txt);


        }
        if (what[0] == "extent") {
          OGREnvelope env;
          OGR_G_GetEnvelope(poGeometry, &env);
          // if geometry is empty, set the envelope to undefined (otherwise all 0s)
          double minx, maxx, miny, maxy;
          if (poGeometry->IsEmpty()) {

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
          feature_xx.push_back(extent);


        }
        // FIXME: do with wk, perhaps post-hoc from WKB return from here
        // if (what[0] == "point") {
        //   Rcpp::List pts = Rcpp::List(1);
        //   pts = GetPointsInternal(poGeometry, 1);
        //   feature_xx.push_back(pts);
        // }
        if (what[0] == "type") {
          OGRwkbGeometryType gtyp = OGR_G_GetGeometryType(poGeometry);
          IntegerVector r_gtyp = IntegerVector(1);
          r_gtyp[0] = (int)gtyp;
          feature_xx.push_back(r_gtyp);
        }
      }
      OGRFeature::DestroyFeature( poFeature );
      lFeature = lFeature + 1;
    }  //    if (iFeature >= skip_n[0]) {  // we are at skip_n

    iFeature = iFeature + 1;
    if (limit_n[0] > 0 && lFeature >= limit_n[0]) {
      break;  // short-circuit for limit_n
    }

  }

  // clean up if SQL was used https://www.gdal.org/classGDALDataset.html#ab2c2b105b8f76a279e6a53b9b4a182e0
  if (sql[0] != "") {
    poDS->ReleaseResultSet(poLayer);
  }
  GDALClose( poDS );

  if (lFeature < 1) {
    if (skip_n[0] > 0) {
      Rcpp::stop("no features to be read (is 'skip_n' set too high?");
    }

    Rcpp::stop("no features to be read");
  }

  return(feature_xx.vector());
}



inline List gdal_read_names(CharacterVector dsn,
                           IntegerVector layer,
                           CharacterVector sql,
                           IntegerVector limit_n,
                           IntegerVector skip_n,
                           NumericVector ex)
{

  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer  *poLayer;


  OGRPolygon poly;
  OGRLinearRing ring;

  if (ex.length() == 4) {

    ring.addPoint(ex[0], ex[2]); //xmin, ymin
    ring.addPoint(ex[0], ex[3]); //xmin, ymax
    ring.addPoint(ex[1], ex[3]); //xmax, ymax
    ring.addPoint(ex[1], ex[2]); //xmax, ymin

    ring.closeRings();
    poly.addRing(&ring);
  }

  if (sql[0] != "") {
    if (ex.length() == 4) {
      poLayer =  poDS->ExecuteSQL(sql[0],
                                  &poly,
                                  NULL );
    } else {
      poLayer =  poDS->ExecuteSQL(sql[0],
                                  NULL,
                                  NULL );
    }

    if (poLayer == NULL) {
      Rcpp::stop("SQL execution failed.\n");
    }

  } else {
    poLayer =  poDS->GetLayer(layer[0]);
  }
  if (poLayer == NULL) {
    Rcpp::stop("Layer open failed.\n");
  }
  OGRFeature *poFeature;
  poLayer->ResetReading();

  //OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
  CollectorList feature_xx;
  int iFeature = 0;
  int lFeature = 0;
  int nFeature = (int)poLayer->GetFeatureCount();
  if (nFeature > MAX_INT)
    Rcpp::stop("Number of features exceeds maximal number able to be read");

  if (nFeature == -1) {
    nFeature = 0;
    // we have to find out first because this driver doesn't support GetFeatureCount
    // https://trac.osgeo.org/gdal/wiki/rfc66_randomlayerreadwrite
    while( (poFeature = poLayer->GetNextFeature()) != NULL )
    {
      nFeature++;
      OGRFeature::DestroyFeature( poFeature );
    }
    poLayer->ResetReading();

  }
  if (limit_n[0] > 0) {
    if (limit_n[0] < nFeature) {
      nFeature = limit_n[0];
    }
  }

  if (nFeature < 1) {
    if (skip_n[0] > 0) {
      Rcpp::stop("no features to be read (is 'skip_n' set too high?");
    }

    Rcpp::stop("no features to be read");
  }

  double aFID;
  Rcpp::NumericVector rFID(1);
  while( (poFeature = poLayer->GetNextFeature()) != NULL )
  {

    if (iFeature >= skip_n[0]) {
      aFID = (double) poFeature->GetFID();
      OGRFeature::DestroyFeature( poFeature );
      rFID[0] = aFID;
      feature_xx.push_back(Rcpp::clone(rFID));
      lFeature++;
    }
    iFeature++;
    if (limit_n[0] > 0 && lFeature >= limit_n[0]) {
      break;  // short-circuit for limit_n
    }
  }
  // clean up if SQL was used https://www.gdal.org/classGDALDataset.html#ab2c2b105b8f76a279e6a53b9b4a182e0
  if (sql[0] != "") {
    poDS->ReleaseResultSet(poLayer);
  }
  GDALClose( poDS );

  if (lFeature < 1) {
    if (skip_n[0] > 0) {
      Rcpp::stop("no features to be read (is 'skip_n' set too high?");
    }

    Rcpp::stop("no features to be read");
  }
  return(feature_xx.vector());
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

inline List gdal_projection_info(CharacterVector dsn,
                                IntegerVector layer,
                                CharacterVector sql)
{
  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer  *poLayer;
  if (sql[0] != "") {
    poLayer =  poDS->ExecuteSQL(sql[0],
                                NULL,
                                NULL );

    if (poLayer == NULL) {
      Rcpp::stop("SQL execution failed.\n");
    }

  } else {
    poLayer =  poDS->GetLayer(layer[0]);
  }
  if (poLayer == NULL) {
    Rcpp::stop("Layer open failed.\n");
  }
  OGRSpatialReference *SRS =  poLayer->GetSpatialRef();

  char *proj;  // this gets cleaned up lower in the SRS==NULL else
  List info_out(6);
  CharacterVector outproj(1);
  CharacterVector outnames(6);
  outnames[0] = "Proj4";
  outnames[1] = "MICoordSys";
  outnames[2] = "PrettyWkt";
  outnames[3] = "Wkt";
  outnames[4] = "EPSG";
  outnames[5] = "XML";
  info_out.attr("names") = outnames;

  if (SRS == NULL) {
    //Rcpp::warning("null");
    // do nothing, or warn
    // e.g. .shp with no .prj
  } else {
    Rcpp::warning("not null");
    // SRS is not NULL, so explore validation
    //  OGRErr err = SRS->Validate();
    SRS->exportToProj4(&proj);
    outproj[0] = proj;
    info_out[0] = Rcpp::clone(outproj);

    SRS->exportToMICoordSys(&proj);
    outproj[0] = proj;
    info_out[1] = Rcpp::clone(outproj);

    SRS->exportToPrettyWkt(&proj, false);
    outproj[0] = proj;
    info_out[2] = Rcpp::clone(outproj);

    SRS->exportToWkt(&proj);
    outproj[0] = proj;
    info_out[3] = Rcpp::clone(outproj);

    int epsg = SRS->GetEPSGGeogCS();
    info_out[4] = epsg;

    SRS->exportToXML(&proj);
    outproj[0] = proj;
    info_out[5] = Rcpp::clone(outproj);

    CPLFree(proj);
  }

  // clean up if SQL was used https://www.gdal.org/classGDALDataset.html#ab2c2b105b8f76a279e6a53b9b4a182e0
  if (sql[0] != "") {
    poDS->ReleaseResultSet(poLayer);
  }
  GDALClose( poDS );
  return info_out;
}


} // namespace gdalheaders
#endif
