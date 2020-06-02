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

inline List gdal_geometry_(CharacterVector dsn,
                               IntegerVector layer,
                               CharacterVector what,
                               IntegerVector set)
{
  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( poDS == NULL )
  {
    Rcpp::stop("Open failed.\n");
  }
  OGRLayer *poLayer = gdalheaders::gdal_layer(poDS, layer, "", 0);

  OGRFeature *poFeature;
  OGRGeometry *poGeometry;
  poLayer->ResetReading();

  //OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
  int len = set.length();

  List feature_xx(len);

  int iFeature = 0;
  int lFeature = 0;
  int curr_set = 0;
  int nFeature = gdalheaders::force_layer_feature_count(poLayer);


    // what could be fid, names, ;
   if (what[0] == "fid") {
     // set is ordered integers from 0:(nFeature-1)
    for (int iter = 0; iter < set.length(); iter++) {
        GIntBig fid = (GIntBig)set[curr_set];

         poFeature = poLayer->GetFeature(fid);
         poGeometry = poFeature->GetGeometryRef();
         Rcpp::RawVector raw(poGeometry->WkbSize());
         poGeometry->exportToWkb(wkbNDR, &(raw[0]), wkbVariantIso);
         feature_xx[curr_set] = raw;
         OGRFeature::DestroyFeature(poFeature);
         curr_set++;
     }
    }

   return feature_xx;
  }


}

#endif
