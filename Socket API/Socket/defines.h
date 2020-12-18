#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>
#include <iostream>
#include <functional>
#include <queue>
#include <thread>
#include <mutex>

// Linking
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 1024
