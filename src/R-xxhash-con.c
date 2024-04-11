

#include <R.h>
#include <Rinternals.h>


#define XXH_STATIC_LINKING_ONLY   /* access advanced declarations */

#include "xxhash.h"
#include "R-xxhash-utils.h"

#define BUFSIZE 1 << 17

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Read data from the connection
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP read_connection(SEXP con_) {
  // Call 'readBin(con, raw(), length)' in R
  // Modelled after jsonlite/src/push_parser.c  
  SEXP call = PROTECT(
    Rf_lang4(
      PROTECT(Rf_install("readBin")),
      con_,                                 // con
      PROTECT(Rf_allocVector(RAWSXP, 0)),   // what = raw()
      PROTECT(Rf_ScalarInteger(BUFSIZE))     // n
    ));
  
  // Actually evaluate the readBin() call
  SEXP data_ = PROTECT(Rf_eval(call, R_BaseEnv));
  
  UNPROTECT(5);
  return data_;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// XXH128
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_con_xxh128(SEXP con_, SEXP as_raw_) {
  
  XXH3_state_t* state = XXH3_createState();
  XXH3_128bits_reset(state);
  
  while (1) {
    SEXP data_ = PROTECT(read_connection(con_));
    if (XXH3_128bits_update(state, RAW(data_), (size_t)Rf_xlength(data_)) == XXH_ERROR) {
      error("xxhash_con_xxh128(): Couldn't update state");
    }
    UNPROTECT(1);
    if (Rf_xlength(data_) < BUFSIZE) break;
  }
  
  XXH128_hash_t hash = XXH3_128bits_digest(state);
  XXH3_freeState(state);
  
  return xxh128_hash_to_robj(hash, as_raw_);
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// XXH3 64bit
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_con_xxh3(SEXP con_, SEXP as_raw_) {
  
  XXH3_state_t* state = XXH3_createState();
  XXH3_64bits_reset(state);
  
  while (1) {
    SEXP data_ = PROTECT(read_connection(con_));
    if (XXH3_64bits_update(state, RAW(data_), (size_t)Rf_xlength(data_)) == XXH_ERROR) {
      error("xxhash_con_xxh3(): Couldn't update state");
    }
    UNPROTECT(1);
    if (Rf_xlength(data_) < BUFSIZE) break;
  }
  
  XXH64_hash_t hash = XXH3_64bits_digest(state);
  XXH3_freeState(state);
  
  return xxh64_hash_to_robj(hash, as_raw_);
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// XXH32
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_con_xxh32(SEXP con_, SEXP as_raw_) {
  
  XXH32_state_t *state = XXH32_createState();
  XXH32_reset(state, 0);

  while (1) {
    SEXP data_ = PROTECT(read_connection(con_));
    if (XXH32_update(state, RAW(data_), (size_t)Rf_xlength(data_)) == XXH_ERROR) {
      error("xxhash_con_xxh32(): Couldn't update state");
    }
    UNPROTECT(1);
    if (Rf_xlength(data_) < BUFSIZE) break;
  }
  
  XXH32_hash_t hash = XXH32_digest(state);
  XXH32_freeState(state);
  
  return xxh32_hash_to_robj(hash, as_raw_);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// XXH64
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_con_xxh64(SEXP con_, SEXP as_raw_) {
  
  XXH64_state_t *state = XXH64_createState();
  XXH64_reset(state, 0);
  
  while (1) {
    SEXP data_ = PROTECT(read_connection(con_));
    if (XXH64_update(state, RAW(data_), (size_t)Rf_xlength(data_)) == XXH_ERROR) {
      error("xxhash_con_xxh64(): Couldn't update state");
    }
    UNPROTECT(1);
    if (Rf_xlength(data_) < BUFSIZE) break;
  }
  
  XXH64_hash_t hash = XXH64_digest(state);
  XXH64_freeState(state);
  
  return xxh64_hash_to_robj(hash, as_raw_);
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Serialize an R object
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_con_(SEXP con_, SEXP algo_, SEXP as_raw_) {
  
  const char *algo = CHAR(asChar((algo_)));
  SEXP res_ = R_NilValue;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Set up the state
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (strcmp(algo, "xxh128") == 0) {
    res_ = PROTECT(xxhash_con_xxh128(con_, as_raw_));
  } else if (strcmp(algo, "xxh3") == 0) {
    res_ = PROTECT(xxhash_con_xxh3(con_, as_raw_));
  } else if (strcmp(algo, "xxh32") == 0) {
    res_ = PROTECT(xxhash_con_xxh32(con_, as_raw_));
  } else if (strcmp(algo, "xxh64") == 0) {
    res_ = PROTECT(xxhash_con_xxh64(con_, as_raw_));
  } else {
    error("xxhash_con_(): Unknown algo '%s'", algo);
  }
  
  UNPROTECT(1);
  return res_;
}

