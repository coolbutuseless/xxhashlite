
<!-- README.md is generated from README.Rmd. Please edit that file -->

# xxhashlite

<!-- badges: start -->

![](https://img.shields.io/badge/cool-useless-green.svg)
[![R-CMD-check](https://github.com/coolbutuseless/xxhashlite/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/coolbutuseless/xxhashlite/actions/workflows/R-CMD-check.yaml)
[![CRAN](https://www.r-pkg.org/badges/version/xxhashlite)](https://CRAN.R-project.org/package=xxhashlite)
<!-- badges: end -->

`xxhashlite` provides access to the *extremely* fast hashing functions
in [xxHash](https://cyan4973.github.io/xxHash/) for in-memory hashing of
R objects, files, strings and raw vectors.

This package is a wrapper around [xxHash
v0.8.2](https://github.com/Cyan4973/xxHash).  
See `inst/LICENSE-xxHash` for the copyright and licensing information
for that code.

## Notes

- Only supports R versions \>= v3.5.0 as this is when the serialization
  infrastructure had a breaking change, and this package will not
  support the old version.

## What’s in the box

- `xxhash(robj, algo, as_raw)` calculates the hash of any R object
  understood by `base::serialize()`.
- `xxhash_raw(vec, algo, as_raw)` calculates the hash of a raw vector or
  string. This function is appropriate when comparing hashes of non-R
  objects e.g.  a checksum hash of raw bytes.
- `xxhash_file(file, algo, as_raw)` calculates the hash of a file
- `xxhash_con(con, algo, as_raw)` calculations the hash of data from a
  connection e.g. `gzfile()`, `url()`

Supports all hashes provided by `xxHash` i.e. XXH128, XXH3, XXH32 and
XXH64.

## Installation

This package can be installed from CRAN

``` r
install.packages('xxhashlite')
```

You can install the latest development version from
[GitHub](https://github.com/coolbutuseless/xxhashlite) with:

``` r
# install.package('remotes')
remotes::install_github('coolbutuseless/xxhashlite')
```

Pre-built source/binary versions can also be installed from
[R-universe](https://r-universe.dev)

``` r
install.packages('xxhashlite', repos = c('https://coolbutuseless.r-universe.dev', 'https://cloud.r-project.org'))
```

## Why use a hash?

A hash is a way of succinctly summarising the contents of an object in a
compact format.

If there are changes to the object (no matter how small) then the hash
should see drastic changes.

``` r
library(xxhashlite)
xxhash(mtcars)
#> [1] "d0487363db4e6cc64fdb740cb6617fc0"

# Small changes results in a different hash
mtcars$cyl[1] <- 0
xxhash(mtcars)
#> [1] "e999db3ed8f21dc2cd52b97a08f0c9f5"
```

## Available algorithms

`{xxhashlite}` supports returning the hash as a raw vector, and all the
xxHash algorithms are available:

``` r
xxhash(mtcars, as_raw = TRUE)
#>  [1] e9 99 db 3e d8 f2 1d c2 cd 52 b9 7a 08 f0 c9 f5
xxhash(mtcars, algo = 'xxh3')
#> [1] "cd52b97a08f0c9f5"
xxhash(mtcars, algo = 'xxh64')
#> [1] "fe3d463a549e63ce"
xxhash(mtcars, algo = 'xxh32')
#> [1] "eaa9d7fb"
```

## Strings and Raw Vectors

`xxhash()` uses R’s serialization mechanism to handle any R object.

When you only require a hash of the raw contents of a file, a raw vector
or a string, then use `xxhash_raw()` and `xxhash_file()`.

Rather than serializing the data first, the hash is calculated on the
raw bytes.

## Acknowledgements

- Yann Collett for releasing, maintaining and advancing
  [xxHash](https://cyan4973.github.io/xxHash/)
- R Core for developing and maintaining such a great language.
- CRAN maintainers, for patiently shepherding packages onto CRAN and
  maintaining the repository
