#pragma once
#include "../defines.h"

namespace Socket
{
	namespace UDP
	{
		struct Message
		{
		public:
			std::string sContent;
			uint8_t addr[4];
		};

		// Server - manages the SOCKETS
		class Server
		{
		private:
			WSAData wsaData;
			SOCKET m_socket = INVALID_SOCKET;
			sockaddr_in m_cachedClient;
			int m_nCachedClientLength = sizeof(m_cachedClient);
			constexpr static const int s_bufferLength = 1024;
			char m_buffer[s_bufferLength];

		public:
			Server()                           noexcept;
			~Server()                          noexcept;

		public:
			bool Bind(const std::size_t &port) noexcept;
			Message Accept()                   noexcept;
		};
	}
}
