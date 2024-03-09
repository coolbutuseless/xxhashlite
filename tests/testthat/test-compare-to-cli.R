
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
  raw = list(xxhash32 = "57ff623f", xxhash64 = "2248cf8e3a94dcd6", 
             xxhash128 = "d2e3818b9db41d399fabfe83722c4062", xxh3_64bits = "9fabfe83722c4062"), 
  dbl = list(xxhash32 = "f174df39", xxhash64 = "98eff518cf372652", 
             xxhash128 = "9b6c34a96ba5dec6b730ee74eef07cb7", xxh3_64bits = "31bfe0f8f39443da"), 
  int = list(xxhash32 = "cb4d196f", xxhash64 = "8295d1be2d952a3f", 
             xxhash128 = "886cbed1190eb82139aa549a24cdb3c5", xxh3_64bits = "9d1529b13a2dea6b"), 
  lgl = list(xxhash32 = "91d543e0", xxhash64 = "d6fcc76a7bc7079f", 
             xxhash128 = "c2986ddd79e32c97543f7ef2662aab0d", xxh3_64bits = "543f7ef2662aab0d")
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
      # message(result, " == ", reference)
      expect_identical(result, reference)
    }
  }
  
})
