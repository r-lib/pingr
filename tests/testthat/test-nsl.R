test_that("nsl function works with SRV records", {
  testthat::skip_on_ci()

  result = nsl("_http._tcp.mxtoolbox.com", type = 33L)
  expect_snapshot(result$answer$data)
})
