


test_that("xxhash() works", {
  
  dat <- list("test", 1:3, c(10, 11, 12))
  
  expect_identical(xxhash(dat, algo="xxh128"), "c2fc4e260dddb5b49271720838923d3a")
  expect_identical(xxhash(dat, algo="xxh32" ), "ba28622d")
  expect_identical(xxhash(dat, algo="xxh64" ), "c6ba5f15dd3a1ea6")
  expect_identical(xxhash(dat, algo="xxh3"  ), "15bad8eb82ec503f")
  
})
