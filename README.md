
<!-- README.md is generated from README.Rmd. Please edit that file -->

# xxhashlite

<!-- badges: start -->

![](https://img.shields.io/badge/cool-useless-green.svg)
<!-- badges: end -->

`xxhashlite` provides simple access to the *extremely* fast hashing
functions in [xxHash](https://cyan4973.github.io/xxHash/) for in-memory
hashing of R atomic, numeric vectors.

The scope of this package is limited - it aims to provide functions for
direct, in-memory hashing of vectors which contain raw, integer, real,
complex or logical values.

If you wanted to hash arbitrary R objects, you could first turn it into
a raw vector representation using `base::serialize()`.

Currently code provided with this package is **xxHash v0.8.0**. See
`LICENSE-xxHash` for the copyright and licensing information for that
code.

### Design choices

  - `xxhashlite` will hash the *data payload* within an object, and not
    the R object itself. This means that hashes calculated within R will
    match hashes calculated on the equivalent data in a file hashed by
    the command line programs provided with `xxHash`.

### Limitations

  - As it is the *data payload* of the vector or matrix that is being
    hashed, this does not include any notion of the container for that
    data. This means that a vector and array which contain the same data
    will hash to the same value - regardless of the dimensions or other
    attributes.
  - `xxHash v0.8.0` has stabilized the API for `xxh3` and `xxh128` hash
    functions. While future xxh3 and xxh128 hashes should remain
    identical for identical data, they will be different from hashes
    generated in v0.7.x where they were still marked as experimental
  - `xxHash` is a non-cryptographic hash.

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

## Simple hashing of raw, integer, real, complex and logical vectors

The supplied functions operate directly on the payload within vectors,
matrices or arrays containing raw, integer, real, complex or logical
values.

Because these functions operate on the raw data contained in the vector
and not the R object itself, the attributes and dimensions of the object
are not included in the hash.

By not having to serialize the R object first, overall hashing time is
reduced. The price paid for this however is that a vector and matrix
with the same underlying values, but different dimensions, will hash to
the same value.

Four hash functions from xxHash’s “simple api” are exposed:

  - **xxhash32** - 32 bit output
  - **xxhash64** - 64 bit output
  - **xxhash128** - 128 bit output. New, fast 128bit hash. Stable as of
    xxHash v0.8.0.
  - **xxh3\_64bits** - 64 bit output. New, fast 64bit hash. Stable as of
    xxHash v0.8.0.

<!-- end list -->

``` r
library(xxhashlite)

vec <- raw(1e6)
mat <- as.array(vec, 1e3, 1e3)

xxhashlite::xxhash32(vec)
#> [1] "5fff0bcb"
xxhashlite::xxhash32(mat)
#> [1] "5fff0bcb"

xxhashlite::xxhash64(vec)
#> [1] "8a76d36d39caaecc"
xxhashlite::xxhash64(mat)
#> [1] "8a76d36d39caaecc"

xxhashlite::xxhash128(vec)
#> [1] "ef233fc372a159319d648391f361d99a"
xxhashlite::xxhash128(mat)
#> [1] "ef233fc372a159319d648391f361d99a"

xxhashlite::xxh3_64bits(vec)
#> [1] "9d648391f361d99a"
xxhashlite::xxh3_64bits(mat)
#> [1] "9d648391f361d99a"
```

## Hashing 1 million raw bytes

An R vector of raw bytes may be hashed as:

  - An R object containing opaque data e.g. Using `xxhash32()`
  - The raw data values contained in the object e.g. Using `xxhash32()`

Hashing the raw data in the object is 5x to 20x faster than hashing the
entire object. The reason for this speed difference is because when
hashing the entire R object, it must first be converted to a raw byte
representation (using `base::serialize()`) - and this conversion
operation is expensive.

When hashed as just the data payload, `xxHash` has a throughput of
around \~25 GB/s (2015 MacBook Pro), but your mileage will vary
depending on your CPU and compiler flags.

<details>

<summary> Click here to show/hide benchmark code </summary>

``` r
library(xxhashlite)

N   <- 1e6
vec <- as.raw(seq(N) %% 256)

res <- bench::mark(
  xxhash32(vec),
  xxhash64(vec),
  xxhash128(vec),
  xxh3_64bits(vec),
  check = FALSE
)
```

</details>

| package    | expression        |  median | itr/sec | GB/s |
| :--------- | :---------------- | ------: | ------: | ---: |
| xxhashlite | xxhash32(vec)     | 154.2µs |    6154 |  6.0 |
| xxhashlite | xxhash64(vec)     |  79.3µs |   12048 | 11.7 |
| xxhashlite | xxhash128(vec)    |  33.8µs |   28079 | 27.5 |
| xxhashlite | xxh3\_64bits(vec) |  33.7µs |   28155 | 27.6 |

Hashing 1 million raw bytes

## Hashing arbitrary R objects

To hash arbitrary R objects (including numeric vectors with dimensions
and other attributes), use `base::serialize()` to convert the object to
a stream of raw bytes.

``` r
xxhash64(base::serialize(mtcars, NULL))
#> [1] "64bf7020580dd24d"
```

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
