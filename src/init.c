
#include <R.h>
#include <R_ext/Rdynload.h>

#include "pingr.h"

static const R_CallMethodDef callMethods[]  = {
  {"r_ping", (DL_FUNC) &r_ping, 7},
  {"r_nsl",  (DL_FUNC) &r_nsl,  4},
  {NULL, NULL, 0}
};

void R_init_pingr(DllInfo *dll) {
  R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
  R_forceSymbols(dll, TRUE);
}
