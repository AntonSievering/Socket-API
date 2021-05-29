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


namespace Socket
{
	namespace TCP
	{
		class SocketConnection
		{
		private:
			IOContext            *m_pIOContext        = nullptr;
			Socket                m_socket            = INVALID_SOCKET;
			bool                  m_bStartupSucceeded = false;
			bool                  m_bIsConnected      = false;
			std::shared_ptr<char> m_buffer            = std::shared_ptr<char>(new char[65536]{});

		public:
			IPAddress             m_ipAddr{};

		public:
			SocketConnection() noexcept = default;

			SocketConnection(IOContext &ioContext, const Socket &sock, const sockaddr_in &addr) noexcept
			{
				m_pIOContext = &ioContext;
				m_socket = sock;
				m_ipAddr = IPAddress(addr);
				m_bIsConnected = true;
				disableBlockingMode();
			}

			SocketConnection(IOContext &ioContext, const IPAddress &ip, const std::size_t &port) noexcept
			{
				m_pIOContext = &ioContext;
				m_ipAddr = ip;

				// IPv4 TCP connection
				addrinfo *result, hints;
				std::memset(&hints, 0x00, sizeof(hints));
				hints.ai_family = AF_UNSPEC;
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_protocol = IPPROTO_TCP;

				// convert string ip into 
				(void)getaddrinfo(ip.asString().c_str(), std::to_string(port).c_str(), &hints, &result);

				// try to connect to one of the possible addresses
				for (addrinfo *ptr = result; ptr != nullptr; ptr = ptr->ai_next)
				{
					m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
					m_bStartupSucceeded = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == 0;

					if (m_bStartupSucceeded)
						break;
				}

				m_bIsConnected = m_bStartupSucceeded;

				// cleanup
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

			std::string Recv(const uint32_t &nMax = 65536) noexcept
			{
				if (m_bIsConnected)
				{
					int length = recv(m_socket, m_buffer.get(), 65536, 0);

					if (length < 1)
					{
						m_bIsConnected = false;
						return std::string();
					}

					return std::string(m_buffer.get(), length);
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

			bool startupSucceeded() const noexcept
			{
				return m_bStartupSucceeded;
			}

			bool isConnected() const noexcept
			{
				return m_bIsConnected;
			}

			Socket getSocket() const noexcept
			{
				return m_socket;
			}

			IPAddress getIPAddress() const noexcept
			{
				return m_ipAddr;
			}

			IOContext *getIOContext() const noexcept
			{
				return m_pIOContext;
			}
		};
	}
}
