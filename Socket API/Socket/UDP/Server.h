#pragma once
#include "../defines.h"
#include "../IPAddress.h"
#include "../IOContext.h"

namespace Socket
{
	namespace UDP
	{
		struct Message
		{
		public:
			std::string sContent;
			IPAddress addr;
		};

		class Server
		{
		private:
			constexpr static const int s_bufferLength = 1024;

		private:
			IOContext  *m_pIOContext          = nullptr;
			SOCKET      m_socket              = INVALID_SOCKET;
			int         m_nCachedClientLength = sizeof(m_cachedClient);
			sockaddr_in m_cachedClient{};
			char        m_buffer[s_bufferLength]{};

		public:
			Server() noexcept = default;
			
			Server(IOContext &ioContext) noexcept
			{
				m_pIOContext = &ioContext;
			}
			
			~Server() noexcept
			{
				closesocket(m_socket);
			}

		public:
			bool Bind(const std::size_t &port) noexcept
			{
				m_socket = socket(AF_INET, SOCK_DGRAM, 0);

				sockaddr_in server{};
				server.sin_addr.S_un.S_addr = ADDR_ANY;
				server.sin_family = AF_INET;
				server.sin_port = htons(port);

				if (bind(m_socket, (sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
					return false;

				return m_socket != SOCKET_ERROR;
			}

			Message Accept() noexcept
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
		};
	}
}
