
<!-- README.md is generated from README.Rmd. Please edit that file -->

# xxhashr

<!-- badges: start -->

![](https://img.shields.io/badge/cool-useless-green.svg)
<!-- badges: end -->

`xxhashr` provides simple access to the *very* fast hashing functions in
[xxHash](https://cyan4973.github.io/xxHash/).

The package provides support for hashing of vectors, matrices and arrays
which contain raw, integer, real or logical values. To hash arbitrary R
objects, use `base::serialize()` to first turn the object into a vector
of raw bytes.

### Limitations

  - It is the underlying data of the vector or matrix that is being
    hashed, and this does not include any notion of the container for
    that data. This means that a vector and array which contain the same
    data will hash to the same value - regardless of the dimensions.
  - `xxHash v0.7.x` includes the experimental `xxh3` and `xxh128` hash
    functionx. From the documentation: “The algorithm is currently in
    development, meaning its return values might still change in future
    versions. However, the API is stable, and can be used in production,
    typically for generation of ephemeral hashes (produced and consumed
    in same session)”.
  - `xxHash` is a non-cryptographic hash.

## Installation

You can install from [GitHub](https://github.com/coolbutuseless/xxhashr)
with:

``` r
# install.package('remotes')
remotes::install_github('coolbutuseless/xxhashr)
```

#### Optimisation via `Makevars`

To get the most out of what `xxHash` offers, it’s important to set some
optimisation flags for your machine. The important compiler flags to set
are `-O3` and `-march=native`.

Here are 2 possible ways to do this:

1.  Copy `src/Makevars.custom` to `src/Makevars` re-build package.
2.  Edit your `~/.R/Makevars` to include `CFLAGS = -O3 -march=native`
    (this will change flags for all future compilation, and should
    probably be used with caution)

## Simple hashing

``` r
library(xxhashr)

vec <- raw(1e6)

xxhashr::xxhash32(vec)
#> [1] "5fff0bcb"

xxhashr::xxhash64(vec)
#> [1] "8a76d36d39caaecc"

xxhashr::xxhash128(vec)
#> [1] "118fde282639e0a3b5ce7c14a206fb68"

xxhashr::xxh3_64bits(vec)
#> [1] "b5ce7c14a206fb68"
```

## Hashing 1 million raw bytes

`xxhashr::xxh3_64bits()` hashes data at around 25 GB/s for this input
data size.

Since `digest::digest()` can hash raw bytes like `xxhashr` the speeds
for `xxhashr::xxhash64()` and `digest(algo = 'xxhash64')` are about the
same.

<details>

<summary> Click here to show/hide benchmark code </summary>

``` r
library(digest)

N   <- 1e6
vec <- raw(N)

res <- bench::mark(
  xxhash32(vec),
  xxhash64(vec),
  xxhash128(vec),
  xxh3_64bits(vec),
  digest(vec, algo = 'xxhash32', serialize = FALSE),
  digest(vec, algo = 'xxhash64', serialize = FALSE),
  check = FALSE
)
```

</details>

| expression                                        |  median | itr/sec | GB/s |
| :------------------------------------------------ | ------: | ------: | ---: |
| xxhash32(vec)                                     | 168.9µs |    5557 |  5.5 |
| xxhash64(vec)                                     |  83.4µs |   11367 | 11.2 |
| xxhash128(vec)                                    |  35.1µs |   25999 | 26.5 |
| xxh3\_64bits(vec)                                 |  32.6µs |   27884 | 28.6 |
| digest(vec, algo = “xxhash32”, serialize = FALSE) | 319.9µs |    3018 |  2.9 |
| digest(vec, algo = “xxhash64”, serialize = FALSE) | 113.5µs |    8317 |  8.2 |

Hashing 1 million raw bytes

## Hashing 1 thousand raw bytes

At smaller data sizes, the call overhead becomes more significant, and
the overall throughput drops drastically.

`xxhashr` is about 20x faster than `digest::digest()` in this case.

<details>

<summary> Click here to show/hide benchmark code </summary>

``` r
N <- 1024L
vec <- raw(N)

res <- bench::mark(
  xxhash32(vec),
  xxhash64(vec),
  xxhash128(vec),
  xxh3_64bits(vec),
  digest(vec, algo = 'xxhash32', serialize = FALSE),
  digest(vec, algo = 'xxhash64', serialize = FALSE),
  check = FALSE
)
```

</details>

| expression                                        |  median | itr/sec |  GB/s |
| :------------------------------------------------ | ------: | ------: | ----: |
| xxhash32(vec)                                     |  1.17µs |  776164 | 0.814 |
| xxhash64(vec)                                     |  1.06µs |  816297 | 0.903 |
| xxhash128(vec)                                    |  1.09µs |  855641 | 0.873 |
| xxh3\_64bits(vec)                                 |  1.02µs |  886907 | 0.933 |
| digest(vec, algo = “xxhash32”, serialize = FALSE) | 31.69µs |   29108 | 0.030 |
| digest(vec, algo = “xxhash64”, serialize = FALSE) | 31.42µs |   29164 | 0.030 |

Hashing 1000 raw bytes

## Hashing 1 million numeric values

In the case of integers, doubles and logical values, `xxhashr` will hash
the data values directly, while `digest::digest()` must first serialize
them to raw bytes.

As a result, `xxhashr::xxhash128()` is about 100x faster than
`digest::digest()`, and operates at about 20 GB/s (on my machine).

<details>

<summary> Click here to show/hide benchmark code </summary>

``` r
N   <- 1e6
vec <- numeric(N)

res <- bench::mark(
  xxhash32(vec),
  xxhash64(vec),
  xxhash128(vec),
  xxh3_64bits(vec),
  digest(vec, algo = 'xxhash32', serialize = TRUE),
  digest(vec, algo = 'xxhash64', serialize = TRUE),
  check = FALSE
)
```

</details>

| expression                                       |   median | itr/sec | GB/s |
| :----------------------------------------------- | -------: | ------: | ---: |
| xxhash32(vec)                                    |   1.37ms |     689 |  5.4 |
| xxhash64(vec)                                    | 727.09µs |    1261 | 10.2 |
| xxhash128(vec)                                   | 369.55µs |    2318 | 20.2 |
| xxh3\_64bits(vec)                                | 363.57µs |    2372 | 20.5 |
| digest(vec, algo = “xxhash32”, serialize = TRUE) |  33.29ms |      29 |  0.2 |
| digest(vec, algo = “xxhash64”, serialize = TRUE) |  32.28ms |      31 |  0.2 |

Hashing 1 million numeric values

## Hashing 1 million integer values

In the case of integers, doubles and logical values, `xxhashr` will hash
the data values directly, while `digest::digest()` must first serialize
them to raw bytes.

The `hashFunction` package offers a few hashes that work directly with
integer values.

`xxhashr::xxhash128()` is about 100x faster than `digest::digest()`, and
about 5x faster than `hashFunction` hashes.

<details>

<summary> Click here to show/hide benchmark code </summary>

``` r
library(hashFunction)
N <- 1e6
vec <- integer(N)

res <- bench::mark(
  xxhash32(vec),
  xxhash64(vec),
  xxhash128(vec),
  xxh3_64bits(vec),
  digest(vec, algo = 'xxhash32', serialize = TRUE),
  digest(vec, algo = 'xxhash64', serialize = TRUE),
  murmur3.32(vec),
  cityhash.64(vec),
  spooky.32(vec),
  check = FALSE
)
```

</details>

| expression                                       |  median | itr/sec | GB/s |
| :----------------------------------------------- | ------: | ------: | ---: |
| xxhash32(vec)                                    | 659.5µs |    1474 |  5.6 |
| xxhash64(vec)                                    | 333.9µs |    2847 | 11.2 |
| xxhash128(vec)                                   | 140.5µs |    6505 | 26.5 |
| xxh3\_64bits(vec)                                | 134.2µs |    6501 | 27.8 |
| digest(vec, algo = “xxhash32”, serialize = TRUE) |  13.6ms |      71 |  0.3 |
| digest(vec, algo = “xxhash64”, serialize = TRUE) |  11.9ms |      79 |  0.3 |
| murmur3.32(vec)                                  | 572.8µs |    1460 |  6.5 |
| cityhash.64(vec)                                 | 629.3µs |     934 |  5.9 |
| spooky.32(vec)                                   |   622µs |    1085 |  6.0 |

Hashing 1 million integer values

## Related Software

  - The original [xxHash](https://cyan4973.github.io/xxHash/) software.
  - The [`digest`](https://cran.r-project.org/package=digest) package is
    a much more comprehensive approach to hashing from within R -
    multiple hashing functions and much more configurability
  - The
    [`hashFunction`](https://cran.r-project.org/package=hashFunction)
    offers some hasing functions that work directly on character or
    integer values.

## Acknowledgements

  - Yann Collett for releasing, maintaining and advancing
    [xxHash](https://cyan4973.github.io/xxHash/)
  - R Core for developing and maintaining such a great language.
  - CRAN maintainers, for patiently shepherding packages onto CRAN and
    maintaining the repository
