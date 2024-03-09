
obj <- list(
  raw  = as.raw(seq_len(1024*1024) %% 255),
  dbl  = as.double(seq(1024*1024)),
  int  = as.integer(seq(1024*1024)),
  lgl  = rep(c(TRUE, FALSE), 1024)
)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Run the command line xxHash over data
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if (FALSE) {
  
  ref <- list()
  
  writeBin(serialize(obj$raw, NULL, xdr = TRUE), "working/test.img", size = 1)
  ref$raw <- list(
    xxhash32    = system('xxhsum -H0 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> head(1),
    xxhash64    = system('xxhsum -H1 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> head(1),
    xxhash128   = system('xxhsum -H2 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> head(1),
    xxh3_64bits = system('xxhsum -H3 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> tail(1)
  )
  
  writeBin(serialize(obj$dbl, NULL, xdr = TRUE), "working/test.img", size = 1)
  ref$dbl <- list(
    xxhash32    = system('xxhsum -H0 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> head(1),
    xxhash64    = system('xxhsum -H1 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> head(1),
    xxhash128   = system('xxhsum -H2 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> head(1),
    xxh3_64bits = system('xxhsum -H3 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> tail(1)
  )

  
  writeBin(serialize(obj$int, NULL, xdr = TRUE), "working/test.img", size = 1)
  ref$int <- list(
    xxhash32    = system('xxhsum -H0 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> head(1),
    xxhash64    = system('xxhsum -H1 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> head(1),
    xxhash128   = system('xxhsum -H2 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> head(1),
    xxh3_64bits = system('xxhsum -H3 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> tail(1)
  )

  
  writeBin(serialize(obj$lgl, NULL, xdr = TRUE), "working/test.img", size = 1)
  ref$lgl <- list(
    xxhash32    = system('xxhsum -H0 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> head(1),
    xxhash64    = system('xxhsum -H1 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> head(1),
    xxhash128   = system('xxhsum -H2 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> head(1),
    xxh3_64bits = system('xxhsum -H3 working/test.img', intern = TRUE) |> strsplit(" ") |> el() |> tail(1)
  )

}


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Capture all the hashes from the command line here
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ref <- list(
  raw = list(xxhash32 = "6ff92168", xxhash64 = "dcfd90ef642d73f8", 
             xxhash128 = "4fc87515240631d4b48315fe8abb8212", xxh3_64bits = "b48315fe8abb8212"), 
  dbl = list(xxhash32 = "6ae334e0", xxhash64 = "7299b3c5ced2e073", 
             xxhash128 = "6f4bebda420f81152b704eb96fec1db3", xxh3_64bits = "c63557f4a3267354"), 
  int = list(xxhash32 = "4f307e83", xxhash64 = "bef75013873e762e", 
             xxhash128 = "391e00a5330655a9bbd83ad3220d2a67", xxh3_64bits = "d1a1bad45282b79c"), 
  lgl = list(xxhash32 = "e58e8003", xxhash64 = "5a3f227afa24d073", 
             xxhash128 = "b5cd4df90650c13db3a5c75d23dd6456", xxh3_64bits = "b3a5c75d23dd6456")
)


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Test that xxhashlite::funcs match the command line output
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
test_that("Same hashes in R and from xxHash command line (xxhsum)", {

  for (nm in names(obj)) {
    test <- obj[[nm]]
    for (algo in c('xxhash32', 'xxhash64', 'xxhash128', 'xxh3_64bits')) {
      result    <- xxhash(test, algo = algo)
      reference <- ref[[nm]][[algo]]
      expect_identical(result, reference, label = paste(nm, algo))
    }
  }
  
})
