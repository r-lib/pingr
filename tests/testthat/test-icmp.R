test_that("We can ping localhost", {
  pr <- ping("127.0.0.1", count = 1)
  expect_true(is.double(pr))
  expect_true(length(pr) == 1)
  expect_true(pr < 5000)
})

test_that("We can ping a remote host", {

  ## can't ping google.com from GHA...
  testthat::skip_on_ci()

  ## Non-existent IP
  pr <- ping("0.0.0.1", count = 1)
  print(pr)
  expect_equal(pr, NA_real_)

  ## Google
  pr <- ping("google.com", count = 1, timeout = 5.0)
  expect_true(is.double(pr))
  expect_true(length(pr) == 1)
  print(pr)
  expect_true(pr < 5000)

  pr <- ping("8.8.8.8", count = 1, timeout = 5.0)
  expect_true(is.double(pr))
  expect_true(length(pr) == 1)
  print(pr)
  expect_true(pr < 5000)
})

test_that("We don't wait too long", {

  ## TODO
  expect_true(TRUE)
})
