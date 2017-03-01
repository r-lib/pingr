
#include <R.h>
#include <Rdefines.h>
#include <R_ext/Rdynload.h>

#ifdef WIN32

#  define WIN32_LEAN_AND_MEAN
#  include <winsock2.h>
#  include <windows.h>
#  define close closesocket

#  define WINSTARTUP() if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {	\
    error("Cannot initialize network");					\
  }

#  define WINCLEANUP() WSACleanup()

void usleep(__int64 usec) {
  HANDLE timer;
  LARGE_INTEGER ft;
  ft.QuadPart = -(10*usec);
  timer = CreateWaitableTimer(NULL, TRUE, NULL);
  SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
  WaitForSingleObject(timer, INFINITE);
  CloseHandle(timer);
}

#else

#  include <sys/socket.h>
#  include <sys/select.h>
#  include <unistd.h>
#  include <netdb.h>
#  include <arpa/inet.h>
#  include <fcntl.h>
#  define WINSTARTUP()
#  define WINCLEANUP()
#endif

#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>

SEXP r_ping(SEXP p_destination, SEXP p_port, SEXP p_type, SEXP p_continuous,
	    SEXP p_verbose, SEXP p_count, SEXP p_timeout) {

  SEXP result;

  const char *destination;
  int port, type, continuous, verbose, count, timeout;

  struct in_addr ip_address;
  struct hostent *remote_host = NULL;
  int i = 0;

#ifdef WIN32
  WSADATA wsaData;
#endif

  /* ---------------------------------------------------------------- */
  /* Check arguments                                                  */
  /* ---------------------------------------------------------------- */

  if (LENGTH(p_destination) != 1) {
    error("destination must be a character scalar");
  }
  if (LENGTH(p_port) != 1) { error("port must be a numeric scalar"); }
  if (LENGTH(p_type) != 1) { error("type must be a character scalar"); }
  if (LENGTH(p_continuous) != 1) {
    error("continuous must be a logical scalar");
  }
  if (LENGTH(p_verbose) != 1) { error("verbose must be a logical scalar"); }
  if (LENGTH(p_count) != 1) { error("type must be a numeric scalar"); }
  if (LENGTH(p_timeout) != 1) { error("type must be a numeric scalar"); }

  destination = CHAR(STRING_ELT(AS_CHARACTER(p_destination), 0));
  port = INTEGER(AS_INTEGER(p_port))[0];
  type = INTEGER(AS_INTEGER(p_type))[0];
  if (type == 0) { type = IPPROTO_TCP; } else { type = IPPROTO_UDP; }
  continuous = INTEGER(AS_INTEGER(p_continuous))[0];
  verbose = INTEGER(AS_INTEGER(p_verbose))[0];
  count = INTEGER(AS_INTEGER(p_count))[0];
  timeout = INTEGER(AS_INTEGER(p_timeout))[0];

  /* ---------------------------------------------------------------- */
  /* Resolve host                                                     */
  /* ---------------------------------------------------------------- */

  WINSTARTUP();

  remote_host = gethostbyname(destination);
  if (!remote_host) { error("Cannot resolve host name"); }
  ip_address = *(struct in_addr*) remote_host->h_addr_list[0];

  WINCLEANUP();

  if (verbose) {
    Rprintf("TCP PING %s (%s) Port:\n", destination, inet_ntoa(ip_address),
	    port);
  }

  /* ---------------------------------------------------------------- */
  /* Main ping loop                                                   */
  /* ---------------------------------------------------------------- */

  PROTECT(result = NEW_NUMERIC(count));

  while (1) {

    /* Try to connect */

    struct timeval tv, start, stop;
    double t_start, t_stop;
    struct sockaddr_in c_address;
    fd_set read, write;
    int c_socket, ret;
    double time;
#ifdef WIN32
    u_long imode = 1;
#endif

    WINSTARTUP();

    c_socket = socket(AF_INET,
		      type == IPPROTO_UDP ? SOCK_DGRAM : SOCK_STREAM,
		      type);

    if (c_socket == -1) {
      WINCLEANUP();
      error("Cannot connect to host");
    }

    c_address.sin_addr = ip_address;
    c_address.sin_family = AF_INET;
    c_address.sin_port = htons(port);

    tv.tv_sec  = timeout / 1000000;
    tv.tv_usec = timeout % 1000000;

    gettimeofday(&start, NULL);

    /* Set non-blocking */
#ifdef WIN32
    ioctlsocket(c_socket, FIONBIO, &imode);
#else
    if (fcntl(c_socket, F_SETFL, O_NONBLOCK) < 0) {
      error("Cannot set socket to non-blocking");
    }
#endif

    ret = connect(c_socket, (const struct sockaddr*) &c_address,
		  sizeof(c_address));

#ifdef WIN32
    ret = WSAGetLastError();
    if (ret != WSAEWOULDBLOCK && ret != 0) { error("Cannot connect"); }

#else
    if (ret < 0 && errno != EINPROGRESS) { error("Cannot connect"); }
#endif

    FD_ZERO(&read);
    FD_ZERO(&write);
    FD_SET(c_socket, &read);
    FD_SET(c_socket, &write);

    ret = select(c_socket + 1, &read, &write, NULL, &tv);

    if (ret != 1) {
      close(c_socket);
      time = NA_REAL;
    } else {
      gettimeofday(&stop, NULL);
      t_start = start.tv_usec + start.tv_sec * 1000000;
      t_stop = stop.tv_usec + stop.tv_sec * 1000000;
      time = (t_stop - t_start) / 1000;
    }

    if (!FD_ISSET(c_socket, &read) && !FD_ISSET(c_socket, &write)) {
      close(c_socket);
      time = NA_REAL;
    }

    REAL(result)[i] = time;

    close(c_socket);
    WINCLEANUP();

    if (verbose) {
      if (ISNA(time)) {
	Rprintf("Request timeout for package %i\n", i + 1);
      } else {
	Rprintf("From %s: ping=%i time=%.3f ms\n", destination,
		i + 1, time);
      }
    }

    /* Are we done? */

    i++;
    if (!continuous && i == count) { break; }
    R_CheckUserInterrupt();

    /* No, wait a bit then */

    usleep((1000 - time) * 1000);
  }

  UNPROTECT(1);
  return result;
}

static const R_CallMethodDef callMethods[]  = {
  {"r_ping", (DL_FUNC) &r_ping, 7},
  {NULL, NULL, 0}
};

void R_init_parsedate(DllInfo *dll) {
  R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
  R_forceSymbols(dll, TRUE);
}
