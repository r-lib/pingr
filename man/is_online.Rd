% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/ping-package.R
\name{is_online}
\alias{is_online}
\title{Is the computer online?}
\usage{
is_online(timeout = 1)
}
\arguments{
\item{timeout}{Timeout for the queries. (Note: it is currently not
used for the DNS query.)}
}
\value{
Possible values: \itemize{
\item \code{TRUE} Yes, online.
\item \code{FALSE} No, not online.
}
}
\description{
Check if the computer is online. It does three tries:
\itemize{
\item Retrieve Apple's Captive Portal test page, see \code{\link[=apple_captive_test]{apple_captive_test()}}.
\item Queries myip.opendns.com on OpenDNS, see \code{\link[=my_ip]{my_ip()}}.
\item Retrieves icanhazip.com via HTTPS, see \code{\link[=my_ip]{my_ip()}}.
If any of these are successful, it returns \code{TRUE}.
}
}
\examples{
\dontshow{if (pingr:::safe_examples()) (if (getRversion() >= "3.4") withAutoprint else force)(\{ # examplesIf}
is_online()
\dontshow{\}) # examplesIf}
}
