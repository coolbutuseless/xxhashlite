

#include <R.h>
#include <Rinternals.h>


#define XXH_STATIC_LINKING_ONLY   /* access advanced declarations */
#define XXH_IMPLEMENTATION   /* access definitions */

#include "xxhash.h"

#include <inttypes.h> // PRIx64


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Hash a byte
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void xxh32_hash_byte(R_outpstream_t stream, int c) {
  XXH32_state_t *xxstate = (XXH32_state_t *)stream->data;

  if (XXH32_update(xxstate, (unsigned char *)(&c), 1) == XXH_ERROR) {
    error("hash_byte: couldn't update state");
  }
}


void xxh64_hash_byte(R_outpstream_t stream, int c) {
  XXH64_state_t *xxstate = (XXH64_state_t *)stream->data;

  if (XXH64_update(xxstate, (unsigned char *)(&c), 1) == XXH_ERROR) {
    error("hash_byte: couldn't update state");
  }
}


void xxh3_128bits_hash_byte(R_outpstream_t stream, int c) {
  XXH3_state_t *xxstate = (XXH3_state_t *)stream->data;

  if (XXH3_128bits_update(xxstate, (unsigned char *)(&c), 1) == XXH_ERROR) {
    error("hash_byte: couldn't update state");
  }
}


void xxh3_64bits_hash_byte(R_outpstream_t stream, int c) {
  XXH3_state_t *xxstate = (XXH3_state_t *)stream->data;

  if (XXH3_64bits_update(xxstate, (unsigned char *)(&c), 1) == XXH_ERROR) {
    error("hash_byte: couldn't update state");
  }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Hash multiple bytes
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void xxh32_hash_bytes(R_outpstream_t stream, void *src, int length) {
  XXH32_state_t *xxstate = (XXH32_state_t *)stream->data;

  if (XXH32_update(xxstate, src, length) == XXH_ERROR) {
    error("hash_bytes: couldn't update state");
  }
}


void xxh64_hash_bytes(R_outpstream_t stream, void *src, int length) {
  XXH64_state_t *xxstate = (XXH64_state_t *)stream->data;

  if (XXH64_update(xxstate, src, length) == XXH_ERROR) {
    error("hash_bytes: couldn't update state");
  }
}


void xxh3_128bits_hash_bytes(R_outpstream_t stream, void *src, int length) {
  XXH3_state_t *xxstate = (XXH3_state_t *)stream->data;

  if (XXH3_128bits_update(xxstate, src, length) == XXH_ERROR) {
    error("hash_bytes: couldn't update state");
  }
}


void xxh3_64bits_hash_bytes(R_outpstream_t stream, void *src, int length) {
  XXH3_state_t *xxstate = (XXH3_state_t *)stream->data;

  if (XXH3_64bits_update(xxstate, src, length) == XXH_ERROR) {
    error("hash_bytes: couldn't update state");
  }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Serialize an R object
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_(SEXP robj_, SEXP algo_) {

  const char *algo = CHAR(asChar((algo_)));

  void *xxstate, *hash_byte, *hash_bytes;
  XXH_errorcode err;

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Set up the state
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (strcmp(algo, "xxh3") == 0) {
    xxstate = XXH3_createState();
    err = XXH3_64bits_reset(xxstate);
    hash_byte  = xxh3_64bits_hash_byte;
    hash_bytes = xxh3_64bits_hash_bytes;
  } else if (strcmp(algo, "xxh128") == 0) {
    xxstate = XXH3_createState();
    err = XXH3_128bits_reset(xxstate);
    hash_byte  = xxh3_128bits_hash_byte;
    hash_bytes = xxh3_128bits_hash_bytes;
  } else if (strcmp(algo, "xxh32") == 0) {
    xxstate = XXH32_createState();
    XXH32_hash_t const seed = 0;
    err = XXH32_reset((XXH32_state_t *)xxstate, seed);
    hash_byte  = xxh32_hash_byte;
    hash_bytes = xxh32_hash_bytes;
  } else if (strcmp(algo, "xxh64") == 0) {
    xxstate = XXH64_createState();
    XXH64_hash_t const seed = 0;
    err = XXH64_reset((XXH64_state_t *)xxstate, seed);
    hash_byte  = xxh64_hash_byte;
    hash_bytes = xxh64_hash_bytes;
  } else {
    error("Nope: algo = %s\n", algo);
  }

  if (err == XXH_ERROR) {
    error("xxhash(): couldn't initialise state for %s", algo);
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Create the output stream structure
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  struct R_outpstream_st output_stream;

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Initialise the output stream structure
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  R_InitOutPStream(
    &output_stream,             // The stream object which wraps everything
    (R_pstream_data_t) xxstate, // The "location" to write to
    R_pstream_binary_format,    // Store as binary
    3,                          // Version = 3 for R >3.5.0 See `?base::serialize`
    hash_byte,                  // Function to write single byte to buffer
    hash_bytes,                 // Function for writing multiple bytes to buffer
    NULL,                       // Func for special handling of reference data.
    R_NilValue                  // Data related to reference data handling
  );

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Serialize the object into the output_stream
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  R_Serialize(robj_, &output_stream);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Char string to hole longest hash of 128bits.
  // Each byte needs 2 chars in hex e.g. 255 = 'FF'
  // 128bits = 16bytes = 32 chars + 1-byte for trailing NULL
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  char chash[32+1];

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Produce the final hash value
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (strcmp(algo, "xxh3") == 0) {
    XXH64_hash_t const hash = XXH3_64bits_digest(xxstate);
    XXH3_freeState(xxstate);
    snprintf(chash, sizeof(chash), "%016" PRIx64, hash);
  } else if (strcmp(algo, "xxh128") == 0) {
    XXH128_hash_t const hash = XXH3_128bits_digest(xxstate);
    XXH3_freeState(xxstate);
    snprintf(chash, sizeof(chash), "%016" PRIx64 "%016" PRIx64, hash.high64, hash.low64);
  } else if (strcmp(algo, "xxh32") == 0) {
    XXH32_hash_t const hash = XXH32_digest(xxstate);
    XXH32_freeState(xxstate);
    snprintf(chash, sizeof(chash), "%08x", hash);
  } else if (strcmp(algo, "xxh64") == 0) {
    XXH64_hash_t const hash = XXH64_digest(xxstate);
    XXH64_freeState(xxstate);
    snprintf(chash, sizeof(chash), "%016" PRIx64, hash);
  } else {
    error("Nope: algo = %s\n", algo);
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP result = PROTECT(allocVector(STRSXP, 1));
  SET_STRING_ELT(result, 0, mkChar(chash));

  UNPROTECT(1);
  return result;
}




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
  if (strcmp(algo, "xxh3") == 0){
    XXH64_hash_t const hash = XXH3_64bits(src, len);
    snprintf(chash, sizeof(chash), "%016" PRIx64, hash);
  } else if (strcmp(algo, "xxh128") == 0) {
    XXH128_hash_t const hash = XXH3_128bits(src, len);
    snprintf(chash, sizeof(chash), "%016" PRIx64 "%016" PRIx64, hash.high64, hash.low64);
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


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_file_(SEXP robj_, SEXP algo_) {
  return R_NilValue;
}




