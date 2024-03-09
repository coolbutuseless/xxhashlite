
<!-- README.md is generated from README.Rmd. Please edit that file -->

# xxhashlite

<!-- badges: start -->

![](https://img.shields.io/badge/cool-useless-green.svg)
[![R-CMD-check](https://github.com/coolbutuseless/xxhashlite/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/coolbutuseless/xxhashlite/actions/workflows/R-CMD-check.yaml)
<!-- badges: end -->

`xxhashlite` provides simple access to the *extremely* fast hashing
functions in [xxHash](https://cyan4973.github.io/xxHash/) for in-memory
hashing of R objects.

This new version of `xxhashlite` (v0.2.0) is a major update which now
offers fast hashing of *any* R object by internally leveraging R’s
serialization capabilities.

This package is a wrapper around [xxHash
v0.8.3](https://github.com/Cyan4973/xxHash).  
See `LICENSE-xxHash` for the copyright and licensing information for
that code. With this latest version of xxHash, the new (even faster)
hash functions, `xxh3_64bits` and `xxhash128`, are considered stable.

## ToDo

- `xxhash_file()` use streaming interface to hash incrementally
- Skip first 18+n bytes when serializing - this is all very speific
  information like R_VERSION number which would change on every minor
  version bump. See `rlang::hash()` implementation for details.
- rename algorithms to:
  - `xxh32`, `xxh64`
  - `xxh3-64`, `xxh3-128`

## Notes

- Only supports R versions \>= v3.5.0 as this is when the serialization
  infrastructure had a breaking change, and this package will not
  support the old version.

## What’s in the box

- `xxhash(robj, algo)` calculates the hash of any R object understood by
  `base::serialize()`.
- `xxhash(robj, algo, seed)` calculates the hash of a raw vector or
  string. This function is appropriate when comparing hashes of non-R
  objects.

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
should change as well.

``` r
library(xxhashlite)
xxhash(mtcars)
#> [1] "5813d8235e54fe81"

# Small changes results in a different hash
mtcars$cyl[1] <- 0
xxhash(mtcars)
#> [1] "7b43291c0bc36080"
```

## Timing for hashing arbitrary R objects

`xxhashlite` uses the [xxHash](https://github.com/Cyan4973/xxHash)
family of hash functions to provide very fast hashing of R objects.

For the test case shown below, `xxhashlite` is faster at calculating a
hash than all other methods I could find, with a maximum hashing speed
in this specific case of 20 GB/s.

Note: actual hashing speed will still depend on R’s serialization
functions e.g. small complex data.frames might have a lot of
serialization overhead compared to long numeric vectors.

<details>
<summary>
Click to show/hide the benchmarking code
</summary>

``` r
library(xxhashlite)
library(digest)
library(fastdigest)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Simple data.frame
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N   <- 5e6
df  <- data.frame(
  x = runif(N),
  y = sample(N)
)

size <- pryr::object_size(df)
size
#> 60.00 MB


res <- bench::mark(
  # {xxhashlite}
  xxhash(df, 'xxh32'),
  xxhash(df, 'xxh64'),
  xxhash(df, 'xxh128'),
  xxhash(df, 'xxh3'),
  
  # {digest}
  digest(df, algo = 'xxhash32'),
  digest(df, algo = 'xxhash64'),
  digest(df, algo = 'murmur32'),
  digest(df, algo = 'spookyhash'),
  
  # {fastdigest}
  fastdigest(df),
  
  check = FALSE
)
#> Warning: Some expressions had a GC in every iteration; so filtering is
#> disabled.
```

</details>

| package    | expression                      |  median | itr/sec |    MB/s |
|:-----------|:--------------------------------|--------:|--------:|--------:|
| xxhashlite | xxhash(df, “xxh32”)             | 16.73ms |      60 |  3419.9 |
| xxhashlite | xxhash(df, “xxh64”)             |   3.8ms |     262 | 15043.0 |
| xxhashlite | xxhash(df, “xxh128”)            |  3.43ms |     289 | 16703.9 |
| xxhashlite | xxhash(df, “xxh3”)              |  3.42ms |     292 | 16725.1 |
| digest     | digest(df, algo = “xxhash32”)   | 57.85ms |      17 |   989.1 |
| digest     | digest(df, algo = “xxhash64”)   | 50.99ms |      20 |  1122.3 |
| digest     | digest(df, algo = “murmur32”)   | 71.26ms |      14 |   803.0 |
| digest     | digest(df, algo = “spookyhash”) |  4.33ms |     228 | 13218.4 |
| fastdigest | fastdigest(df)                  |  4.24ms |     235 | 13499.2 |

Hashing a simple data.frame

<img src="man/figures/README-unnamed-chunk-4-1.png" width="100%" />

## Related Software

- The original [xxHash](https://cyan4973.github.io/xxHash/) software.
- The [`digest`](https://cran.r-project.org/package=digest) package is a
  much more comprehensive approach to hashing from within R - multiple
  hashing functions and much more configurability
- The [`hashFunction`](https://cran.r-project.org/package=hashFunction)
  package offers some hasing functions that work directly on character
  or integer values.
- The [`fastdigest`](https://cran.r-project.org/package=fastdigest)
  offers the fast non-cryptographic ‘SpookyHash’ and will hash anything
  that serialize knows about.

## Acknowledgements

- Yann Collett for releasing, maintaining and advancing
  [xxHash](https://cyan4973.github.io/xxHash/)
- R Core for developing and maintaining such a great language.
- CRAN maintainers, for patiently shepherding packages onto CRAN and
  maintaining the repository
