#include "Client.h"

namespace Socket
{
	namespace UDP
	{
		Client::Client(IOContext &ioContext) noexcept
		{
			m_pIOContext = &ioContext;
			m_socket = socket(AF_INET, SOCK_DGRAM, 0);
		}
		
		Client::~Client() noexcept
		{
			closesocket(m_socket);
		}
		
		bool Client::Send(const std::string &sContent, const ClientInfo &info) noexcept
		{
			sockaddr_in addr = info.getSockaddr();
			return sendto(m_socket, sContent.c_str(), sContent.size(), 0, (sockaddr *)&addr, sizeof(addr)) > 0;
		}
	}
}
