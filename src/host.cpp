#include "standard.h"


using namespace std;

namespace ping {

  pcc_t host_c::IPAddressString() {
    return host_c::GetIPAddressAsString(this->IPAddress);
  }


  int   host_c::GetSuccessfulConnectionString(pcc_t str, double time) {
    int         length  = 0;
    pcc_t       format  = "Connected to $%s$: "
      "time=$%.2fms$ protocol=$%s$ port=$%d$";

    length = snprintf(NULL, 0, format, this->IPAddressString(), time,
                      socket_c::GetFriendlyTypeName(this->Type),
                      this->Port);
    if (str != NULL) sprintf((pc_t)str, format, this->IPAddressString(),
                             time, socket_c::GetFriendlyTypeName(this->Type),
                             this->Port);

    return length;
  }


  int host_c::GetConnectInfoString(pcc_t str) {
    int         length  = 0;
    pcc_t       format  = NULL;

    if (this->HostIsIP) {
      format = "Connecting to $%s$ on $%s %d$:";

      length = snprintf(NULL, 0, format, this->Hostname,
                        socket_c::GetFriendlyTypeName(this->Type),
                        this->Port);
      if (str != NULL) sprintf((pc_t)str, format, this->Hostname,
                               socket_c::GetFriendlyTypeName(this->Type),
                               this->Port);
    } else {
      format = "Connecting to $%s$ [$%s$] on $%s %d$:";

      length = snprintf(NULL, 0, format, this->Hostname,
                        this->IPAddressString(),
                        socket_c::GetFriendlyTypeName(this->Type),
                        this->Port);
      if (str != NULL) sprintf((pc_t)str, format, this->Hostname,
                               this->IPAddressString(),
                               socket_c::GetFriendlyTypeName(this->Type),
                               this->Port);
    }

    return length;
  }


  pcc_t host_c::GetIPAddressAsString(in_addr ipAddress) {
    return inet_ntoa(ipAddress);
  }

} // namespace
