
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
#'        Default: FALSE.  If TRUE, then the raw bytes are returned in big-endian
#'        order - which is what \code{xxHash} considers the \emph{canonical} form.
#'
#' @return String representation of hash. If \code{as_raw = TRUE} then a 
#'         raw vector is returned instead.
#'         
#' @export
#'
#' @examples
#' xxhash(mtcars)
#' xxhash(mtcars, algo = 'xxh3', as_raw = TRUE)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
xxhash <- function(robj, algo = 'xxh128', as_raw = FALSE) {
  .Call(xxhash_, robj, algo, as_raw)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Calculate the hash of a raw vector or string
#' 
#' This performs a hash of the raw bytes - not of the serialized representation.
#' 
#' @inheritParams xxhash
#' @param vec raw vector or single character string
#' 
#' @return String representation of hash. If \code{as_raw = TRUE} then a 
#'         raw vector is returned instead.
#'         
#' @export
#' 
#' @examples
#' vec <- "hello"
#' xxhash_raw(vec)
#' vec <- as.raw(c(0x01, 0x02, 0x99))
#' xxhash_raw(vec)
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
#' @return String representation of hash. If \code{as_raw = TRUE} then a 
#'         raw vector is returned instead.
#'         
#' @export
#'
#' @examples
#' filename <- system.file('DESCRIPTION', package = 'base', mustWork = TRUE)
#' xxhash_file(filename)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
xxhash_file <- function(file, algo = 'xxh128', as_raw = FALSE) {
  .Call(xxhash_file_, normalizePath(file), algo, as_raw)
}

