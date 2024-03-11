

#include <R.h>
#include <Rinternals.h>
#include <inttypes.h>

#define XXH_STATIC_LINKING_ONLY   /* access advanced declarations */
#include "xxhash.h"

#include "R-xxhash-utils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxh128_hash_to_robj(XXH128_hash_t hash, SEXP as_raw_) {
  SEXP res_ = R_NilValue;
  
  if (asLogical(as_raw_)) {
    res_ = PROTECT(allocVector(RAWSXP, 16));
    XXH128_canonicalFromHash((XXH128_canonical_t *)RAW(res_), hash);
  } else {
    char chash[32+1];
    snprintf(chash, sizeof(chash), "%016" PRIx64 "%016" PRIx64, hash.high64, hash.low64);
    res_ = PROTECT(mkString(chash));
  }
  
  UNPROTECT(1);
  return res_;
} 


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxh64_hash_to_robj(XXH64_hash_t hash, SEXP as_raw_) {
  SEXP res_ = R_NilValue;
  
  if (asLogical(as_raw_)) {
    res_ = PROTECT(allocVector(RAWSXP, 8));
    XXH64_canonicalFromHash((XXH64_canonical_t *)RAW(res_), hash);
  } else {
    char chash[16+1];
    snprintf(chash, sizeof(chash), "%016" PRIx64, hash);
    res_ = PROTECT(mkString(chash));
  }
  
  UNPROTECT(1);
  return res_;
} 


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxh32_hash_to_robj(XXH32_hash_t hash, SEXP as_raw_) {
  SEXP res_ = R_NilValue;
  
  if (asLogical(as_raw_)) {
    res_ = PROTECT(allocVector(RAWSXP, 4));
    XXH32_canonicalFromHash((XXH32_canonical_t *)RAW(res_), hash);
  } else {
    char chash[8+1];
    snprintf(chash, sizeof(chash), "%08x", hash);
    res_ = PROTECT(mkString(chash));
  }
  
  UNPROTECT(1);
  return res_;
} 
