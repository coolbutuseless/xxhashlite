

# xxhashlite 0.2.2  2024-03-11

* Rename hash `algo` options to better align with official documentation
* Skip serialization header so results for `algo = xxh128` match `rlang::hash()`
* Hash files, strings and raw vector contents directly (i.e. not using
  serialization)
* Return hash as string or raw vector

# xxhashlite 0.2.1.9000  2024-03-09

* Update vendored 'xxHash' library to version 0.8.2

# xxhashlite 0.2.1 2020-08-22

* Remove `xxhash_vec()` is now redundant.  It is slightly faster than `xxhash`
  on vanilla vector inputs, but not useful enough generally to maintain.

# xxhashlite 0.2.0 2020-09-19

* Refactor: Consolidate hash functions for atomic vectors into a single call with an 
  `algo` argument.
* Feature: Calculation hashes on any R object understood by `base::serialize()`

# xxhashlite 0.1.2

* Update to xxHash v0.8.0

# xxhashlite 0.1.1

* Added support for hashing of vectors of complex numbers

# xxhashlite 0.1.0

* Initial release.
