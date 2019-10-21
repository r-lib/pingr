
http_get <- function(url) {
  tmp <- tempfile()
  on.exit(unlink(tmp, recursive = TRUE), add = TRUE)

  suppressWarnings(utils::download.file(url, tmp, quiet = TRUE))

  if (!file.exists(tmp)) stop("Cannot download `", url, "`")

  readChar(tmp, file.info(tmp)$size, useBytes = TRUE)
}

#' Download Apple's captive portal test
#'
#' If the test page, returns "Success" that means that the computer is
#' connected to the Internet.
#'
#' Note that this function will fail if the computer is offline. Use
#' [is_online()] to check if the computer is online.
#'
#' @export
#' @examples
#' apple_captive_test()

apple_captive_test <- function() {
  out <- http_get("http://captive.apple.com/hotspot-detect.html")
  grepl("Success", out)
}
