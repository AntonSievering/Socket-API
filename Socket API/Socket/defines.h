#pragma once

#include <string>
#include <iostream>
#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <cstring>

#ifdef _WIN32
	#include <WinSock2.h>
	#include <ws2tcpip.h>

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

	using SOCKET = int;
	#define INVALID_SOCKET 0
	#define ZeroMemory(dest, size) std::memset(dest, 0x00, size)

#endif // _WIN32

#define DEFAULT_BUFLEN 1024
