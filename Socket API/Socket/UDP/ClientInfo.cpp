#include "ClientInfo.h"

namespace Socket
{
	namespace UDP
	{
		ClientInfo::ClientInfo(const IPAddress &ip, const uint16_t &port) noexcept
		{
			m_serverConnectionInfo.sin_family = AF_INET;
			m_serverConnectionInfo.sin_port   = htons(port);

			m_serverConnectionInfo.sin_addr.S_un.S_un_b.s_b1 = ip.addr[0];
			m_serverConnectionInfo.sin_addr.S_un.S_un_b.s_b2 = ip.addr[1];
			m_serverConnectionInfo.sin_addr.S_un.S_un_b.s_b3 = ip.addr[2];
			m_serverConnectionInfo.sin_addr.S_un.S_un_b.s_b4 = ip.addr[3];
		}
		
		sockaddr_in ClientInfo::getSockaddr() const noexcept
		{
			return m_serverConnectionInfo;
		}
	}
}
