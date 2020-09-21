


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


