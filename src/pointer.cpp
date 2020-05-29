#include <Rcpp.h>
using namespace Rcpp;
#include "gdal_priv.h"

// [[Rcpp::export]]
SEXP gh_GDALOpenEx(CharacterVector dsn) {
  // create pointer to an GDAL object and
  // wrap it as an external pointer
  GDALDataset       *poDS;
  poDS = (GDALDataset*) GDALOpenEx(dsn[0], GDAL_OF_READONLY, NULL, NULL, NULL);
  //poDS = (GDALDataset*) GDALOpen(dsn[0], GDAL_OF_READONLY);

  if( poDS == NULL )
  {
    Rprintf("Problem with 'dsn' input: %s\n", dsn[0]);
    Rcpp::stop("Open failed.\n");
  }
  Rcpp::XPtr<GDALDataset> ptr(poDS);
  return ptr;
}


/// invoke the close method
// [[Rcpp::export]]
SEXP gh_GDALClose(SEXP xp) {
  // grab the object as a XPtr (smart pointer)
  // to GDALDataset
  Rcpp::XPtr<GDALDataset> ptr(xp);
  NumericVector res(1);
  res[0] = 0;
  GDALClose(ptr);
  res[0] = 1;
  // return the result to R
  return res;
}

// surely no point to a hook like this, we just want a src/ wrap to do what gdal_list_drivers() gets
// [[Rcpp::export]]
SEXP gh_GetGDALDriverManager()  {
  GDALDriverManager *gdm  = GetGDALDriverManager();
 Rcpp::XPtr<GDALDriverManager> ptr(gdm);
 return ptr;
}

// but I'm practicing the art ...
// [[Rcpp::export]]
SEXP gh_GetDriverCount(SEXP xp)  {
  Rcpp::XPtr<GDALDriverManager> ptr(xp);
  int n = ptr->GetDriverCount();
  return Rcpp::wrap(n);
}

