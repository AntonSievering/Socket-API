#pragma once

#include "../defines.h"
#include "../IPAddress.h"
#include "../Socket.h"
#include <algorithm>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif


namespace net
{
	namespace TCP
	{
		class SocketConnection
		{
		private:
			Socket    m_socket            = INVALID_SOCKET;
			bool      m_bIsConnected      = false;
			IPAddress m_ipAddr{};

		public:
			SocketConnection() noexcept = default;

			SocketConnection(Socket &&sock, const sockaddr_in &addr) noexcept
			{
				m_socket = std::move(sock);
				m_ipAddr = IPAddress(addr);
				m_bIsConnected = true;
				disableBlockingMode();
			}

			SocketConnection(const IPAddress &ip, const std::size_t &port) noexcept
			{
				m_ipAddr = ip;

				addrinfo *result, hints;
				std::memset(&hints, 0x00, sizeof(hints));
				hints.ai_family = AF_UNSPEC;
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_protocol = IPPROTO_TCP;

				(void)getaddrinfo(ip.asString().c_str(), std::to_string(port).c_str(), &hints, &result);

				for (addrinfo *ptr = result; ptr != nullptr and !m_bIsConnected; ptr = ptr->ai_next)
				{
					m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
					m_bIsConnected = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == 0;
				}

				freeaddrinfo(result);
				disableBlockingMode();
			}

		private:
			void disableBlockingMode()
			{
				u_long iMode = 0;
				Uniform::ioctl_socket(m_socket, FIONBIO, &iMode);
			}

		public:
			bool Send(const std::string &msg) noexcept
			{
				if (isConnected())
				{
					int n = send(m_socket, msg.c_str(), (int)msg.size(), 0);

					if (n == -1)
					{
						m_bIsConnected = false;
						return false;
					}
					return true;
				}
				else
					return false;
			}

			std::string Recv() noexcept
			{
				if (m_bIsConnected)
				{
					std::string s;
					uint32_t nBytes = getIncomingBytes();
					s.resize(nBytes);
					int length = recv(m_socket, s.data(), nBytes, 0);

					if (length < 1)
					{
						m_bIsConnected = false;
						return std::string();
					}

					return s;
				}

				return std::string();
			}

			uint32_t getIncomingBytes() const noexcept
			{
				u_long n{};
				Uniform::ioctl_socket(m_socket, FIONREAD, &n);

				return n;
			}

			bool isAvailable() const noexcept
			{
				return getIncomingBytes() > 0;
			}

			bool isConnected() const noexcept
			{
				return m_bIsConnected;
			}

			IPAddress getIPAddress() const noexcept
			{
				return m_ipAddr;
			}
		};
	}
}
