#pragma once
#include "../defines.h"
#include "../IPAddress.h"
#include "../IOContext.h"
#include "ClientInfo.h"
#include "../Socket.h"

namespace Socket
{
	namespace UDP
	{
		struct Message
		{
		public:
			std::string sContent;
			ClientInfo addr;
		};

		class Server
		{
		private:
			constexpr static const int s_bufferLength = 1024;

		private:
			IOContext              *m_pIOContext          = nullptr;
			int                     m_nCachedClientLength = sizeof(m_cachedClient);
			std::shared_ptr<Socket> m_socket{};
			sockaddr_in             m_cachedClient{};
			char                    m_buffer[s_bufferLength]{};

		public:
			Server() noexcept = default;
			
			Server(IOContext &ioContext) noexcept
			{
				m_pIOContext = &ioContext;
			}
			
		public:
			bool Bind(const std::size_t &port) noexcept
			{
				m_socket = std::shared_ptr<Socket>(new Socket(socket(AF_INET, SOCK_DGRAM, 0)));

				sockaddr_in server{};
				server.sin_addr.S_un.S_addr = ADDR_ANY;
				server.sin_family = AF_INET;
				server.sin_port = htons(port);

				if (bind(m_socket.get()->getSocket(), (sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
					return false;

				return m_socket.get()->getSocket() != SOCKET_ERROR;
			}

			Message Accept() noexcept
			{
				ZeroMemory(&m_cachedClient, m_nCachedClientLength);
				std::string sContent;

				int nReceivedBytes = s_bufferLength;
				while (nReceivedBytes == s_bufferLength)
				{
					ZeroMemory(m_buffer, s_bufferLength);
					nReceivedBytes = recvfrom(m_socket.get()->getSocket(), m_buffer, s_bufferLength, 0, (sockaddr *)&m_cachedClient, &m_nCachedClientLength);

					if (nReceivedBytes > 0)
						sContent.append(m_buffer, nReceivedBytes);
				}

				Message message{};
				message.addr = ClientInfo(m_cachedClient);
				message.sContent = sContent;

				return message;
			}

			bool Send(const std::string &sContent, const ClientInfo &info) noexcept
			{
				sockaddr_in addr = info.getSockaddr();
				return sendto(m_socket.get()->getSocket(), sContent.c_str(), sContent.size(), 0, (sockaddr *)&addr, sizeof(addr)) > 0;
			}
		};
	}
}
