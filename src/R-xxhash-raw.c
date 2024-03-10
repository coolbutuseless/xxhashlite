

#include <R.h>
#include <Rinternals.h>


#define XXH_STATIC_LINKING_ONLY   /* access advanced declarations */
#define XXH_IMPLEMENTATION   /* access definitions */

#include "xxhash.h"

#include <inttypes.h> // PRIx64


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Serialize an R object
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_raw_(SEXP robj_, SEXP algo_) {
  
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
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Char string to hole longest hash of 128bits.
  // Each byte needs 2 chars in hex e.g. 255 = 'FF'
  // 128bits = 16bytes = 32 chars + 1-byte for trailing NULL
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  char chash[32+1];
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Set up the state
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (strcmp(algo, "xxh128") == 0) {
    XXH128_hash_t const hash = XXH3_128bits(src, len);
    snprintf(chash, sizeof(chash), "%016" PRIx64 "%016" PRIx64, hash.high64, hash.low64);
  } else if (strcmp(algo, "xxh3") == 0){
    XXH64_hash_t const hash = XXH3_64bits(src, len);
    snprintf(chash, sizeof(chash), "%016" PRIx64, hash);
  } else if (strcmp(algo, "xxh32") == 0) {
    XXH32_hash_t const hash = XXH32(src, len, 0);
    snprintf(chash, sizeof(chash), "%08x", hash);
  } else if (strcmp(algo, "xxh64") == 0) {
    XXH64_hash_t const hash = XXH64(src, len, 0);
    snprintf(chash, sizeof(chash), "%016" PRIx64, hash);
  } else {
    error("Nope: algo = %s\n", algo);
  }
  

  return mkString(chash);
}

