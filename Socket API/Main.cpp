#include "Socket/TCP/Server.h"

class AsyncServer
{
private:
	Socket::TCP::Server m_acceptingServer;
	Socket::IOContext *ioContext;
};

int main()
{
	Socket::IOContext ioContext{};
}
