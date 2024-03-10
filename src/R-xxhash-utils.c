

#include <R.h>
#include <Rinternals.h>
#include <inttypes.h>

#define XXH_STATIC_LINKING_ONLY   /* access advanced declarations */
#include "xxhash.h"

#include "R-xxhash-utils.h"

SEXP xxh128_hash_to_robj(XXH128_hash_t hash, SEXP raw_) {
  SEXP res_ = R_NilValue;
  char chash[32+1];
  
  if (asLogical(raw_)) {
    res_ = PROTECT(allocVector(RAWSXP, 16));
    memcpy(RAW(res_)    , &hash.high64, 8);
    memcpy(RAW(res_) + 8, &hash.low64 , 8);
  } else {
    snprintf(chash, sizeof(chash), "%016" PRIx64 "%016" PRIx64, hash.high64, hash.low64);
    res_ = PROTECT(mkString(chash));
  }
  
  return res_;
} 



SEXP xxh64_hash_to_robj(XXH64_hash_t hash, SEXP raw_) {
  SEXP res_ = R_NilValue;
  char chash[16+1];
  
  if (asLogical(raw_)) {
    res_ = PROTECT(allocVector(RAWSXP, 8));
    memcpy(RAW(res_), &hash, 8);
  } else {
    snprintf(chash, sizeof(chash), "%016" PRIx64, hash);
    res_ = PROTECT(mkString(chash));
  }
  
  return res_;
} 



SEXP xxh32_hash_to_robj(XXH32_hash_t hash, SEXP raw_) {
  SEXP res_ = R_NilValue;
  char chash[8+1];
  
  if (asLogical(raw_)) {
    res_ = PROTECT(allocVector(RAWSXP, 4));
    memcpy(RAW(res_), &hash, 4);
  } else {
    snprintf(chash, sizeof(chash), "%08x", hash);
    res_ = PROTECT(mkString(chash));
  }
  
  return res_;
} 
