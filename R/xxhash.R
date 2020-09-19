

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Calculate the hash of the numeric data within an atomic vector.
#'
#' This function calculates the hash of only the data within an atomic vector, and
#' ignores all attributes (including dimensions).  By working in this way, it
#' avoids having to serialize the vector into raw bytes, and thus gains a speed
#' advantage.
#'
#' The limitation of this method is that the same data in a vector will hash
#' to the same data stored in an array, even though the structure
#' of the data is different.
#'
#' For general hashing of any R object, see
#' \code{\link[xxhashlite:xxhash]{xxhashlite::xxhash()}}
#' attributes and dimensions in hashing calculations.
#'
#' @param vec atomic vector (including matrices and arrays) containing raw,
#'        integer, numeric, complex or logical values only.
#' @param algo Select the specific xxhash algorithm. Default: 'xxh3_64bits'.
#'        (the latest algorithm in the xxhash family)
#'        Valid values: 'xxhash32', 'xxhash64', 'xxhash128', 'xxh3_64bits'
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
xxhash_vec <- function(vec, algo = 'xxh3_64bits') {
  .Call(xxhash_vec_, vec, algo);
}




#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Calculate the hash of an arbitrary R object.
#'
#' This function will calculate the hash of any object understood by
#' \code{base::serialize()}.
#'
#' If you are only interested in hashing the data contents of atomic vectors
#' containing numeric values, then
#' \code{\link[xxhashlite:xxhash_vec]{xxhashlite::xxhash_vec()}}
#' offers a speed advantage
#' over this function.
#'
#' @inheritParams  xxhash_vec
#' @param robj Any R object
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
xxhash <- function(robj, algo = 'xxh3_64bits') {
  .Call(xxhash_, robj, algo)
}


