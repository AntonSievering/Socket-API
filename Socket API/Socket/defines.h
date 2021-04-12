#pragma once

#include <string>
#include <iostream>
#include <functional>
#include <queue>
#include <thread>
#include <mutex>

#ifdef _WIN32
	#include <WinSock2.h>
	#include <ws2tcpip.h>

	#pragma comment (lib, "Ws2_32.lib")
	#pragma comment (lib, "Mswsock.lib")
	#pragma comment (lib, "AdvApi32.lib")

#elif __linux__
	#include <sys/socket.h>

#endif // _WIN32

#define DEFAULT_BUFLEN 1024
