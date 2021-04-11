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
			Server(IOContext &ioContext) noexcept;
			~Server() noexcept;

		public:
			bool Bind(const std::size_t &port) noexcept;
			Message Accept() noexcept;
		};
	}
}
