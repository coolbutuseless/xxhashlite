

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









if (FALSE) {

  N <- 1048576L
  # N <- 104800L
  vec <- raw(N)
  xxhash64(vec)
  xxhash32(vec)
  xxhash128(vec)
  xxh3_64bits(vec)

  library(digest)

  b <- bench::mark(
    xxhash32(vec),
    xxhash64(vec),
    xxhash128(vec),
    xxh3_64bits(vec),
    digest(vec, algo = 'xxhash64', serialize = FALSE),
    check = FALSE
  )
  b

  dbl_vec <- rnorm(N)

  b <- bench::mark(
    xxhash64(dbl_vec),
    xxh3_64bits(dbl_vec),
    digest(dbl_vec, algo = 'xxhash64', serialize = TRUE),
    check = FALSE
  )
  b


  bench::mark(
    xxhash64(serialize(dbl_vec, connection = NULL)),
    digest(dbl_vec, algo = 'xxhash64', serialize = TRUE),
    check = FALSE
  )



}


if (FALSE) {
  N <- as.integer(1e7)
  set.seed(1); m1 <- rnorm(N)
  set.seed(1); m2 <- rnorm(N)

  bench::mark(
    identical(xxhash64(m1), xxhash64(m2)),
    identical(xxh3_64bits(m1), xxh3_64bits(m2)),
    xxh3_64bits(m1) == xxh3_64bits(m2),
    identical(m1, m2)
  )
}











