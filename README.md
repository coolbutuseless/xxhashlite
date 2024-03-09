
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
v0.8.0](https://github.com/Cyan4973/xxHash).  
See `LICENSE-xxHash` for the copyright and licensing information for
that code. With this latest version of xxHash, the new (even faster)
hash functions, `xxh3_64bits` and `xxhash128`, are considered stable.

## Notes

- Only supports R versions \>= v3.5.0 as this is when the serialization
  infrastructure had a breaking change, and this package will not
  support the old version.

## What’s in the box

- `xxhash(robj, algo)` calculates the hash of any R object understood by
  `base::serialize()`.

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
#> Warning: Some expressions had a GC in every iteration; so filtering is
#> disabled.
```

</details>

| package    | expression                      |  median | itr/sec |    MB/s |
|:-----------|:--------------------------------|--------:|--------:|--------:|
| xxhashlite | xxhash(df, “xxhash32”)          | 16.64ms |      60 |  3439.1 |
| xxhashlite | xxhash(df, “xxhash64”)          |  3.81ms |     262 | 15022.7 |
| xxhashlite | xxhash(df, “xxhash128”)         |  3.43ms |     291 | 16686.6 |
| xxhashlite | xxhash(df, “xxh3_64bits”)       |  3.44ms |     287 | 16636.8 |
| digest     | digest(df, algo = “xxhash32”)   |  57.6ms |      17 |   993.4 |
| digest     | digest(df, algo = “xxhash64”)   |  50.1ms |      20 |  1142.2 |
| digest     | digest(df, algo = “murmur32”)   | 70.29ms |      14 |   814.1 |
| digest     | digest(df, algo = “spookyhash”) |  4.32ms |     231 | 13255.4 |
| fastdigest | fastdigest(df)                  |  4.25ms |     235 | 13473.8 |

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
