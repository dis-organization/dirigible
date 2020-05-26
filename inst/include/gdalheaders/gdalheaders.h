#ifndef GDALHEADERS_H
#define GDALHEADERS_H
#include <Rcpp.h>
#include "ogrsf_frmts.h"
//#include "ogr_api.h"
#include "gdal_priv.h"

namespace gdalheaders {
  using namespace Rcpp;

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


