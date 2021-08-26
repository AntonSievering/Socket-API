#include "Socket/TCP/AsyncServer.h"

struct ConnectionData
{
	Socket::IPAddress addr{};
};

class TestServer : public Socket::TCP::AsyncServer<ConnectionData>
{
public:
	TestServer() noexcept = default;

public:
	bool acceptConnection(const Socket::IPAddress &addr, ConnectionData &data) noexcept override
	{
		std::cout << addr << ": connection established" << std::endl;
		data.addr = addr;

		return true;
	}
	
	std::string handleQuery(const std::string &sQuery, ConnectionData &data) noexcept override
	{
		std::cout << data.addr << ": " << sQuery << std::endl;

		return "test answer message";
	}

	void closeConnection(ConnectionData &data) noexcept override
	{
		std::cout << data.addr << ": connection closed\n";
	}
};

int main()
{
	TestServer server{};
	server.start(5000, 1);
}
