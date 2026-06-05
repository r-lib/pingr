# DNS query

Perform a DNS query for a domain. It supports custom name servers, and
querying DNS records of certain class and type.

## Usage

``` r
nsl(domain, server = NULL, type = 1L, class = 1L)
```

## Arguments

- domain:

  Domain to query.

- server:

  Custom name server IP address, to use. Note that this must be an IP
  address currently. E.g. 8.8.8.8 is Google's DNS server.

- type:

  Record type to query, an integer scalar. 1L is an A record, 28L is an
  AAAA record, etc. See e.g.
  https://en.wikipedia.org/wiki/List_of_DNS_record_types for the record
  types.

- class:

  Query class. This is usually 1L, i.e. "Internet". See e.g.
  https://www.iana.org/assignments/dns-parameters/dns-parameters.xhtml#dns-parameters-2
  for all DNS classes.

## Value

A list of two entries currently, additional entries might be added
later:

- `answer`: a data frame of DNS records, with columns: `name`, `class`,
  `type`, `ttl`, `data`. `data` is a list column and contains the IP(6)
  address for A and AAAA records, but it contains other data, e.g. host
  name for CNAME, for other records. If pingr could not parse a record
  (it only parses the most common records types: A, AAAA, NA, PTR,
  CNAME, TXT, MX, SOA), then the data of the record is included as a raw
  vector.

- `flags`: a named logical vector of flags `aa`, `tc`, `rd`, `ra`, `ad`,
  `cd`. See the RFC (https://www.ietf.org/rfc/rfc1035.txt) for these. On
  Windows they are all set to NA currently.

## Examples

``` r
nsl("r-project.org")
#> $answer
#>            name class type  ttl          data
#> 1 r-project.org     1    1 1800 137.208.57.37
#> 
#> $flags
#>    aa    tc    rd    ra    ad    cd 
#> FALSE FALSE  TRUE  TRUE FALSE FALSE 
#> 
nsl("google.com", type = 28L)
#> $answer
#>         name class type ttl                     data
#> 1 google.com     1   28 158 2607:f8b0:400a:808::200e
#> 
#> $flags
#>    aa    tc    rd    ra    ad    cd 
#> FALSE FALSE  TRUE  TRUE FALSE FALSE 
#> 
```
