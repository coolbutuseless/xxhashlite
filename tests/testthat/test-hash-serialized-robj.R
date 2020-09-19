


test_that("serialization of r objects works", {

  for (method in c("xxhash32", 'xxhash64', 'xxhash128', 'xxh3_64bits')) {
    res1 <- xxhash_vec(base::serialize(mtcars, NULL, xdr = FALSE), method)
    res2 <- xxhash(mtcars, method)
    expect_identical(res1, res2)
  }

})
