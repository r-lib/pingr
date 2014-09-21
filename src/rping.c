
#include <R.h>
#include <Rdefines.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>

SEXP r_ping(SEXP p_destination, SEXP p_port, SEXP p_type, SEXP p_continuous,
	    SEXP p_count, SEXP p_timeout) {

  SEXP result;

  const char *destination;
  int port, type, continuous, count, timeout;

  struct in_addr ip_address;
  struct hostent *remote_host = NULL;
  const char *host_name;
  int is_ip;
  int i = 0;

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
  if (LENGTH(p_count) != 1) { error("type must be a numeric scalar"); }
  if (LENGTH(p_timeout) != 1) { error("type must be a numeric scalar"); }

  destination = CHAR(STRING_ELT(AS_CHARACTER(p_destination), 0));
  port = INTEGER(AS_INTEGER(p_port))[0];
  type = INTEGER(AS_INTEGER(p_type))[0];
  if (type == 0) { type = IPPROTO_TCP; } else { type = IPPROTO_UDP; }
  continuous = INTEGER(AS_INTEGER(p_continuous))[0];
  count = INTEGER(AS_INTEGER(p_count))[0];
  timeout = INTEGER(AS_INTEGER(p_timeout))[0];

  /* ---------------------------------------------------------------- */
  /* Resolve host                                                     */
  /* ---------------------------------------------------------------- */

  remote_host = gethostbyname(destination);
  if (!remote_host) { error("Cannot resolve host name"); }
  host_name = remote_host->h_name;
  is_ip = !strcmp(host_name, destination);
  ip_address = *(struct in_addr*) remote_host->h_addr_list[0];

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
    int ret;
    double time;

    int c_socket = socket(AF_INET,
			  type == IPPROTO_UDP ? SOCK_DGRAM : SOCK_STREAM,
			  type);

    if (c_socket == -1) { error("Cannot connect to host"); }

    c_address.sin_addr = ip_address;
    c_address.sin_family = AF_INET;
    c_address.sin_port = htons(port);

    tv.tv_sec  = timeout / 1000000;
    tv.tv_usec = timeout % 1000000;

    gettimeofday(&start, NULL);

    connect(c_socket, (const struct sockaddr*) &c_address,
	    sizeof(c_address));

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

    /* Are we done? */

    i++;
    if (!continuous && i == count) { break; }

    /* No, wait a bit then */

    usleep((1000 - time) * 1000);
  }

  UNPROTECT(1);
  return result;
}
