
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
