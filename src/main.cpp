#include "standard.h"

#include <R.h>

using namespace std;
using namespace ping;

extern "C" {

 int r_ping_main(const char *Destination, int Port, int Type,
		 int Continuous, int Count, int Timeout,
		 double *Values) {

    host_c host;
    stats_c stats;
    int result;
    int exitCode;

    if (Type == 0) { Type = IPPROTO_TCP; } else { Type = IPPROTO_UDP; }

    stats.Attempts = 0;
    stats.Connects = 0;
    stats.Failures = 0;
    stats.Minimum  = 0.0;
    stats.Maximum  = 0.0;
    stats.Total    = 0.0;

    result = socket_c::Resolve(Destination, host);

    if (result == SUCCESS) {
      socket_c::SetPortAndType(Port, Type, host);
    }

    unsigned int i = 0;

    double time = 0.0;

    while (1) {
      result = socket_c::Connect(host, Timeout, time);

      stats.Attempts++;

      if (result == SUCCESS) {
        stats.Connects++;
        stats.Total += time;
        stats.UpdateMaxMin(time);
	Values[i] = time;

      } else {
        exitCode = 1;
        stats.Failures++;
	Values[i] = -1;
      }

      R_CheckUserInterrupt();

      i++;
      if (! Continuous && i == (unsigned int) Count) { break; }

#ifdef WIN32    // Windows cannot sleep to that accuracy (I think!)
      if ((int)time < 1000) Sleep((1000 - (int)time));
#else
      if ((int)time < 1000) usleep((1000 - (int)time) * 1000);
#endif

    }

    return exitCode;
  }

} // extern C
