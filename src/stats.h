#include "standard.h"

namespace ping {

class stats_c
{
	public:
		double	Minimum;
		double	Maximum;
		double	Total;
		double	Average();

		int		Attempts;
		int		Connects;
		int		Failures;

		void	UpdateMaxMin(double value);
		int		GetStatisticsString(pcc_t stats);
};

}
