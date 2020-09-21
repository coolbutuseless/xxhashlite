
<!-- README.md is generated from README.Rmd. Please edit that file -->

# xxhashlite

<!-- badges: start -->

![](https://img.shields.io/badge/cool-useless-green.svg) [![Lifecycle:
maturing](https://img.shields.io/badge/lifecycle-maturing-blue.svg)](https://www.tidyverse.org/lifecycle/#maturing)
[![R build
status](https://github.com/coolbutuseless/xxhashlite/workflows/R-CMD-check/badge.svg)](https://github.com/coolbutuseless/xxhashlite/actions)
<!-- badges: end -->

`xxhashlite` provides simple access to the *extremely* fast hashing
functions in [xxHash](https://cyan4973.github.io/xxHash/) for in-memory
hashing of R objects.

This new version of `xxhashlite` (v0.2.0) is a major update which now
offers fast hashing of *any* R object by internally leveraging R’s
serialization capabilities.

This package is a wrapper around [xxHash
v0.8.0](https://github.com/Cyan4973/xxHash).  
See `LICENSE-xxHash` for the copyright and licensing information for
that code. With this latest version of xxHash, the new (even faster)
hash functions, `xxh3_64bits` and `xxhash128`, are considered stable.

## What’s in the box

  - `xxhash(robj, algo)` calculates the hash of any R object understood
    by `base::serialize()`.

## Installation

You can install from
[GitHub](https://github.com/coolbutuseless/xxhashlite) with:

``` r
# install.package('remotes')
remotes::install_github('coolbutuseless/xxhashlite)
```

#### Installation - set CFLAGs for optimised executable

To get the most out of what `xxHash` offers, it will be important to set
some optimization flags for your machine. The important compiler flags
to set are `-O3` and `-march=native`.

Here are 2 possible ways to do this:

1.  Copy `src/Makevars.custom` to `src/Makevars` re-build package.
2.  Edit your `~/.R/Makevars` to include `CFLAGS = -O3 -march=native`
    (this will change flags for all future compilation, and should
    probably be used with caution)

## Why use a hash?

A hash is a way of succinctly summarising the contents of an object in a
compact format.

If there are changes to the object (no matter how small) then the hash
should change as well.

``` r
library(xxhashlite)
xxhash(mtcars)
#> [1] "2c8a35b061878544"

# Small changes results in a different hash
mtcars$cyl[1] <- 0
xxhash(mtcars)
#> [1] "06a3bba3891cfe7e"
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

<summary> Click to show/hide the benchmarking code </summary>

``` r
library(xxhashlite)
library(digest)
library(fastdigest)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Simple data.frame
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N   <- 1e5
df  <- data.frame(
  x = runif(N),
  y = sample(N)
)

size <- pryr::object_size(df)
#> Registered S3 method overwritten by 'pryr':
#>   method      from
#>   print.bytes Rcpp
size
#> 1.2 MB


res <- bench::mark(
  # {xxhashlite}
  xxhash(df, 'xxhash32'),
  xxhash(df, 'xxhash64'),
  xxhash(df, 'xxhash128'),
  xxhash(df, 'xxh3_64bits'),
  
  # {digest}
  digest(df, algo = 'xxhash32'),
  digest(df, algo = 'xxhash64'),
  digest(df, algo = 'murmur32'),
  digest(df, algo = 'spookyhash'),
  
  # {fastdigest}
  fastdigest(df),
  
  check = FALSE
)
```

</details>

| package    | expression                      |   median | itr/sec |    MB/s |
| :--------- | :------------------------------ | -------: | ------: | ------: |
| xxhashlite | xxhash(df, “xxhash32”)          | 193.74µs |    4934 |  5911.0 |
| xxhashlite | xxhash(df, “xxhash64”)          | 197.31µs |    4845 |  5804.2 |
| xxhashlite | xxhash(df, “xxhash128”)         |   54.5µs |   17580 | 21014.3 |
| xxhashlite | xxhash(df, “xxh3\_64bits”)      |   53.1µs |   17647 | 21567.6 |
| digest     | digest(df, algo = “xxhash32”)   |   4.34ms |     229 |   264.0 |
| digest     | digest(df, algo = “xxhash64”)   |   4.06ms |     243 |   281.8 |
| digest     | digest(df, algo = “murmur32”)   |   4.47ms |     219 |   256.1 |
| digest     | digest(df, algo = “spookyhash”) | 165.21µs |    5763 |  6932.0 |
| fastdigest | fastdigest(df)                  | 202.34µs |    4774 |  5659.7 |

Hashing a simple data.frame

<img src="man/figures/README-unnamed-chunk-4-1.png" width="100%" />

## Related Software

  - The original [xxHash](https://cyan4973.github.io/xxHash/) software.
  - The [`digest`](https://cran.r-project.org/package=digest) package is
    a much more comprehensive approach to hashing from within R -
    multiple hashing functions and much more configurability
  - The
    [`hashFunction`](https://cran.r-project.org/package=hashFunction)
    package offers some hasing functions that work directly on character
    or integer values.
  - The [`fastdigest`](https://cran.r-project.org/package=fastdigest)
    offers the fast non-cryptographic ‘SpookyHash’ and will hash
    anything that serialize knows about.

## Acknowledgements

  - Yann Collett for releasing, maintaining and advancing
    [xxHash](https://cyan4973.github.io/xxHash/)
  - R Core for developing and maintaining such a great language.
  - CRAN maintainers, for patiently shepherding packages onto CRAN and
    maintaining the repository
