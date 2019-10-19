
#ifndef R_PINGR_H
#define R_PINGR_H

#include <R.h>
#include <Rinternals.h>

SEXP r_ping(SEXP p_destination, SEXP p_port, SEXP p_type, SEXP p_continuous,
	    SEXP p_verbose, SEXP p_count, SEXP p_timeout);

SEXP r_nsl(SEXP hostname, SEXP server, SEXP class, SEXP type);

#endif
