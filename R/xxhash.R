


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Calculate the hash of an arbitrary R object.
#'
#' This function will calculate the hash of any object understood by
#' \code{base::serialize()}.
#'
#' @param robj Any R object
#' @param algo Select the specific xxhash algorithm. Default: 'xxh128'.
#'        (the latest algorithm in the xxhash family)
#'        Valid values: 'xxh32', 'xxh64', 'xxh128', 'xxh3'
#' @param as_raw Return the hash as a raw vector of bytes instead of string?
#'        Default: FALSE
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
xxhash <- function(robj, algo = 'xxh128', as_raw = FALSE) {
  .Call(xxhash_, robj, algo, as_raw)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Calculate the hash of a raw vector or string
#' 
#' @inheritParams xxhash
#' @param vec raw vector or single character string
#' 
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
xxhash_raw <- function(vec, algo = 'xxh128', as_raw = FALSE) {
  .Call(xxhash_raw_, vec, algo, as_raw)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Calculate the hash of a file
#' 
#' @inheritParams xxhash_raw
#' @param file filename
#' 
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
xxhash_file <- function(file, algo = 'xxh128', as_raw = FALSE) {
  .Call(xxhash_file_, file, algo, as_raw)
}




if (FALSE) {
  v <- c(1L, 2L, 3L, 4L)
  v <- "hello"
  rlang::hash(v)
 
  sv <- serialize(v, NULL, xdr = FALSE)
  
  xxhash_raw(sv[-c(1:23)]) |> print()
}