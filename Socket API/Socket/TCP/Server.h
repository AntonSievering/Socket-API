#pragma once
#include "../defines.h"
#include "SocketConnection.h"
#include "../Socket.h"

namespace Socket
{
	namespace TCP
	{
		class Server
		{
		private:
			IOContext *m_ioContext = nullptr;
			Socket     m_socket    = INVALID_SOCKET;
			int        m_nResult   = 0;

		public:
			Server() noexcept = default;

			Server(IOContext &context) noexcept
			{
				m_ioContext = &context;
			}
			
		public:
			bool Bind(const std::size_t &port) noexcept
			{
				addrinfo hints{}, *result;
				hints.ai_family = AF_INET;
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_protocol = IPPROTO_TCP;
				hints.ai_flags = AI_PASSIVE;

				getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
				m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
				bind(m_socket, result->ai_addr, (int)result->ai_addrlen);
				freeaddrinfo(result);
				listen(m_socket, SOMAXCONN);

				return m_socket != SOCKET_ERROR;
			}
			
			[[nodiscard]] SocketConnection *Accept() noexcept
			{
				Socket sock = INVALID_SOCKET;
				sockaddr_in addr{};
				
				// TODO: Linux: accept() on failure == -1?
				// Windows SOCKET is uint64_t?

				while (sock != INVALID_SOCKET)
				{
					int addrlen = sizeof(sockaddr_in);
					Socket sock = accept(m_socket, (sockaddr *)&addr, &addrlen);
				}

				return new SocketConnection(*m_ioContext, sock, addr);
			}
		};
	}
}