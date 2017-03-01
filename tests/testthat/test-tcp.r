
context("TCP")

test_that("We can ping localhost", {

  ## Chances are, there is nothing here
  pr <- ping_port("127.0.0.1", port = 4695, count = 1)
  expect_equal(pr, NA_real_)

  ## Start web server
  r_httpd_port <- if(R.version[["svn rev"]] < 67550) {
    try(tools::startDynamicHelp(TRUE), silent = TRUE)
    getFromNamespace("httpdPort", "tools")
  } else {
    tools::startDynamicHelp(NA)
  }
  pr <- ping_port("127.0.0.1", port = r_httpd_port, count = 1)
  expect_true(is.double(pr))
  expect_true(length(pr) == 1)
  expect_true(pr < 1000)

  ## Shut down web server
  tools::startDynamicHelp(start = FALSE)
})

test_that("We can ping a remote host", {

  ## There is surely nothing here
  pr <- ping_port("igraph.org", port = 4695, count = 1)
  expect_equal(pr, NA_real_)

  ## There is surely something here
  pr <- ping_port("httpbin.org", count = 1)
  expect_true(is.double(pr))
  expect_true(length(pr) == 1)
  expect_true(pr < 1000)
})

test_that("We don't wait too long", {

  ## TODO

})

test_that("We don't wait for the resolver", {

  ## TODO

})
