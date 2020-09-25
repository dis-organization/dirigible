#ifndef GDALWARPMEM_H
#define GDALWARPMEM_H
#include <Rcpp.h>
#include "gdal.h"
#include "gdal_alg.h"
#include "gdal_alg_priv.h"
#include "gdal_priv.h"
#include "gdalwarper.h"
#include "gdal_utils.h"
#include "gdal_version.h"
//#include "commonutils.h"
//#include "gdal_utils_priv.h"

#include "cpl_conv.h" // for CPLMalloc()

//#include "opt.h"

//inline std::vector<char *> create_options(Rcpp::CharacterVector lco, bool quiet = true);

namespace gdalwarpmem{


using namespace Rcpp;


inline List gdal_warp_in_memory(CharacterVector source_filename,
                     CharacterVector source_WKT,
                     CharacterVector target_WKT,
                     NumericVector target_geotransform,
                     IntegerVector target_dim,
                     IntegerVector band) {

  std::vector<GDALDatasetH> po_SrcDS(source_filename.size());

  GDALDatasetH hDstDS;
  GDALRasterBandH poBand, dstBand;
  GDALDriverH hDriver;
  GDALDataType eDT;

  // Open input and output files.
  GDALAllRegister();
  Rcpp::CharacterVector oo;
  std::vector <char *> oo_char; // = create_options(oo, true); // open options

//  hSrcDS = GDALOpenEx( source_filename[0], GA_ReadOnly, NULL, oo_char.data(), NULL );

  for (int i = 0; i < source_filename.size(); i++) {
    po_SrcDS[i] = GDALOpenEx((const char *) source_filename[i], GA_ReadOnly, NULL, oo_char.data(), NULL);
  }

  CPLAssert( hSrcDS[0] != NULL );

  if (source_WKT[0].empty()) {
    // do nothing
  } else {

    Rprintf("setting projection");
  //  GDALSetProjection( hSrcDS, source_WKT[0] );
  }
  poBand = GDALGetRasterBand(po_SrcDS[0], 1);

  //TODO need type handling for nodata
  //int serr;
  //double no_data = GDALGetRasterNoDataValue(poBand, &serr);
  // Create output with same datatype as first input band.
  eDT = GDALGetRasterDataType(poBand);
  // Get output driver
  hDriver = GDALGetDriverByName( "MEM" );

  // Create the output file.
  hDstDS = GDALCreate( hDriver, "", target_dim[0], target_dim[1],
                       GDALGetRasterCount(po_SrcDS[0]), eDT, NULL );


  CPLAssert( hDstDS != NULL );

  // Write out the projection definition.
  GDALSetProjection( hDstDS, target_WKT[0] );
  double GeoTransform[6];
  for (int i = 0; i < 6; i++) GeoTransform[i] = target_geotransform[i];
  GDALSetGeoTransform( hDstDS, GeoTransform );

  Rcpp::CharacterVector options;
  std::vector <char *> options_char; //create_options(options, true);
  GDALWarpAppOptions* psOptions = GDALWarpAppOptionsNew(options_char.data(), NULL);


  int err_0 = 0;
  GDALDatasetH hOutDS = GDALWarp(NULL, hDstDS, 1, po_SrcDS.data(), psOptions, &err_0);


  // // Establish reprojection transformer.
  // psWarpOptions->pTransformerArg =
  //   GDALCreateGenImgProjTransformer( hSrcDS,
  //                                    GDALGetProjectionRef(hSrcDS),
  //                                    hDstDS,
  //                                    GDALGetProjectionRef(hDstDS),
  //                                    FALSE, 0.0, 1 );
  // psWarpOptions->pfnTransformer = GDALGenImgProjTransform;
  //
  // // Initialize and execute the warp operation.
  // GDALWarpOperation oOperation;
  // oOperation.Initialize( psWarpOptions );
  // oOperation.ChunkAndWarpMulti( 0, 0,
  //                               GDALGetRasterXSize( hDstDS ),
  //                               GDALGetRasterYSize( hDstDS ) );
  // GDALDestroyGenImgProjTransformer( psWarpOptions->pTransformerArg );
  // GDALDestroyWarpOptions( psWarpOptions );
  //

   double *double_scanline;
  double_scanline = (double *) CPLMalloc(sizeof(double)*
     static_cast<unsigned long>(target_dim[0])*
     static_cast<unsigned long>(target_dim[1]));
  // //GDALRasterIOExtraArg psExtraArg;
  // //INIT_RASTERIO_EXTRA_ARG(psExtraArg);
  //
  //
  CPLErr err;
  dstBand = GDALGetRasterBand(hOutDS, band[0]);

   err = GDALRasterIO(dstBand,  GF_Read, 0, 0, target_dim[0], target_dim[1],
                      double_scanline, target_dim[0], target_dim[1], GDT_Float64,
                      0, 0);
  NumericVector res(target_dim[0] * target_dim[1]);
  for (int i = 0; i < (target_dim[0] * target_dim[1]); i++) {
    //  if (((float)double_scanline[i] == (float)no_data) || ISNAN(double_scanline[i])) {
    //    res[i] = NA_REAL;
    //  } else {
    res[i] = double_scanline[i];
    //  }
  }

  GDALClose( hDstDS );
  GDALClose( po_SrcDS[0] );

  Rcpp::List outlist(1);  //hardcode to 1 for now
  outlist[0] = res;

  return outlist;
}

 } // namespace gdalwarpmem
#endif
