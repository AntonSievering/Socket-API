#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Socket/defines.h"
#include "Socket/IOContext.h"

int main2()
{
	Socket::IOContext ioContext{};

	sockaddr_in server{};
	server.sin_family = AF_INET;
	server.sin_port = htons(5005);
	inet_pton(AF_INET, "192.168.178.52", &server.sin_addr);

	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	std::string sContent =
		"Das ist ein String mit sehr vielen unnötigen Zeichen. eigenlich soll der auch nur lang sein und erfüllt"
		"damit keinen Zweck, ausser viele Daten über UDP zu senden. Liebe Grüße...";

	if (out != INVALID_SOCKET)
	{
		std::cout << "socket created" << std::endl;
		for (int i = 0; true; i++)
		{
			sendto(out, sContent.c_str(), sContent.size(), 0, (sockaddr *)&server, sizeof(server));

			if (i % 1000 == 0)
				std::cout << i << "\n";
		}
	}
}

#include "Socket/TCP/SocketConnection.h"

int main()
{
	Socket::IOContext ioContext{};
	Socket::TCP::SocketConnection client = Socket::TCP::SocketConnection(ioContext, "192.168.178.52", 5005);

	while (true)
		client.Recv();
}
