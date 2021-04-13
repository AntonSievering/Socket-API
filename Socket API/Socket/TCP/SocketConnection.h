#include "../defines.h"
#include "../IPAddress.h"
#include "../IOContext.h"
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
			IOContext *m_pIOContext        = nullptr;
			SOCKET     m_socket            = INVALID_SOCKET;
			bool       m_bStartupSucceeded = false;

		public:
			IPAddress  m_ipAddr{};

		public:
			SocketConnection() noexcept = default;

			SocketConnection(IOContext &ioContext, const SOCKET &sock, const sockaddr_in &addr) noexcept
			{
				m_pIOContext = &ioContext;
				m_socket = sock;
				m_ipAddr = IPAddress(addr);
			}

			SocketConnection(IOContext &ioContext, const IPAddress &ip, const std::size_t &port) noexcept
			{
				m_pIOContext = &ioContext;
				m_ipAddr = ip;

				// IPv4 TCP connection
				addrinfo *result, hints;
				ZeroMemory(&hints, sizeof(hints));
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

				// cleanup
				freeaddrinfo(result);
			}

			~SocketConnection() noexcept
			{
				Close();
			}

		public:
			bool Send(const std::string &msg) noexcept
			{
				return send(m_socket, msg.c_str(), msg.size(), 0) > 0;
			}

			std::string Recv(const uint32_t &nMax = 65536) noexcept
			{
				uint32_t n = getReceivedBytes();
				n = std::min(n, nMax);

				if (n > 0)
				{
					std::unique_ptr<char> buffer = std::unique_ptr<char>(new char[n]{});
					(void)recv(m_socket, buffer.get(), n, 0);

					return std::string(buffer.get(), n);
				}

				return std::string();
			}
			
			void Close() noexcept
			{
				closesocket(m_socket);
			}

			uint32_t getReceivedBytes() const noexcept
			{
				u_long n;
				(void)ioctlsocket(m_socket, FIONREAD, &n);

				return n;
			}

			bool isAvailable() const noexcept
			{
				return getReceivedBytes() > 0;
			}
			
			bool startupSucceeded() const noexcept
			{
				return m_bStartupSucceeded;
			}
			
			SOCKET getSocket() const noexcept
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
