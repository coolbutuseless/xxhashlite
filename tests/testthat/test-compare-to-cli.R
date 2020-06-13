

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Run the command line xxHash over data
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if (FALSE) {
  test <- raw(1024*1024)
  writeBin(test, "test.img", size = 1)
  system('xxhsum -H0 test.img')
  system('xxhsum -H1 test.img')
  system('xxhsum -H2 test.img')

  test <- as.raw(seq(1024*1024))
  writeBin(test, "test.img", size = 1)
  system('xxhsum -H0 test.img')
  system('xxhsum -H1 test.img')
  system('xxhsum -H2 test.img')

  test <- as.numeric(seq(1024*1024))
  writeBin(test, "test.img", size = 8)
  system('xxhsum -H0 test.img')
  system('xxhsum -H1 test.img')
  system('xxhsum -H2 test.img')

  test <- as.integer(seq(1024*1024))
  writeBin(test, "test.img", size = 4)
  system('xxhsum -H0 test.img')
  system('xxhsum -H1 test.img')
  system('xxhsum -H2 test.img')

  test <- rep(c(TRUE, FALSE), 1024)
  writeBin(test, "test.img", size = 4)
  system('xxhsum -H0 test.img')
  system('xxhsum -H1 test.img')
  system('xxhsum -H2 test.img')
}


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Capture all the hashes from the command line here
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ref0 <- list(
  xxhash32  = '9430f97f',
  xxhash64  = '87d2a1b6e1163ef1',
  xxhash128 = 'b811c9319489c8e6d5c5e856f17ac497'
)

ref_raw <- list(
  xxhash32  = 'c4b952f3',
  xxhash64  = '7da1c892ebecf432',
  xxhash128 = '6af1fb304eef27231f4be0d55a17a31a'
)

ref_dbl <- list(
  xxhash32  = '7e1e0bf2',
  xxhash64  = 'bc684dac194c65a4',
  xxhash128 = '194e7b1cdb8d9156c6d6d1863ebe6447'
)

ref_int <- list(
  xxhash32  = 'a1f58821',
  xxhash64  = '2a4e535963d5c229',
  xxhash128 = 'fc48b80feecedcb118e77d64b09efedb'
)

ref_lgl <- list(
  xxhash32  = '863ec8ce',
  xxhash64  = '00016fe6dc9e8615',
  xxhash128 = '7665bb333cbf0319e119c3dc0f7b112b'
)


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Test that xxhashlite::funcs match the command line output
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
test_that("Same hashes in R and from xxHash command line (xxhsum)", {

  test <- raw(1024*1024)
  expect_equal(xxhash32 (test), ref0$xxhash32)
  expect_equal(xxhash64 (test), ref0$xxhash64)
  expect_equal(xxhash128(test), ref0$xxhash128)

  test <- suppressWarnings(as.raw(seq(1024*1024)))
  expect_equal(xxhash32 (test), ref_raw$xxhash32)
  expect_equal(xxhash64 (test), ref_raw$xxhash64)
  expect_equal(xxhash128(test), ref_raw$xxhash128)

  test <- as.numeric(seq(1024*1024))
  expect_equal(xxhash32 (test), ref_dbl$xxhash32)
  expect_equal(xxhash64 (test), ref_dbl$xxhash64)
  expect_equal(xxhash128(test), ref_dbl$xxhash128)

  test <- as.integer(seq(1024*1024))
  expect_equal(xxhash32 (test), ref_int$xxhash32)
  expect_equal(xxhash64 (test), ref_int$xxhash64)
  expect_equal(xxhash128(test), ref_int$xxhash128)

  test <- rep(c(TRUE, FALSE), 1024)
  expect_equal(xxhash32 (test), ref_lgl$xxhash32)
  expect_equal(xxhash64 (test), ref_lgl$xxhash64)
  expect_equal(xxhash128(test), ref_lgl$xxhash128)
})
