test_that("hashing of complex values works", {

  # A complex is just 2 doubles in a row.
  com <- complex(real = 1:2, imaginary = 3:4)
  dbl <- c(1, 3, 2, 4)

  expect_identical(xxhash32(com), xxhash32(dbl))
  expect_identical(xxhash64(com), xxhash64(dbl))
  expect_identical(xxhash128(com), xxhash128(dbl))
  expect_identical(xxh3_64bits(com), xxh3_64bits(dbl))

})
