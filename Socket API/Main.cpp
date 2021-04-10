#include "Socket/defines.h"
#include "Socket/UDP/Server.h"

// TODO: IP class, UDP Client (connection information)

int main()
{
	Socket::UDP::Server server = Socket::UDP::Server();
	std::cout << server.Bind(5005) << std::endl;

	while (true)
	{
		Socket::UDP::Message msg = server.Accept();
		std::cout << msg.sContent << std::endl;
	}
}
