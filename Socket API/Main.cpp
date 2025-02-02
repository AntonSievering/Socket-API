#include "Socket/Socket.h"
#include "Socket/IPAddress.h"
#include "Socket/TCP/SocketConnection.h"
#include <iostream>

int main()
{
	net::IPAddress ip = net::IPAddress(127, 0, 0, 1);
	std::cout << ip << std::endl;
	net::TCP::SocketConnection conn(ip, 12345);
	
	std::string buf;
	buf.resize(512);

	buf[0] = 0; buf[1] = 1;
	for (int i = 2; i < sizeof(buf); i++)
		buf[i] = buf[i - 1] + buf[i - 2];

	for (int i = 0; i < 100; i++)
	{
		conn.Send(buf);

		while (not conn.isAvailable())
		{
			if (not conn.isConnected())
			{
				std::cout << "socket disconnected\n";
				return -1;
			}
		}
		
		std::string return_data = conn.Recv();

		auto check_data = [](const char *data1, const char *data2) -> bool
			{
				for (int j = 0; j < sizeof(data1); j++)
					if (data1[j] != data2[j])
						return false;
				return true;
			};

		if (check_data(buf.c_str(), return_data.c_str()))
		{
			std::cout << i << " rx ok\n";
		}
		else
		{
			std::cout << "receive error\n";
			return -1;
		}
	}

	return 0;
}
