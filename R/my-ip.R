
#' Query the computer's public IP address
#'
#' It can use a DNS query to opendns.com, if `method == "dns"`, or
#' an HTTPS query to icanhazip.com, see https://github.com/major/icanhaz.
#' The DNS query is much faster, the HTTPS query is secure.
#'
#' @param method Whether to use a DNS or HTTPS query.
#' @return Computer's public IP address as a string.
#'
#' @export
#' @examples
#' my_ip()
#' my_ip(method = "https")

my_ip <- function(method = c("dns", "https")) {
  method <- match.arg(method)
  if (method == "dns") my_ip_dns() else my_ip_https()
}

my_ip_dns <- function() {
  out <- nsl("myip.opendns.com", server = "208.67.222.222", type = 1L)
  if (nrow(out$answer) != 1 ||
      out$answer$type != 1L ||
      !is_ip_address(out$answer$data[[1]])) {
    stop("Cannot query my iP address via DNS")
  }

  out$answer$data[[1]]
}

is_ip_address <- function(x) {
  grepl("^[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+$", x)
}

my_ip_https <- function() {
  out <- http_get("https://ipv4.icanhazip.com/")
  out <- gsub("\\s+", "", out)

  if (!is_ip_address(out)) stop("Cannot query my IP address via https")

  out
}
