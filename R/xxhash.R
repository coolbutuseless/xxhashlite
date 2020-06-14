

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Calculate hash of the data in a raw, numeric, integer or logical vector using \code{xxHash}
#'
#' This function will calculate
#' the hash on the data payload within a raw, integer, numeric or logical vector.
#' It disregards dimensions and other attributes.
#'
#' Vectors, matrices and arrays which contain the same data will hash to the same
#' value regardless of dimensions or attributes.
#'
#' @param vec vector, matrix or array of raw, real, integer or logical values
#'
#' @useDynLib xxhashlite xxhash64_
#' @useDynLib xxhashlite xxhash32_
#' @useDynLib xxhashlite xxhash128_
#' @useDynLib xxhashlite xxh3_64bits_
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
xxhash64 <- function(vec) {
  .Call(xxhash64_, vec)
}

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' @rdname xxhash64
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
xxhash32 <- function(vec) {
  .Call(xxhash32_, vec)
}

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' @rdname xxhash64
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
xxhash128 <- function(vec) {
  .Call(xxhash128_, vec)
}

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' @rdname xxhash64
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
xxh3_64bits <- function(vec) {
  .Call(xxh3_64bits_, vec)
}
