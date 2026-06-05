# Changelog

## pingr (development version)

## pingr 2.0.5

CRAN release: 2024-12-12

- pingr now compiles with musl, e.g. on Alpine Linux, again.

## pingr 2.0.4

CRAN release: 2024-10-28

- [`ping()`](http://r-lib.github.io/pingr/dev/reference/ping.md) now
  handles sub-millisecond response times
  ([\#24](https://github.com/r-lib/pingr/issues/24)).

- [`ping()`](http://r-lib.github.io/pingr/dev/reference/ping.md) now
  works on FreeBSD, OpenBSD, NetBSD and DragonFlyBSD.

## pingr 2.0.3

CRAN release: 2023-12-10

- [`ping_port()`](http://r-lib.github.io/pingr/dev/reference/ping_port.md)
  now correctly prints the port if `version = TRUE`.

## pingr 2.0.2

CRAN release: 2022-10-26

- [`pingr::ping()`](http://r-lib.github.io/pingr/dev/reference/ping.md)
  now works better in a non-English locale
  ([\#18](https://github.com/r-lib/pingr/issues/18)).

## pingr 2.0.1

CRAN release: 2020-06-22

- [`is_online()`](http://r-lib.github.io/pingr/dev/reference/is_online.md)
  now tries the Apple captive test first, because it works better when
  DNS is not masked, but HTTP is
  ([\#13](https://github.com/r-lib/pingr/issues/13)).

- [`ping()`](http://r-lib.github.io/pingr/dev/reference/ping.md) now
  works on Linux systems with a non-English locale
  [@pekkarr](https://github.com/pekkarr)
  ([\#18](https://github.com/r-lib/pingr/issues/18)).

## pingr 2.0.0

CRAN release: 2019-10-21

- New [`nsl()`](http://r-lib.github.io/pingr/dev/reference/nsl.md)
  function to perform DNS queries.

- New [`my_ip()`](http://r-lib.github.io/pingr/dev/reference/my_ip.md)
  function to query the computer’s public IP address.

- New
  [`apple_captive_test()`](http://r-lib.github.io/pingr/dev/reference/apple_captive_test.md)
  function to check Apple’s captive test web page to see if the computer
  is online.

- Better
  [`is_online()`](http://r-lib.github.io/pingr/dev/reference/is_online.md)
  implementation, it uses DNS and HTTPS instead of an ICMP ping via an
  external ping program.

- Now `ip_up()` checks first if the computer is connected to the
  internet, via
  [`is_online()`](http://r-lib.github.io/pingr/dev/reference/is_online.md).

## pingr 1.2.0

CRAN release: 2019-10-09

- New
  [`is_up()`](http://r-lib.github.io/pingr/dev/reference/ping_port.md)
  function to check if a web (other other TCP) server is up.

- Timeout now works correctly on Linux systems
  ([\#7](https://github.com/r-lib/pingr/issues/7)).

- [`ping()`](http://r-lib.github.io/pingr/dev/reference/ping.md) uses
  processx now to run the external ping program, so the the ping error
  messages do not litter the R console
  ([\#8](https://github.com/r-lib/pingr/issues/8),
  [\#9](https://github.com/r-lib/pingr/issues/9)).

## pingr 1.1.2

CRAN release: 2017-03-02

No user visible changes.

## pingr 1.1.0

CRAN release: 2014-10-06

- New
  [`is_online()`](http://r-lib.github.io/pingr/dev/reference/is_online.md)
  function to check if the computer is online, by pinging two DNS
  servers.

- TCP Timeout now works for the connect phase as well.

## pingr 1.0.0

CRAN release: 2014-09-22

First release on CRAN.
