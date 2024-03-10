

#include <R.h>
#include <Rinternals.h>


#define XXH_STATIC_LINKING_ONLY   /* access advanced declarations */
#define XXH_IMPLEMENTATION   /* access definitions */

#include "xxhash.h"
#include "R-xxhash-utils.h"
#include <inttypes.h> // PRIx64


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Serialize an R object
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_raw_(SEXP robj_, SEXP algo_, SEXP as_raw_) {
  
  const char *algo = CHAR(asChar((algo_)));
  
  void *src;
  size_t len;
  char *tmp;
  
  if (TYPEOF(robj_) == RAWSXP) {
    src = (void *)RAW(robj_);
    len = (size_t)length(robj_);
  } else if (TYPEOF(robj_) == STRSXP) {
    if (length(robj_) != 1) {
      error("xxhash_raw() character arguments must only be length-1");
    }
    tmp = (char *)CHAR(STRING_ELT(robj_, 0));
    src = (void *)tmp;
    len = strlen(tmp);
  } else {
    error("xxhash_raw_(): only raw vectors and strings are supported");
  }
  
  SEXP res_ = R_NilValue;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Set up the state
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (strcmp(algo, "xxh128") == 0) {
    XXH128_hash_t const hash = XXH3_128bits(src, len);
    res_ = PROTECT(xxh128_hash_to_robj(hash, as_raw_));
  } else if (strcmp(algo, "xxh3") == 0){
    XXH64_hash_t const hash = XXH3_64bits(src, len);
    res_ = PROTECT(xxh64_hash_to_robj(hash, as_raw_));
  } else if (strcmp(algo, "xxh32") == 0) {
    XXH32_hash_t const hash = XXH32(src, len, 0);
    res_ = PROTECT(xxh32_hash_to_robj(hash, as_raw_));
  } else if (strcmp(algo, "xxh64") == 0) {
    XXH64_hash_t const hash = XXH64(src, len, 0);
    res_ = PROTECT(xxh64_hash_to_robj(hash, as_raw_));
  } else {
    error("Nope: algo = %s\n", algo);
  }
  
  UNPROTECT(1);
  return res_;
}

