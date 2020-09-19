test_that("hashing of complex values works", {

  # A complex is just 2 doubles in a row.
  com <- complex(real = 1:2, imaginary = 3:4)
  dbl <- c(1, 3, 2, 4)


  expect_identical(xxhash_vec(com, 'xxhash32'   ), xxhash_vec(dbl, 'xxhash32'   ))
  expect_identical(xxhash_vec(com, 'xxhash64'   ), xxhash_vec(dbl, 'xxhash64'   ))
  expect_identical(xxhash_vec(com, 'xxhash128'  ), xxhash_vec(dbl, 'xxhash128'  ))
  expect_identical(xxhash_vec(com, 'xxh3_64bits'), xxhash_vec(dbl, 'xxh3_64bits'))

})
