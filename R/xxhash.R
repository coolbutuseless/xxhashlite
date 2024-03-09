


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Calculate the hash of an arbitrary R object.
#'
#' This function will calculate the hash of any object understood by
#' \code{base::serialize()}.
#'
#' @param robj Any R object
#' @param algo Select the specific xxhash algorithm. Default: 'xxh3_64bits'.
#'        (the latest algorithm in the xxhash family)
#'        Valid values: 'xxhash32', 'xxhash64', 'xxhash128', 'xxh3_64bits'
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
xxhash <- function(robj, algo = 'xxh3_64bits') {
  .Call(xxhash_, robj, algo)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Calculate the hash of a raw vector or string
#' 
#' @inheritParams xxhash
#' @param robj raw vector or single character string
#' 
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
xxhash_raw <- function(robj, algo = 'xxh3_64bits') {
  .Call(xxhash_raw_, robj, algo)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Calculate the hash of a file
#' 
#' @inheritParams xxhash_raw
#' @param file filename
#' 
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
xxhash_file <- function(file, algo = 'xxh3_64bits') {
  .Call(xxhash_file_, file, algo)
}




if (FALSE) {
  v <- c(1L, 2L, 3L, 4L)
  rlang::hash(v)
 
  sv <- serialize(v, NULL, xdr = FALSE)
  
  xxhash_raw(sv[-c(1:23)], algo = 'xxhash128') |> print()
}