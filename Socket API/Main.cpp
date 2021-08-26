#include "Socket/TCP/AsyncServer.h"

struct ConnectionData
{
	Socket::IPAddress addr{};
};

class TestServer : public Socket::TCP::AsyncServer<ConnectionData>
{
public:
	TestServer() noexcept = default;
	
	TestServer(Socket::IOContext &ioContext) noexcept
		: AsyncServer(ioContext) {}

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
	Socket::IOContext ioContext{};
	TestServer server = TestServer(ioContext);
	server.start(5000, 1);
}
