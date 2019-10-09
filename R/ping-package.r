
#' Check if a remote computer is up
#'
#' @useDynLib pingr, .registration = TRUE, .fixes = "C_"
#' @docType package
#' @name pingr-package
NULL

#' Check if a port of a server is active, measure response time
#'
#' @param destination Host name or IP address.
#' @param port Port.
#' @param continuous Logical, whether to keep pinging until
#'   the user interrupts.
#' @param verbose Whether to print progress on the screen while
#'   pinging.
#' @param count Number of pings to perform.
#' @param timeout Timeout, in seconds. How long to wait for a
#'   ping to succeed.
#' @return Vector of response times, in milliseconds.
#'   \code{NA} means no response within the timeout.
#'
#' @export
#' @examples
#' ping_port("127.0.0.1")
#' ping_port("r-project.org")

ping_port <- function(destination, port = 80L,
                      continuous = FALSE, verbose = continuous,
                      count = 3L, timeout = 1.0) {

  type <- "tcp"
  type <- switch(type, "tcp" = 0L, "udp" = 1L)
  timeout <- as.integer(timeout * 1000000)
  res <- .Call("r_ping", destination, port, type, continuous, verbose,
               count, timeout, PACKAGE = "pingr")
  res[ res == -1 ] <- NA_real_
  res
}

#' Ping a remote server, to see if it is alive
#'
#' This is the classic ping, using ICMP packages. Only the
#' system administrator can send ICMP packages, so we call out
#' to the system's ping utility.
#'
#' @param destination Host name or IP address.
#' @param continuous Logical, whether to keep pinging until the
#'   user interrupts.
#' @param verbose Whether to print progress on the screen while
#'   pinging.
#' @param count Number of pings to perform.
#' @param timeout Timeout for a ping response.
#' @return Vector of response times. \code{NA} means no response, in
#'   milliseconds. Currently \code{NA}s are always at the end of the vector,
#'   and not in their correct position.
#'
#' @export
#' @importFrom processx run
#' @examples
#' ping("8.8.8.8")
#' ping("r-project.org")

ping <- function(destination, continuous = FALSE, verbose = continuous,
                 count = 3L, timeout = 1.0) {

  if (!continuous && verbose) {
    stop("'!continuous' && 'verbose' does not work currently")
  }

  os <- ping_os(destination, continuous, count, timeout)

  status <- run(os$cmd[1], os$cmd[-1], error_on_status = FALSE)
  output <- strsplit(status$stdout, "\r?\n")[[1]]

  if (!continuous) {
    timings <- grep(os$regex, output, value = TRUE, perl = TRUE)
    times <- sub(os$regex, "\\1", timings, perl = TRUE)
    res <- as.numeric(times)
    length(res) <- count
    res
  } else {
    invisible()
  }
}

ping_os <- function(destination, continuous, count, timeout) {

  if (.Platform$OS.type == "windows") {
    ping_file <- file.path("C:", "windows", "system32", "ping.exe")
    if (!file.exists(ping_file)) { ping_file <- "ping" }
    cmd <- c(
      ping_file,
      "-w", int(timeout * 1000),
      if (continuous) "-t" else c("-n", count),
      destination
    )

  } else if (Sys.info()["sysname"] == "Darwin") {
    cmd <- c(
      "/sbin/ping",
      "-W", int(timeout * 1000),
      if (!continuous) c("-c", count),
      destination
    )

  } else if (Sys.info()[["sysname"]] == "Linux") {
    cmd <- c(
      "ping",
      "-W", int(timeout),
      if (!continuous) c("-c", count),
      destination
    )

  } else if (.Platform$OS.type == "unix") {
    cmd <- c(
      "ping",
      "-W", int(timeout * 1000),
      if (!continuous) c("-c", count),
      destination
    )
  }

  list(cmd = cmd, regex = "^.*time=(.+)[ ]?ms.*$")
}

## Domains and IPs to test for internet connection

internet_domains <- c("google-public-dns-a.google.com",
                      "b.resolvers.Level3.net")

internet_ips <- c("8.8.8.8",
                  "4.2.2.2")

#' Is the computer online?
#'
#' Ping some name servers that are always (well, almost) up.
#' If these are unreachable, then you are most probably not online.
#'
#' @param timeout Timeout for the pings.
#' @return Possible values: \itemize{
#'   \item \code{TRUE} Yes, online.
#'   \item \code{FALSE} No, not online.
#'   \item \code{"nodns"} We re online, but without a DNS service.
#' }
#'
#' @export
#' @examples
#' is_online()
#' is_online(timeout = 0.01)

is_online <- function(timeout = 0.2) {
  for (domain in internet_domains) {
    if (!is.na(ping(domain, count = 1, timeout = timeout))) { return(TRUE) }
  }
  for (ip in internet_ips) {
    if (!is.na(ping(ip, count = 1, timeout = timeout))) { return("nodns") }
  }
  return(FALSE)
}

#' `is_up()` checks if a web server is up.
#'
#' @rdname ping_port
#' @param fail_on_dns_error If `TRUE` then `is_up()` fails if the DNS
#'   resolution fails. Otherwise it will return `FALSE`.
#' @export
#' @examples
#' is_up("google.com")
#' is_up("google.com", timeout = 0.01)

is_up <- function(destination, port = 80, timeout = 0.5,
                  fail_on_dns_error = FALSE) {
  tryCatch(
    !is.na(ping_port(destination, port = port, timeout = timeout, count = 1)),
    error = function(e) {
      if (fail_on_dns_error) stop(e)
      FALSE
    })
}
