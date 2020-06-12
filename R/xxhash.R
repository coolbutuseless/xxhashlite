

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Calculate hash of the given numeric, integer or logical vector using \code{xxHash}
#'
#' Vectors, matrices and arrays which contain the same data will hash to the same
#' value regardless of dimensions.
#'
#' @param vec vector of real, integer or logical values
#'
#' @useDynLib xxhashr xxhash64_
#' @useDynLib xxhashr xxhash32_
#' @useDynLib xxhashr xxhash128_
#' @useDynLib xxhashr xxh3_64bits_
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

