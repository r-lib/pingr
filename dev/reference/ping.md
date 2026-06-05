# Ping a remote server, to see if it is alive

This is the classic ping, using ICMP packages. Only the system
administrator can send ICMP packages, so we call out to the system's
ping utility.

## Usage

``` r
ping(
  destination,
  continuous = FALSE,
  verbose = continuous,
  count = 3L,
  timeout = 1
)
```

## Arguments

- destination:

  Host name or IP address.

- continuous:

  Logical, whether to keep pinging until the user interrupts.

- verbose:

  Whether to print progress on the screen while pinging.

- count:

  Number of pings to perform.

- timeout:

  Timeout for a ping response.

## Value

Vector of response times. `NA` means no response, in milliseconds.
Currently `NA`s are always at the end of the vector, and not in their
correct position.

## Examples

``` r
ping("8.8.8.8")
#> [1] NA NA NA
ping("r-project.org")
#> [1] NA NA NA
```
