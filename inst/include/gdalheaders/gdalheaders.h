#ifndef GDALHEADERS_H
#define GDALHEADERS_H
#include <Rcpp.h>
#include "ogrsf_frmts.h"
//#include "ogr_api.h"
#include "gdal_priv.h"

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


  // [[Rcpp::export]]
  List gdal_read_fields(Rcpp::CharacterVector dsource,
                                  Rcpp::IntegerVector layer,
                                  Rcpp::CharacterVector sql,
                                  Rcpp::IntegerVector limit_n,
                                  Rcpp::IntegerVector skip_n,
                                  Rcpp::NumericVector ex,
                                  Rcpp::CharacterVector fid_column_name)
  {
    GDALDataset       *poDS;
    poDS = (GDALDataset*) GDALOpenEx(dsource[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
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
    Rcpp::List out = allocate_fields_list(poFDefn, nFeature, int64_as_string, fid_column_name);
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
            Rcpp::IntegerVector nv;
            nv = out[iField];
            nv[lFeature] = poFeature->GetFieldAsInteger( iField );
          }

          if( poFieldDefn->GetType() == OFTReal || poFieldDefn->GetType() == OFTInteger64) {
            Rcpp::NumericVector nv;
            nv = out[iField];
            nv[lFeature] = poFeature->GetFieldAsDouble( iField );
          }

          if( poFieldDefn->GetType() == OFTString || poFieldDefn->GetType() == OFTDate || poFieldDefn->GetType() == OFTTime || poFieldDefn->GetType() == OFTDateTime) {
            Rcpp::CharacterVector nv;
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


  inline DoubleVector gdal_feature_count(CharacterVector dsource,
                                      IntegerVector layer,
                                      LogicalVector iterate) {
    GDALDataset       *poDS;
    poDS = (GDALDataset*) GDALOpenEx(dsource[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
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
  inline Rcpp::CharacterVector gdal_driver(Rcpp::CharacterVector dsource)
  {

    GDALDataset       *poDS;
    poDS = (GDALDataset*) GDALOpenEx(dsource[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
    if( poDS == NULL )
    {
      Rcpp::stop("Open failed.\n");
    }
    Rcpp::CharacterVector dname(1);
    dname[0] = poDS->GetDriverName();
    GDALClose(poDS);
    return(dname);
  } // gdal_driver

  inline Rcpp::CharacterVector gdal_layer_names(Rcpp::CharacterVector dsource,
                                                Rcpp::CharacterVector sql = "")
  {
    GDALDataset       *poDS;
    poDS = (GDALDataset*) GDALOpenEx(dsource[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
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
    Rcpp::CharacterVector lnames = Rcpp::CharacterVector(nlayer);
    for (int ilayer = 0; ilayer < nlayer; ilayer++) {
      poLayer = poDS->GetLayer(ilayer);
      lnames[ilayer] = poLayer->GetName();
    }
    GDALClose(poDS);
    return(lnames);
  } // gdal_layer_names

} // namespace gdalheaders
#endif


