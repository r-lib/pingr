
#' Check if a remote computer is up
#'
#' @useDynLib ping
#' @docType package
#' @name ping-package
NULL

#' Check if a port of a server is active, measure response time
#'
#' @param destination Host name or IP address.
#' @param port Port.
#' @param type Protocol, \sQuote{tcp} or \sQuote{udp}.
#' @param continuous Logical, whether to keep pinging until
#'   the user interrupts.
#' @param count Number of pings to perform.
#' @param timeout Timeout, in milliseconds. How long to wait for a
#'   ping to succeed.
#' @return Vector of response times. \code{NA} means no response
#'
#' @export

ping_port <- function(destination, port = 80L, type = c("tcp", "udp"),
                      continuous = FALSE, count = 3L, timeout = 1.0) {

  type <- switch(match.arg(type), "tcp" = 0L, "udp" = 1L)
  timeout <- as.integer(timeout * 1000000)
  res <- .Call("r_ping", destination, port, type, continuous, count, timeout,
               PACKAGE = "ping")
  res[ res == -1 ] <- NA_real_
  res
}
