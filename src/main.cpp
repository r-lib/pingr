#include "standard.h"

using namespace std;

namespace ping {

int		exitCode	= 0;
stats_c	stats;

int main(const char *Destination, int Port, int Type, bool Continous,
	 int Count, int Timeout) {

  host_c		host;
	int			result;

	stats.Attempts	= 0;
	stats.Connects	= 0;
	stats.Failures	= 0;
	stats.Minimum	= 0.0;
	stats.Maximum	= 0.0;
	stats.Total		= 0.0;

	result = socket_c::Resolve(Destination, host);

	if (result == SUCCESS)
	{
		socket_c::SetPortAndType(Port, Type, host);

	}

	unsigned int	i		= 0;

	double			time	= 0.0;

	while (Continous || i < (unsigned int)Count)
	{
		result = socket_c::Connect(host, Timeout, time);

		stats.Attempts++;

		if (result == SUCCESS)
		{
			stats.Connects++;
			stats.Total += time;
			stats.UpdateMaxMin(time);

		}
		else
		{
			exitCode = 1;
			stats.Failures++;

		}

		#ifdef WIN32	// Windows cannot sleep to that accuracy (I think!)
			if ((int)time < 1000) Sleep((1000 - (int)time));
		#else
			if ((int)time < 1000) usleep((1000 - (int)time) * 1000);
		#endif

		i++;
	}


	return exitCode;
	}

} // namespace
