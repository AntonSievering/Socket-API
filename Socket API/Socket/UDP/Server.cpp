#include "Server.h"

namespace Socket
{
	namespace UDP
	{
		// Server implementation

		Server::Server() noexcept
		{
		}

		Server::~Server() noexcept
		{
			closesocket(m_socket);
			WSACleanup();
		}

		bool Server::Bind(const std::size_t &port) noexcept
		{
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
				return false;

			m_socket = socket(AF_INET, SOCK_DGRAM, 0);

			sockaddr_in server{};
			server.sin_addr.S_un.S_addr = ADDR_ANY;
			server.sin_family = AF_INET;
			server.sin_port = htons(port);

			if (bind(m_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
				return false;

			return m_socket != SOCKET_ERROR;
		}

		Message Server::Accept() noexcept
		{
			ZeroMemory(&m_cachedClient, m_nCachedClientLength);
			std::string sContent;

			int nReceivedBytes = s_bufferLength;
			while (nReceivedBytes == s_bufferLength)
			{
				ZeroMemory(m_buffer, s_bufferLength);
				nReceivedBytes = recvfrom(m_socket, m_buffer, s_bufferLength, 0, (sockaddr *)&m_cachedClient, &m_nCachedClientLength);

				if (nReceivedBytes > 0)
					sContent.append(m_buffer, nReceivedBytes);
			}

			Message message{};
			message.addr[0] = m_cachedClient.sin_addr.S_un.S_un_b.s_b1;
			message.addr[1] = m_cachedClient.sin_addr.S_un.S_un_b.s_b2;
			message.addr[2] = m_cachedClient.sin_addr.S_un.S_un_b.s_b3;
			message.addr[3] = m_cachedClient.sin_addr.S_un.S_un_b.s_b4;
			message.sContent = sContent;

			return message;
		}
	}
}
