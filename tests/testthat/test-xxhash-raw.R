
obj <- list(
  raw  = as.raw(seq_len(1024) %% 255),
  dbl  = as.double(seq(1024)),
  int  = as.integer(seq(1024)),
  lgl  = rep(c(TRUE, FALSE), 512)
)

algos <- c('xxh32', 'xxh64', 'xxh128', 'xxh3')

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Run the command line xxHash over data
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if (FALSE) {
  
  ref <- list()
  
  for (nm in names(obj)) {
    ref[[nm]] <- list()
    dat <- obj[[nm]]
    filename <- paste0("ref/", nm, ".bin")
    filename <- testthat::test_path(filename)
    writeBin(serialize(dat, NULL, xdr = FALSE), filename, size = 1)
    for (i in seq_along(algos)) {
      algo <- algos[i]
      cmd <- sprintf("xxhsum -H%i %s", i-1, filename)
      res <- system(cmd, intern = TRUE) |> strsplit(" ") |> el()
      if (algo == 'xxh3') {
        res <- tail(res, 1)
      } else {
        res <- head(res, 1)
      }
      ref[[nm]][[algo]] <- res
    }
  }
  
  ref
  

}


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Capture all the hashes from the command line here
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ref <- list(
  raw = list(xxh32 = "8460bda9", xxh64 = "00e57cb921892898", 
             xxh128 = "44c4a18300025592f8f3fcb32de7befc", xxh3 = "f8f3fcb32de7befc"), 
  dbl = list(xxh32 = "ca9c9fce", xxh64 = "aca5accb6a0dfe74", 
             xxh128 = "41cd4df490971609446fe5ee32c8410f", xxh3 = "9de70506fdaba2a9"), 
  int = list(xxh32 = "716051ed", xxh64 = "683312d708813dc9", 
             xxh128 = "4f95b75e8b9506fb1c101904f3279907", xxh3 = "c9851632e886e01c"), 
  lgl = list(xxh32 = "ba36c788", xxh64 = "7e5df9cc3c997ca8", 
             xxh128 = "8da01f89f4bd520eb615184122e4760a", xxh3 = "b615184122e4760a")
)


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Test that xxhashlite::funcs match the command line output
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
test_that("Same hashes in R and from xxHash command line (xxhsum)", {

  for (nm in names(obj)) {
    filename <- paste0("ref/", nm, ".bin")
    filename <- testthat::test_path(filename)
    dat      <- readBin(filename, raw(), file.size(filename))
    for (algo in algos) {
      
      result    <- xxhash_raw(dat, algo = algo)
      reference <- ref[[nm]][[algo]]
      
      expect_identical(result, reference, label = paste(nm, algo))
    }
  }
  
})


test_that("xxhash_raw() on strings works", {
  
  expect_identical(
    xxhash_raw("hello"),
    xxhash_raw(charToRaw("hello"))
  )
  
})



