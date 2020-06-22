
#' DNS query
#'
#' Perform a DNS query for a domain. It supports custom name servers,
#' and querying DNS records of certain class and type.
#'
#' @param domain Domain to query.
#' @param server Custom name server IP address, to use. Note that this
#'   must be an IP address currently. E.g. 8.8.8.8 is Google's DNS server.
#' @param type Record type to query, an integer scalar. 1L is an A record,
#'   28L is an AAAA record, etc. See e.g.
#'   https://en.wikipedia.org/wiki/List_of_DNS_record_types for the record
#'   types.
#' @param class Query class. This is usually 1L, i.e. "Internet". See e.g.
#'   https://www.iana.org/assignments/dns-parameters/dns-parameters.xhtml#dns-parameters-2
#'   for all DNS classes.
#' @return A list of two entries currently, additional entries might be
#'   added later:
#'   * `answer`: a data frame of DNS records, with columns:
#'     `name`, `class`, `type`, `ttl`, `data`. `data` is a list column and
#'     contains the IP(6) address for A and AAAA records, but it contains
#'     other data, e.g. host name for CNAME, for other records. If pingr
#'     could not parse a record (it only parses the most common records
#'     types: A, AAAA, NA, PTR, CNAME, TXT, MX, SOA), then the data of
#'     the record is included as a raw vector.
#'   * `flags`: a named logical vector of flags `aa`, `tc`, `rd`, `ra`,
#'     `ad`, `cd`. See the RFC (https://www.ietf.org/rfc/rfc1035.txt) for
#'      these. On Windows they are all set to NA currently.
#'
#' @export
#' @examplesIf pingr:::safe_examples()
#' nsl("r-project.org")
#' nsl("google.com", type = 28L)

nsl <- function(domain, server = NULL, type = 1L, class = 1L) {
  stopifnot(
    is_string(domain),
    is_string_or_null(server),
    is_type(type),
    is_class(class)
  )
  .Call(r_nsl, domain, server, class, type)
}
