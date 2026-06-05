# Query the computer's public IP address

It can use a DNS query to opendns.com, if `method == "dns"`, or an HTTPS
query to icanhazip.com, see https://github.com/major/icanhaz. The DNS
query is much faster, the HTTPS query is secure.

## Usage

``` r
my_ip(method = c("dns", "https"))
```

## Arguments

- method:

  Whether to use a DNS or HTTPS query.

## Value

Computer's public IP address as a string.

## Examples

``` r
my_ip()
#> [1] "52.233.87.87"
my_ip(method = "https")
#> [1] "52.233.87.87"
```
