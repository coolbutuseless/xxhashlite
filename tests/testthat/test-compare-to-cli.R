

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Run the command line xxHash over data
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if (FALSE) {
  test <- raw(1024*1024)
  writeBin(serialize(test, NULL, xdr = FALSE), "test.img", size = 1)
  system('xxhsum -H0 test.img')
  system('xxhsum -H1 test.img')
  system('xxhsum -H2 test.img')

  test <- as.raw(seq(1024*1024))
  writeBin(serialize(test, NULL, xdr = FALSE), "test.img", size = 1)
  system('xxhsum -H0 test.img')
  system('xxhsum -H1 test.img')
  system('xxhsum -H2 test.img')

  test <- as.numeric(seq(1024*1024))
  writeBin(serialize(test, NULL, xdr = FALSE), "test.img", size = 1)
  system('xxhsum -H0 test.img')
  system('xxhsum -H1 test.img')
  system('xxhsum -H2 test.img')

  test <- as.integer(seq(1024*1024))
  writeBin(serialize(test, NULL, xdr = FALSE), "test.img", size = 1)
  system('xxhsum -H0 test.img')
  system('xxhsum -H1 test.img')
  system('xxhsum -H2 test.img')

  test <- rep(c(TRUE, FALSE), 1024)
  writeBin(serialize(test, NULL, xdr = FALSE), "test.img", size = 1)
  system('xxhsum -H0 test.img')
  system('xxhsum -H1 test.img')
  system('xxhsum -H2 test.img')
}


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Capture all the hashes from the command line here
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ref0 <- list(
  xxhash32  = '3d9e4acd',
  xxhash64  = 'ec0eadb95100d228',
  xxhash128 = 'f735e8ea0edb724085d2eac09452e487'
)

ref_raw <- list(
  xxhash32  = '2317cc28',
  xxhash64  = 'e44a4f00644c19b6',
  xxhash128 = '2e1a014c6584a9a45c6921d84bcef56e'
)

ref_dbl <- list(
  xxhash32  = '92c84650',
  xxhash64  = 'cd496818c7b7376c',
  xxhash128 = '9a30999e168db4e4a05d70ec87cc6d88'
)

ref_int <- list(
  xxhash32  = '0f020d06',
  xxhash64  = '8093505490a763f9',
  xxhash128 = '2cc31940cdc79003348d9ce111ec19af'
)

ref_lgl <- list(
  xxhash32  = '677a05bb',
  xxhash64  = '01f40a093b4e715d',
  xxhash128 = '4026f5a79358345f113d38edf374c4ee'
)


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Test that xxhashlite::funcs match the command line output
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
test_that("Same hashes in R and from xxHash command line (xxhsum)", {

  test <- raw(1024*1024)
  expect_equal(xxhash(test, 'xxhash32' ), ref0$xxhash32)
  expect_equal(xxhash(test, 'xxhash64' ), ref0$xxhash64)
  expect_equal(xxhash(test, 'xxhash128'), ref0$xxhash128)

  test <- suppressWarnings(as.raw(seq(1024*1024)))
  expect_equal(xxhash(test, 'xxhash32' ), ref_raw$xxhash32)
  expect_equal(xxhash(test, 'xxhash64' ), ref_raw$xxhash64)
  expect_equal(xxhash(test, 'xxhash128'), ref_raw$xxhash128)

  test <- as.numeric(seq(1024*1024))
  expect_equal(xxhash(test, 'xxhash32' ), ref_dbl$xxhash32)
  expect_equal(xxhash(test, 'xxhash64' ), ref_dbl$xxhash64)
  expect_equal(xxhash(test, 'xxhash128'), ref_dbl$xxhash128)

  test <- as.integer(seq(1024*1024))
  expect_equal(xxhash(test, 'xxhash32' ), ref_int$xxhash32)
  expect_equal(xxhash(test, 'xxhash64' ), ref_int$xxhash64)
  expect_equal(xxhash(test, 'xxhash128'), ref_int$xxhash128)

  test <- rep(c(TRUE, FALSE), 1024)
  expect_equal(xxhash(test, 'xxhash32' ), ref_lgl$xxhash32)
  expect_equal(xxhash(test, 'xxhash64' ), ref_lgl$xxhash64)
  expect_equal(xxhash(test, 'xxhash128'), ref_lgl$xxhash128)
})
