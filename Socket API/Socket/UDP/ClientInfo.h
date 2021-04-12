#pragma once
#include "../defines.h"
#include "../IPAddress.h"

namespace Socket
{
	namespace UDP
	{
		class ClientInfo
		{
		private:
			sockaddr_in m_serverConnectionInfo{};

		public:
			ClientInfo() noexcept = default;
			ClientInfo(const IPAddress &ip, const uint16_t &port) noexcept;

		public:
			sockaddr_in getSockaddr() const noexcept;
		};
	}
}
