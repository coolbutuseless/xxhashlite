

#include <R.h>
#include <Rinternals.h>


#define XXH_STATIC_LINKING_ONLY   /* access advanced declarations */
#define XXH_IMPLEMENTATION   /* access definitions */

#include "xxhash.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  xxhash 32-bit
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
SEXP xxhash32_(SEXP buffer_) {
  int sexp_type = TYPEOF(buffer_);
  int size = length(buffer_);
  SEXP result = R_NilValue;
  XXH32_hash_t hash;
  char chash[8+1];    /* 32bits = 4bytes = 8 chars + 1-byte for trailing NULL */

  switch(sexp_type) {
  case LGLSXP:
  case INTSXP:
    hash = XXH32(INTEGER(buffer_), size*4, 0);
    break;
  case REALSXP:
    hash = XXH32(REAL(buffer_), size*8, 0);
    break;
  case RAWSXP:
    hash = XXH32(RAW(buffer_), size, 0);
    break;
  default:
    error("xxhash32 cannot handles SEXP type: %i\n", sexp_type);
  }

  /* convert the hash to a character string */
  sprintf(chash, "%08x", hash);
  PROTECT(result=allocVector(STRSXP, 1));
  SET_STRING_ELT(result, 0, mkChar(chash));

  UNPROTECT(1);
  return result;
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  xxhash 64-bit
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
SEXP xxhash64_(SEXP buffer_) {
  int sexp_type = TYPEOF(buffer_);
  int size = length(buffer_);
  SEXP result = R_NilValue;
  XXH64_hash_t hash;
  char chash[16+1];    /* 64bits = 8bytes = 16 chars + 1-byte for trailing NULL */

switch(sexp_type) {
case LGLSXP:
case INTSXP:
  hash = XXH64(INTEGER(buffer_), size*4, 0);
  break;
case REALSXP:
  hash = XXH64(REAL(buffer_), size*8, 0);
  break;
case RAWSXP:
  hash = XXH64(RAW(buffer_), size, 0);
  break;
default:
  error("xxhash64 cannot handles SEXP type: %i\n", sexp_type);
}

/* convert the hash to a character string */
sprintf(chash, "%016llx", hash);
PROTECT(result=allocVector(STRSXP, 1));
SET_STRING_ELT(result, 0, mkChar(chash));

UNPROTECT(1);
return result;
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  xxhash 128-bit
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
SEXP xxhash128_(SEXP buffer_) {
  int sexp_type = TYPEOF(buffer_);
  int size = length(buffer_);
  SEXP result = R_NilValue;
  XXH128_hash_t hash;
  char chash[32+1];    /* 128bits = 16bytes = 32 chars + 1-byte for trailing NULL */

switch(sexp_type) {
case LGLSXP:
case INTSXP:
  hash = XXH128(INTEGER(buffer_), size*4, 0);
  break;
case REALSXP:
  hash = XXH128(REAL(buffer_), size*8, 0);
  break;
case RAWSXP:
  hash = XXH128(RAW(buffer_), size, 0);
  break;
default:
  error("xxhash64 cannot handles SEXP type: %i\n", sexp_type);
}

/* convert the hash to a character string */
sprintf(chash, "%016llx%016llx", hash.high64, hash.low64);
PROTECT(result=allocVector(STRSXP, 1));
SET_STRING_ELT(result, 0, mkChar(chash));

UNPROTECT(1);
return result;
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  xxh3 64-bit
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
SEXP xxh3_64bits_(SEXP buffer_) {
  int sexp_type = TYPEOF(buffer_);
  int size = length(buffer_);
  SEXP result = R_NilValue;
  XXH64_hash_t hash;
  char chash[16+1];    /* 64bits = 8bytes = 16 chars + 1-byte for trailing NULL */

switch(sexp_type) {
case LGLSXP:
case INTSXP:
  hash = XXH3_64bits(INTEGER(buffer_), size*4);
  break;
case REALSXP:
  hash = XXH3_64bits(REAL(buffer_), size*8);
  break;
case RAWSXP:
  hash = XXH3_64bits(RAW(buffer_), size);
  break;
default:
  error("XXH3_64bits cannot handles SEXP type: %i\n", sexp_type);
}

/* convert the hash to a character string */
sprintf(chash, "%016llx", hash);
PROTECT(result=allocVector(STRSXP, 1));
SET_STRING_ELT(result, 0, mkChar(chash));

UNPROTECT(1);
return result;
}




/* https://stackoverflow.com/questions/42313373/r-cmd-check-note-found-no-calls-to-r-registerroutines-r-usedynamicsymbols#42339658 */
void R_init_xxhashlite(DllInfo *info) {
  R_registerRoutines(info, NULL, NULL, NULL, NULL);
  R_useDynamicSymbols(info, TRUE);
}
