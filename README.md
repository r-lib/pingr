
# pingr: check if a server is alive

[![Linux Build
Status](https://travis-ci.org/r-lib/pingr.svg?branch=master)](https://travis-ci.org/r-lib/pingr)
[![Windows Build
status](https://ci.appveyor.com/api/projects/status/github/r-lib/pingr?svg=true)](https://ci.appveyor.com/project/gaborcsardi/pingr)
[![CRAN RStudio mirror
downloads](http://cranlogs.r-pkg.org/badges/pingr)](https://r-pkg.org/pkg/pingr)

The pingr package has tools to check if a remote computer or web server
is up and some other related tools.

## ICMP ping

The `ping()` function does ICMP ping, via the system’s `ping` utility:

``` r
library(pingr)
ping("127.0.0.1")
```

    #> [1] 0.046 0.058 0.053

By default it sends three packets and measures the time it receives and
answer. It waits between sending out the packets, so if you want a
really quick check, you can just send a single packet:

``` r
ping("127.0.0.1", count = 1)
```

    #> [1] 0.067

If a machine is down (or it does not exist), then `NA` is returned
instead of the roundtrip time:

``` r
ping("192.0.2.1", count = 1)
```

    #> [1] NA

## TCP ping

With TCP ping we can check if a machine is listeing on a TCP port,
e.g. if google’s search web server is up and running:

``` r
ping_port("www.google.com", port = 80, count = 1)
```

    #> [1] 12.676

## Query the public IP address of the computer

`my_ip()` queries the public IP of the computer, either via DNS or
HTTPS:

``` r
my_ip()
```

    #> [1] "81.133.85.232"

## Check if the computer is online

`is_online()` checks if the computer is online. It makes three tries:

  - Queries myip.opendns.com on OpenDNS, see `my_ip()`.
  - Retrieves icanhazip.com via HTTPS, see `my_ip()`.
  - Retrieve Apple’s Captive Portal test page, see
    `apple_captive_test()`.

If any of these are successful, it returns `TRUE`.

``` r
is_online()
```

    #> [1] TRUE

## DNS queries

The package also contains a function to perform DNS queries. This is a
more portable and more functional version of the `utils::nsl()`
function:

``` r
nsl("www.r-project.org", type = 1L)
```

    #> $answer
    #>                 name class type ttl               data
    #> 1  www.r-project.org     1    5 900 cran.wu-wien.ac.at
    #> 2 cran.wu-wien.ac.at     1    1 300      137.208.57.37
    #> 
    #> $flags
    #>    aa    tc    rd    ra    ad    cd 
    #> FALSE FALSE  TRUE  TRUE FALSE FALSE

``` r
nsl("google.com", type = 28L)
```

    #> $answer
    #>         name class type ttl                     data
    #> 1 google.com     1   28 110 2a00:1450:4009:81a::200e
    #> 
    #> $flags
    #>    aa    tc    rd    ra    ad    cd 
    #> FALSE FALSE  TRUE  TRUE FALSE FALSE

## License

MIT © RStudio
