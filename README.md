
<!-- README.md is generated from README.Rmd. Please edit that file -->

# xxhashlite

<!-- badges: start -->

![](https://img.shields.io/badge/cool-useless-green.svg)
[![R-CMD-check](https://github.com/coolbutuseless/xxhashlite/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/coolbutuseless/xxhashlite/actions/workflows/R-CMD-check.yaml)
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

Supports all hashes provided by `xxHash` i.e. XXH128, XXH3, XXH32 and
XXH64.

## Installation

You can install from
[GitHub](https://github.com/coolbutuseless/xxhashlite) with:

``` r
# install.package('remotes')
remotes::install_github('coolbutuseless/xxhashlite)
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

## Acknowledgements

- Yann Collett for releasing, maintaining and advancing
  [xxHash](https://cyan4973.github.io/xxHash/)
- R Core for developing and maintaining such a great language.
- CRAN maintainers, for patiently shepherding packages onto CRAN and
  maintaining the repository
