#include "Socket/TCP/Client.h"

int main()
{
	Socket::TCP::AsyncClient client;
	std::string msg = "Das ist ein Test";

	if (client.Connect("192.168.178.52", 5005))
		while (client.isConnected())
			client.Send(msg);

	while (client.isAvailable())
		std::cout << client.Recv() << std::endl;

	return 0;
}