

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
  xxhash128 = 'b6ef17a3448492b6918780b90550bf34'
)

ref_raw <- list(
  xxhash32  = 'c4b952f3',
  xxhash64  = '7da1c892ebecf432',
  xxhash128 = 'e5f44940c7724d0063a020b8fefbb3c9'
)

ref_dbl <- list(
  xxhash32  = '7e1e0bf2',
  xxhash64  = 'bc684dac194c65a4',
  xxhash128 = 'a551c561de665f12119b62e86cabea50'
)

ref_int <- list(
  xxhash32  = 'a1f58821',
  xxhash64  = '2a4e535963d5c229',
  xxhash128 = 'f25eee26583a56634bd45a7c51679308'
)

ref_lgl <- list(
  xxhash32  = '863ec8ce',
  xxhash64  = '00016fe6dc9e8615',
  xxhash128 = 'ce44a90221a5abe3bcdd968456273c3a'
)


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Test that xxhashlite::funcs match the command line output
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
test_that("Same hashes in R and from xxHash command line (xxhsum)", {

  test <- raw(1024*1024)
  expect_equal(xxhash_vec(test, 'xxhash32' ), ref0$xxhash32)
  expect_equal(xxhash_vec(test, 'xxhash64' ), ref0$xxhash64)
  expect_equal(xxhash_vec(test, 'xxhash128'), ref0$xxhash128)

  test <- suppressWarnings(as.raw(seq(1024*1024)))
  expect_equal(xxhash_vec(test, 'xxhash32' ), ref_raw$xxhash32)
  expect_equal(xxhash_vec(test, 'xxhash64' ), ref_raw$xxhash64)
  expect_equal(xxhash_vec(test, 'xxhash128'), ref_raw$xxhash128)

  test <- as.numeric(seq(1024*1024))
  expect_equal(xxhash_vec(test, 'xxhash32' ), ref_dbl$xxhash32)
  expect_equal(xxhash_vec(test, 'xxhash64' ), ref_dbl$xxhash64)
  expect_equal(xxhash_vec(test, 'xxhash128'), ref_dbl$xxhash128)

  test <- as.integer(seq(1024*1024))
  expect_equal(xxhash_vec(test, 'xxhash32' ), ref_int$xxhash32)
  expect_equal(xxhash_vec(test, 'xxhash64' ), ref_int$xxhash64)
  expect_equal(xxhash_vec(test, 'xxhash128'), ref_int$xxhash128)

  test <- rep(c(TRUE, FALSE), 1024)
  expect_equal(xxhash_vec(test, 'xxhash32' ), ref_lgl$xxhash32)
  expect_equal(xxhash_vec(test, 'xxhash64' ), ref_lgl$xxhash64)
  expect_equal(xxhash_vec(test, 'xxhash128'), ref_lgl$xxhash128)
})
