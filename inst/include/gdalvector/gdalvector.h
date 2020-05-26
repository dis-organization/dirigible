#ifndef GDALVECTOR_H
#define GDALVECTOR_H
#include <Rcpp.h>
#include "ogrsf_frmts.h"
#include "ogr_api.h"


namespace gdalvector {
  using namespace Rcpp;

  inline void gdal_register() {
    GDALAllRegister();
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

} // namespace gdalvector
#endif


