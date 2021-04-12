#include "Socket/UDP/Client.h"

int main()
{
	Socket::IOContext ioContext{};
	Socket::UDP::ClientInfo clientInfo = Socket::UDP::ClientInfo(Socket::IPAddress("192.168.178.52"), 5005);
	Socket::UDP::Client client = Socket::UDP::Client(ioContext);
	
	for (int i = 0;; i++)
	{
		client.Send("test", clientInfo);

		if (i % 100000 == 0)
			std::cout << i / 100000 << "00k" << std::endl;
	}

	return 0;
}
