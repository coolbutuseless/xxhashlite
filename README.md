
<!-- README.md is generated from README.Rmd. Please edit that file -->

# xxhashlite

<!-- badges: start -->

![](https://img.shields.io/badge/cool-useless-green.svg)
<!-- badges: end -->

`xxhashlite` provides simple access to the *very* fast hashing functions
in [xxHash](https://cyan4973.github.io/xxHash/) for in-memory hashing of
R atomic, numeric vectors.

The package provides support for hashing of vectors, matrices and arrays
which contain raw, integer, real or logical values. To hash arbitrary R
objects, use `base::serialize()` to first turn the object into a vector
of raw bytes.

Currently xxHash code provided with this package is v0.7.3

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

You can install from
[GitHub](https://github.com/coolbutuseless/xxhashlite) with:

``` r
# install.package('remotes')
remotes::install_github('coolbutuseless/xxhashlite)
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

Four hash functions from xxHash’s “simple api” are exposed:

  - **xxhash32** - 32 bit output. Will be slow on a 64bit machine.
  - **xxhash64** - 64 bit output. About 2x faster than xxhash32
  - **xxhash128** - 128 bit output. Still marked as experimental in
    xxHash
  - **xxh3** - 64 bit output. Still marked as experimental in xxHash

<!-- end list -->

``` r
library(xxhashlite)

vec <- raw(1e6)

xxhashlite::xxhash32(vec)
#> [1] "5fff0bcb"

xxhashlite::xxhash64(vec)
#> [1] "8a76d36d39caaecc"

xxhashlite::xxhash128(vec)
#> [1] "118fde282639e0a3b5ce7c14a206fb68"

xxhashlite::xxh3_64bits(vec)
#> [1] "b5ce7c14a206fb68"
```

## Hashing 1 million raw bytes

`xxhashlite::xxh3_64bits()` hashes data at around 25 GB/s for this input
data size.

Since `digest::digest()` can hash raw bytes like `xxhashlite` the speeds
for `xxhashlite::xxhash64()` and `digest(algo = 'xxhash64')` are about
the same.

There are some cryptographic hashes (from `blake3` and `openssl`)
included in this benchmark. These are *cryptographic* hashes, so this
isn’t really a fair comparison, but for those familiar with those hashes
it’s a good yardstick by which to measure the performace of `xxHash`

<details>

<summary> Click here to show/hide benchmark code </summary>

``` r
library(digest)
library(blake3)
library(openssl)
#> 
#> Attaching package: 'openssl'
#> The following object is masked from 'package:digest':
#> 
#>     sha1
library(fastdigest)

N   <- 1e6
vec <- raw(N)

res <- bench::mark(
  xxhash32(vec),
  xxhash64(vec),
  xxhash128(vec),
  xxh3_64bits(vec),
  blake3_hash_raw(vec),
  sha1(vec),
  sha2(vec),
  sha224(vec),
  sha256(vec),
  sha384(vec),
  sha512(vec),
  digest(vec, algo = 'xxhash32', serialize = FALSE),
  digest(vec, algo = 'xxhash64', serialize = FALSE),
  fastdigest(vec),
  check = FALSE
)
```

</details>

| package    | expression                                        |   median | itr/sec | GB/s |
| :--------- | :------------------------------------------------ | -------: | ------: | ---: |
| xxhashlite | xxhash32(vec)                                     | 158.59µs |    6047 |  5.9 |
| xxhashlite | xxhash64(vec)                                     |  81.64µs |   11614 | 11.4 |
| xxhashlite | xxhash128(vec)                                    |     34µs |   26836 | 27.4 |
| xxhashlite | xxh3\_64bits(vec)                                 |  32.16µs |   28231 | 29.0 |
| blake3     | blake3\_hash\_raw(vec)                            | 339.45µs |    2799 |  2.7 |
| openssl    | sha1(vec)                                         |   1.18ms |     833 |  0.8 |
| openssl    | sha2(vec)                                         |   2.67ms |     372 |  0.3 |
| openssl    | sha224(vec)                                       |   2.66ms |     345 |  0.3 |
| openssl    | sha256(vec)                                       |   2.64ms |     376 |  0.4 |
| openssl    | sha384(vec)                                       |    1.8ms |     549 |  0.5 |
| openssl    | sha512(vec)                                       |   1.79ms |     551 |  0.5 |
| digest     | digest(vec, algo = “xxhash32”, serialize = FALSE) | 320.84µs |    3023 |  2.9 |
| digest     | digest(vec, algo = “xxhash64”, serialize = FALSE) | 114.46µs |    8146 |  8.1 |
| fastdigest | fastdigest(vec)                                   | 186.02µs |    5135 |  5.0 |

Hashing 1 million raw bytes

## Hashing 1 thousand raw bytes

At smaller data sizes, the call overhead becomes more significant, and
the overall throughput drops drastically.

`xxhashlite` is about 20x faster than `digest::digest()` in this case.

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
  fastdigest(vec),
  check = FALSE
)
```

</details>

| package    | expression                                        |  median | itr/sec |  GB/s |
| :--------- | :------------------------------------------------ | ------: | ------: | ----: |
| xxhashlite | xxhash32(vec)                                     |  1.17µs |  710882 | 0.818 |
| xxhashlite | xxhash64(vec)                                     |   984ns |  891140 | 0.969 |
| xxhashlite | xxhash128(vec)                                    |  1.08µs |  836998 | 0.885 |
| xxhashlite | xxh3\_64bits(vec)                                 | 945.5ns |  971122 | 1.009 |
| digest     | digest(vec, algo = “xxhash32”, serialize = FALSE) | 31.86µs |   28663 | 0.030 |
| digest     | digest(vec, algo = “xxhash64”, serialize = FALSE) | 31.88µs |   28594 | 0.030 |
| fastdigest | fastdigest(vec)                                   | 12.83µs |   71074 | 0.074 |

Hashing 1000 raw bytes

## Hashing 1 million numeric values

In the case of integers, doubles and logical values, `xxhashlite` will
hash the data values directly, while `digest::digest()` must first
serialize them to raw bytes.

As a result, `xxhashlite::xxhash128()` is about 100x faster than
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
  fastdigest(vec),
  check = FALSE
)
```

</details>

| package    | expression                                       |   median | itr/sec | GB/s |
| :--------- | :----------------------------------------------- | -------: | ------: | ---: |
| xxhashlite | xxhash32(vec)                                    |   1.36ms |     690 |  5.5 |
| xxhashlite | xxhash64(vec)                                    | 702.16µs |    1292 | 10.6 |
| xxhashlite | xxhash128(vec)                                   | 341.21µs |    2507 | 21.8 |
| xxhashlite | xxh3\_64bits(vec)                                | 327.01µs |    2628 | 22.8 |
| digest     | digest(vec, algo = “xxhash32”, serialize = TRUE) |  36.16ms |      27 |  0.2 |
| digest     | digest(vec, algo = “xxhash64”, serialize = TRUE) |  34.35ms |      29 |  0.2 |
| fastdigest | fastdigest(vec)                                  |   1.41ms |     675 |  5.3 |

Hashing 1 million numeric values

## Hashing 1 million integer values

In the case of integers, doubles and logical values, `xxhashlite` will
hash the data values directly, while `digest::digest()` must first
serialize them to raw bytes.

The `hashFunction` package offers a few hashes that work directly with
integer values.

`xxhashlite::xxhash128()` is about 100x faster than `digest::digest()`,
and about 5x faster than `hashFunction` hashes.

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
  fastdigest(vec),
  murmur3.32(vec),
  cityhash.64(vec),
  spooky.32(vec),
  check = FALSE
)
```

</details>

| package      | expression                                       |  median | itr/sec | GB/s |
| :----------- | :----------------------------------------------- | ------: | ------: | ---: |
| xxhashlite   | xxhash32(vec)                                    | 680.9µs |    1397 |  5.5 |
| xxhashlite   | xxhash64(vec)                                    | 338.3µs |    2815 | 11.0 |
| xxhashlite   | xxhash128(vec)                                   | 152.2µs |    5423 | 24.5 |
| xxhashlite   | xxh3\_64bits(vec)                                | 138.2µs |    6757 | 26.9 |
| digest       | digest(vec, algo = “xxhash32”, serialize = TRUE) |  12.8ms |      75 |  0.3 |
| digest       | digest(vec, algo = “xxhash64”, serialize = TRUE) |    12ms |      83 |  0.3 |
| fastdigest   | fastdigest(vec)                                  | 708.8µs |    1369 |  5.3 |
| hashFunction | murmur3.32(vec)                                  | 567.8µs |    1514 |  6.6 |
| hashFunction | cityhash.64(vec)                                 | 730.2µs |     820 |  5.1 |
| hashFunction | spooky.32(vec)                                   | 653.1µs |    1021 |  5.7 |

Hashing 1 million integer values

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
