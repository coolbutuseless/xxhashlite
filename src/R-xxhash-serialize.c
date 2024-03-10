

#include <R.h>
#include <Rinternals.h>
#include <stdbool.h>
#include <inttypes.h> // PRIx64

#define XXH_STATIC_LINKING_ONLY   /* access advanced declarations */

#include "xxhash.h"
#include "R-xxhash-utils.h"

typedef struct {
  bool in_header;
  int n;
  int enc_size;
  void *xxstate;
} ser_state_t;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Hash a byte
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void hash_byte(R_outpstream_t stream, int c) {
  error("Single byte hashing should never be called during binary serialisation");
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Hash multiple bytes
//
// Similar to rlang::hash(), we ignore the first 18+n bytes which are just
// the version-specific header for the data.
// The 'n' represents the length of the string used to specify the
// native encodeing.  This is often a 5 byte string "UTF-8"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void xxh3_128bits_hash_bytes(R_outpstream_t stream, void *src, int n) {
  ser_state_t *ser_state = (ser_state_t *)stream->data;
  
  if (ser_state->in_header) {
    ser_state->n += n;
    if (ser_state->n == 18) {
      memcpy(&ser_state->enc_size, src, sizeof(int));
    }
    if (ser_state->n == 18 + ser_state->enc_size) {
      ser_state->in_header = false;
    }
    return;
  }
  
  XXH3_state_t *xxstate = (XXH3_state_t *)ser_state->xxstate;
  if (XXH3_128bits_update(xxstate, src, (size_t)n) == XXH_ERROR) {
    error("hash_bytes: couldn't update state");
  }
}


void xxh3_64bits_hash_bytes(R_outpstream_t stream, void *src, int n) {
  ser_state_t *ser_state = (ser_state_t *)stream->data;
  
  if (ser_state->in_header) {
    ser_state->n += n;
    if (ser_state->n == 18) {
      memcpy(&ser_state->enc_size, src, sizeof(int));
    }
    if (ser_state->n == 18 + ser_state->enc_size) {
      ser_state->in_header = false;
    }
    return;
  }
  
  XXH3_state_t *xxstate = (XXH3_state_t *)ser_state->xxstate;
  if (XXH3_64bits_update(xxstate, src, (size_t)n) == XXH_ERROR) {
    error("hash_bytes: couldn't update state");
  }
}


void xxh32_hash_bytes(R_outpstream_t stream, void *src, int n) {
  ser_state_t *ser_state = (ser_state_t *)stream->data;
  
  if (ser_state->in_header) {
    ser_state->n += n;
    if (ser_state->n == 18) {
      memcpy(&ser_state->enc_size, src, sizeof(int));
    }
    if (ser_state->n == 18 + ser_state->enc_size) {
      ser_state->in_header = false;
    }
    return;
  }
  
  XXH32_state_t *xxstate = (XXH32_state_t *)ser_state->xxstate;
  if (XXH32_update(xxstate, src, (size_t)n) == XXH_ERROR) {
    error("hash_bytes: couldn't update state");
  }
}


void xxh64_hash_bytes(R_outpstream_t stream, void *src, int n) {
  ser_state_t *ser_state = (ser_state_t *)stream->data;
  
  if (ser_state->in_header) {
    ser_state->n += n;
    if (ser_state->n == 18) {
      memcpy(&ser_state->enc_size, src, sizeof(int));
    }
    if (ser_state->n == 18 + ser_state->enc_size) {
      ser_state->in_header = false;
    }
    return;
  }
  
  XXH64_state_t *xxstate = (XXH64_state_t *)ser_state->xxstate;
  if (XXH64_update(xxstate, src, (size_t)n) == XXH_ERROR) {
    error("hash_bytes: couldn't update state");
  }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Serialize an R object
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP xxhash_(SEXP robj_, SEXP algo_, SEXP as_raw_) {

  const char *algo = CHAR(asChar((algo_)));

  void *hash_bytes;
  XXH_errorcode err;
  ser_state_t ser_state = {
    .in_header = true,
    .n = 0,
    .xxstate = NULL
  };

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Set up the state
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (strcmp(algo, "xxh128") == 0) {
    XXH3_state_t *xxstate = XXH3_createState();
    ser_state.xxstate = (void *)xxstate;
    err = XXH3_128bits_reset(xxstate);
    hash_bytes = (void *)xxh3_128bits_hash_bytes;
  } else if (strcmp(algo, "xxh3") == 0) {
    XXH3_state_t *xxstate = XXH3_createState();
    ser_state.xxstate = (void *)xxstate;
    err = XXH3_64bits_reset(xxstate);
    hash_bytes = (void *)xxh3_64bits_hash_bytes;
  } else if (strcmp(algo, "xxh32") == 0) {
    XXH32_state_t *xxstate = XXH32_createState();
    ser_state.xxstate = (void *)xxstate;
    err = XXH32_reset(xxstate, 0);
    hash_bytes = (void *)xxh32_hash_bytes;
  } else if (strcmp(algo, "xxh64") == 0) {
    XXH64_state_t *xxstate = XXH64_createState();
    ser_state.xxstate = (void *)xxstate;
    err = XXH64_reset(xxstate, 0);
    hash_bytes = (void *)xxh64_hash_bytes;
  } else {
    error("Nope: algo = %s\n", algo);
  }

  if (err == XXH_ERROR) {
    error("xxhash_(): Unknown algorithm '%s'", algo);
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
    (R_pstream_data_t) &ser_state, // The "location" to write to
    R_pstream_binary_format,    // Store as binary
    3,                          // Version = 3 for R >3.5.0 See `?base::serialize`
    hash_byte,                  // Function to write single byte to buffer
    (void (*)(R_outpstream_t, void *, int))hash_bytes,                 // Function for writing multiple bytes to buffer
    NULL,                       // Func for special handling of reference data.
    R_NilValue                  // Data related to reference data handling
  );

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Serialize the object into the output_stream
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  R_Serialize(robj_, &output_stream);

  SEXP res_ = R_NilValue;

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Produce the final hash value
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (strcmp(algo, "xxh128") == 0) {
    XXH128_hash_t const hash = XXH3_128bits_digest(ser_state.xxstate);
    XXH3_freeState(ser_state.xxstate);
    res_ = PROTECT(xxh128_hash_to_robj(hash, as_raw_));
  } else if (strcmp(algo, "xxh3") == 0) {
    XXH64_hash_t const hash = XXH3_64bits_digest(ser_state.xxstate);
    XXH3_freeState(ser_state.xxstate);
    res_ = PROTECT(xxh64_hash_to_robj(hash, as_raw_));
  } else if (strcmp(algo, "xxh32") == 0) {
    XXH32_hash_t const hash = XXH32_digest(ser_state.xxstate);
    XXH32_freeState(ser_state.xxstate);
    res_ = PROTECT(xxh32_hash_to_robj(hash, as_raw_));
  } else if (strcmp(algo, "xxh64") == 0) {
    XXH64_hash_t const hash = XXH64_digest(ser_state.xxstate);
    XXH64_freeState(ser_state.xxstate);
    res_ = PROTECT(xxh64_hash_to_robj(hash, as_raw_));
  } 

  UNPROTECT(1);
  return res_;
}
