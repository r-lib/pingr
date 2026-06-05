# Check if a port of a server is active, measure response time

Check if a port of a server is active, measure response time

`is_up()` checks if a web server is up.

## Usage

``` r
ping_port(
  destination,
  port = 80L,
  continuous = FALSE,
  verbose = continuous,
  count = 3L,
  timeout = 1
)

is_up(
  destination,
  port = 80,
  timeout = 0.5,
  fail_on_dns_error = FALSE,
  check_online = TRUE
)
```

## Arguments

- destination:

  Host name or IP address.

- port:

  Port.

- continuous:

  Logical, whether to keep pinging until the user interrupts.

- verbose:

  Whether to print progress on the screen while pinging.

- count:

  Number of pings to perform.

- timeout:

  Timeout, in seconds. How long to wait for a ping to succeed.

- fail_on_dns_error:

  If `TRUE` then `is_up()` fails if the DNS resolution fails. Otherwise
  it will return `FALSE`.

- check_online:

  Whether to check first if the computer is online. Otherwise it is
  possible that the computer is behind a proxy, that hijacks the HTTP
  connection to `destination`.

## Value

Vector of response times, in milliseconds. `NA` means no response within
the timeout.

## Examples

``` r
ping_port("r-project.org")
#> [1] 162.349 159.869 159.595
is_up("google.com")
#> [1] TRUE
is_up("google.com", timeout = 0.01)
#> [1] TRUE
```
