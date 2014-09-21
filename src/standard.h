
#ifndef PING_STANDARD_H
#define PING_STANDARD_H

#ifdef WIN32
#define close   closesocket
#define snprintf _snprintf
#endif

// Error codes
#define SUCCESS                         0
#define ERROR_POUTOFMEMORY              100
#define ERROR_SOCKET_CANNOTRESOLVE      101
#define ERROR_SOCKET_TIMEOUT            102
#define ERROR_SOCKET_GENERALFAILURE     103
#define ERROR_INVALIDARGUMENTS          200


#include <vector>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <cstdio>

#ifdef WIN32    // Windows specific
#include <Windows.h>
#include <WinSock2.h>
#include <time.h>
#else                   // Linux specific

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>

#endif

#include <sys/types.h>
#include <fcntl.h>

// Types
typedef const wchar_t* pcw_t;
typedef const char* pcc_t;
typedef wchar_t* pwc_t;
typedef char* pc_t;
typedef unsigned short ushort_t;

#include "stats.h"
#include "timer.h"
#include "host.h"
#include "socket.h"

#endif
