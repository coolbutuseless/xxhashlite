
test_that("multiplication works", {
  
  path <- testthat::test_path("ref")
  testfiles <- list.files(path, full.names = TRUE)
  algos <- c('xxh32', 'xxh64', 'xxh128', 'xxh3')
  
  testfile <- testfiles[[1]]
  algo <- 'xxh128'
  
  for (testfile in testfiles) {
    for (algo in algos){
      expect_identical(
        xxhash_raw(readBin(testfile, raw(), file.size(testfile)), algo = algo),
        xxhash_file(testfile, algo = algo),
        label = paste(algo, testfile, ":")
      )
    }
  }  
})
