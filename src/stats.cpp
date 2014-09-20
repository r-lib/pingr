#include "standard.h"

using namespace std;

namespace ping {

double stats_c::Average()
{
	return (this->Total / this->Attempts);
}


void stats_c::UpdateMaxMin(double value)
{
	if (this->Attempts == 1)
	{
		this->Minimum	= value;
		this->Maximum	= value;

		return;
	}

	if (value < this->Minimum) this->Minimum = value;
	if (value > this->Maximum) this->Maximum = value;
}


int stats_c::GetStatisticsString(pcc_t str)
{
	int		length	= 0;
	pcc_t	format	= "\n"
	  "Connection statistics:\n"
	  "\tAttempted = $%d$, Connected = $%d$, Failed = $%d$ ($%.2f%%$)\n"
	  "Approximate connection times:\n"
	  "\tMinimum = $%.2fms$, Maximum = $%.2fms$, Average = $%.2fms$\n";

	double	failPercent	= ((double)this->Failures / (double)this->Attempts) * 100;

	length = snprintf(NULL, 0, format, this->Attempts, this->Connects, this->Failures, failPercent, this->Minimum, this->Maximum, this->Average());
	if (str != NULL) sprintf((pc_t)str, format, this->Attempts, this->Connects, this->Failures, failPercent, this->Minimum, this->Maximum, this->Average());
	
	return length;
}

} // namespace
