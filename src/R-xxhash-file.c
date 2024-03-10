

#include <R.h>
#include <Rinternals.h>
#include <inttypes.h>

#define XXH_STATIC_LINKING_ONLY   /* access advanced declarations */
#include "xxhash.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// XXH128
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_file_xxh128(FILE *f) {
  
  XXH3_state_t* state = XXH3_createState();
  XXH3_128bits_reset(state);
  char buffer[4096];
  size_t count;
  
  while ((count = fread(buffer, 1, sizeof(buffer), f)) != 0) {
    XXH3_128bits_update(state, buffer, count);
  }
  
  XXH128_hash_t hash = XXH3_128bits_digest(state);
  XXH3_freeState(state);
  
  char chash[32+1];
  snprintf(chash, sizeof(chash), "%016" PRIx64 "%016" PRIx64, hash.high64, hash.low64);
  
  return mkString(chash);
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// XXH3 64bit
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_file_xxh3(FILE *f) {
  
  XXH3_state_t* state = XXH3_createState();
  XXH3_64bits_reset(state);
  char buffer[4096];
  size_t count;
  
  while ((count = fread(buffer, 1, sizeof(buffer), f)) != 0) {
    XXH3_64bits_update(state, buffer, count);
  }
  
  XXH64_hash_t hash = XXH3_64bits_digest(state);
  XXH3_freeState(state);
  
  char chash[32+1];
  snprintf(chash, sizeof(chash), "%016" PRIx64, hash);
  
  return mkString(chash);
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// XXH32
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_file_xxh32(FILE *f) {
  
  XXH32_state_t *state = XXH32_createState();
  XXH32_reset(state, 0);
  
  char buffer[4096];
  size_t count;
  
  while ((count = fread(buffer, 1, sizeof(buffer), f)) != 0) {
    XXH32_update(state, buffer, count);
  }
  
  XXH32_hash_t hash = XXH32_digest(state);
  XXH32_freeState(state);
  
  char chash[32+1];
  snprintf(chash, sizeof(chash), "%08x", hash);
  
  return mkString(chash);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// XXH64
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_file_xxh64(FILE *f) {
  
  XXH64_state_t *state = XXH64_createState();
  XXH64_reset(state, 0);
  
  char buffer[4096];
  size_t count;
  
  while ((count = fread(buffer, 1, sizeof(buffer), f)) != 0) {
    XXH64_update(state, buffer, count);
  }
  
  XXH64_hash_t hash = XXH64_digest(state);
  XXH64_freeState(state);
  
  char chash[32+1];
  snprintf(chash, sizeof(chash), "%016" PRIx64, hash);
  
  return mkString(chash);
}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_file_(SEXP file_, SEXP algo_) {
  
  const char *file = CHAR(STRING_ELT(file_, 0));
  FILE *f = fopen(file, "rb");
  if (f == NULL) {
    error("xxhash_file_(): Couldn't open file '%s'", file);
  }
  
  const char *algo = CHAR(STRING_ELT(algo_, 0));
  SEXP res_ = R_NilValue;
  if (strcmp(algo, "xxh128") == 0) {
    res_ = PROTECT(xxhash_file_xxh128(f));
  } else if (strcmp(algo, "xxh3") == 0) {
    res_ = PROTECT(xxhash_file_xxh3(f));
  } else if (strcmp(algo, "xxh32") == 0) {
    res_ = PROTECT(xxhash_file_xxh32(f));
  } else if (strcmp(algo, "xxh64") == 0) {
    res_ = PROTECT(xxhash_file_xxh64(f));
  } 
  
  fclose(f);
  UNPROTECT(1);
  return res_;
}
