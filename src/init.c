
#include <R.h>
#include <Rinternals.h>

SEXP xxhash_     (SEXP robj_, SEXP algo_, SEXP as_raw_);
SEXP xxhash_raw_ (SEXP robj_, SEXP algo_, SEXP as_raw_);
SEXP xxhash_file_(SEXP file_, SEXP algo_, SEXP as_raw_);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// .C      R_CMethodDef
// .Call   R_CallMethodDef
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static const R_CallMethodDef CEntries[] = {
  {"xxhash_"     , (DL_FUNC) &xxhash_     , 3},
  {"xxhash_raw_" , (DL_FUNC) &xxhash_raw_ , 3},
  {"xxhash_file_", (DL_FUNC) &xxhash_file_, 3},
  {NULL, NULL, 0}
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Register the methods
//
// Change the '_simplecall' suffix to match your package name
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void R_init_xxhashlite(DllInfo *info) {
  R_registerRoutines(
    info,      // DllInfo
    NULL,      // .C
    CEntries,  // .Call
    NULL,      // Fortran
    NULL       // External
  );
  R_useDynamicSymbols(info, FALSE);
}
