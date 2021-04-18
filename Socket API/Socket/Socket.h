#pragma once
#include "defines.h"

namespace Socket
{
	class Socket
	{
	private:
		SOCKET m_nSocket{};

	public:
		Socket() noexcept = default;

		Socket(SOCKET sock) noexcept
		{
			m_nSocket = sock;
		}

		~Socket() noexcept
		{
			closesocket(m_nSocket);
		}

	public:
		SOCKET getSocket() const noexcept
		{
			return m_nSocket;
		}
	};
}
