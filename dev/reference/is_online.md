# Is the computer online?

Check if the computer is online. It does three tries:

- Retrieve Apple's Captive Portal test page, see
  [`apple_captive_test()`](http://r-lib.github.io/pingr/dev/reference/apple_captive_test.md).

- Queries myip.opendns.com on OpenDNS, see
  [`my_ip()`](http://r-lib.github.io/pingr/dev/reference/my_ip.md).

- Retrieves icanhazip.com via HTTPS, see
  [`my_ip()`](http://r-lib.github.io/pingr/dev/reference/my_ip.md). If
  any of these are successful, it returns `TRUE`.

## Usage

``` r
is_online(timeout = 1)
```

## Arguments

- timeout:

  Timeout for the queries. (Note: it is currently not used for the DNS
  query.)

## Value

Possible values:

- `TRUE` Yes, online.

- `FALSE` No, not online.

## Examples

``` r
is_online()
#> [1] TRUE
```
