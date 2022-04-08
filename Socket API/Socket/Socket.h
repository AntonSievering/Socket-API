#pragma once

#include "defines.h"
#include "UniformUtils.h"


namespace net
{
	/**
	 * @brief Takes ownership of a socket file descriptor.
	 */
	class Socket
	{
	private:
		SOCKET m_sockfd = INVALID_SOCKET;

	public:
		Socket() noexcept = default;

		Socket(SOCKET sockfd) noexcept
		{
			m_sockfd = sockfd;
		}

		Socket(int af, int type, int protocol = 0) noexcept
		{
			m_sockfd = socket(AF_INET, SOCK_STREAM, protocol);
		}

		Socket(const Socket &) = delete;

		Socket(Socket &&rhs) noexcept
		{
			m_sockfd = rhs.m_sockfd;
			rhs.m_sockfd = INVALID_SOCKET;
		}

		virtual ~Socket() noexcept
		{
			net::Uniform::closesocket(m_sockfd);
		}

	public:
		Socket &operator=(const Socket &) = delete;

		Socket &operator=(Socket &&rhs) noexcept
		{
			std::swap(m_sockfd, rhs.m_sockfd);
		}

	public:
		operator SOCKET() const noexcept
		{
			return m_sockfd;
		}

		bool isValid() const noexcept
		{
			return m_sockfd != INVALID_SOCKET;
		}
	};
}
