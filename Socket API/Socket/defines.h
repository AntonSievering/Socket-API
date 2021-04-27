#pragma once

#include <string>
#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <cstring>
#include <queue>
#include <memory>

#ifdef _WIN32
	#include <WinSock2.h>
	#include <ws2tcpip.h>
	#include "IOContext_Win32.h"

	#pragma comment (lib, "Ws2_32.lib")
	#pragma comment (lib, "Mswsock.lib")
	#pragma comment (lib, "AdvApi32.lib")

#elif __linux__
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <sys/ioctl.h>
	#include "IOContext_Linux.h"

	using SOCKET = int;
	#define INVALID_SOCKET 0

#endif // _WIN32
