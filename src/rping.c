
#include <R.h>
#include <Rdefines.h>

int r_ping_main(const char *Destination, int Port, int Type,
		int Continuous, int Count, int Timeout, double *Values);

SEXP r_ping(SEXP p_destination, SEXP p_port, SEXP p_type, SEXP p_continuous,
	    SEXP p_count, SEXP p_timeout) {

  SEXP result;
  const char *Destination;
  int Port, Type, Continuous, Count, Timeout, ret;

  if (LENGTH(p_destination) != 1) {
    error("destination must be a character scalar");
  }
  if (LENGTH(p_port) != 1) { error("port must be a numeric scalar"); }
  if (LENGTH(p_type) != 1) { error("type must be a character scalar"); }
  if (LENGTH(p_continuous) != 1) {
    error("continuous must be a logical scalar");
  }
  if (LENGTH(p_count) != 1) { error("type must be a numeric scalar"); }
  if (LENGTH(p_timeout) != 1) { error("type must be a numeric scalar"); }

  Destination = CHAR(STRING_ELT(AS_CHARACTER(p_destination), 0));
  Port = INTEGER(AS_INTEGER(p_port))[0];
  Type = INTEGER(AS_INTEGER(p_type))[0];
  Continuous = INTEGER(AS_INTEGER(p_continuous))[0];
  Count = INTEGER(AS_INTEGER(p_count))[0];
  Timeout = INTEGER(AS_INTEGER(p_timeout))[0];

  PROTECT(result = NEW_NUMERIC(Count));

  ret = r_ping_main(Destination, Port, Type, Continuous, Count,
		    Timeout, REAL(result));

  UNPROTECT(1);
  return result;
}
