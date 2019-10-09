
# 1.2.0

* New `is_up()` function to check if a web (other other TCP) server is up.

* Timeout now works correctly on Linux systems (#7).

* `ping()` uses processx now to run the external ping program, so the
  the ping error messages do not litter the R console (#8, #9).

# 1.1.2

No user visible changes.

# 1.1.0

* New `is_online()` function to check if the computer is online, by
  pinging two DNS servers.

* TCP Timeout now works for the connect phase as well.

# 1.0.0

First release on CRAN.
