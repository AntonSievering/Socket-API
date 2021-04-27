#include "Socket/TCP/AsyncServer.h"

class TestServer : public Socket::TCP::AsyncServer
{
public:
	TestServer() noexcept = default;
	
	TestServer(Socket::IOContext &ioContext) noexcept
		: AsyncServer(ioContext) {}

public:
	std::string handleQuery(const std::string &sQuery) noexcept
	{
		return "std::string()";
	}
};

int main()
{
	Socket::IOContext ioContext{};
	TestServer server = TestServer(ioContext);
	server.start(5000, 1);
}
