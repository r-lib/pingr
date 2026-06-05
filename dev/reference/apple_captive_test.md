# Download Apple's captive portal test

If the test page, returns "Success" that means that the computer is
connected to the Internet.

## Usage

``` r
apple_captive_test()
```

## Details

Note that this function will fail if the computer is offline. Use
[`is_online()`](http://r-lib.github.io/pingr/dev/reference/is_online.md)
to check if the computer is online.

## Examples

``` r
apple_captive_test()
#> [1] TRUE
```
