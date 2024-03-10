
test_that("hash as raw vector works", {
  
  
  algos <- c('xxh32', 'xxh64', 'xxh128', 'xxh3')
  v <- letters
  
  for (algo in algos) {
      expect_identical(
              xxhash(v, algo = 'xxh128', as_raw = FALSE),
        paste(xxhash(v, algo = 'xxh128', as_raw = TRUE ), collapse = ""),
        label = paste(algo, ":")
      )
  }
  
  
})
