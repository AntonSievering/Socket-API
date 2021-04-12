#include "SocketConnection.h"

namespace Socket
{
	namespace TCP
	{
		SocketConnection::SocketConnection(IOContext &ioContext, const SOCKET &sock, const sockaddr_in &addr) noexcept
		{
			m_pIOContext = &ioContext;
			m_socket = sock;
			m_ipAddr = IPAddress(addr);
		}

		SocketConnection::SocketConnection(IOContext &ioContext, const IPAddress &ip, const std::size_t &port) noexcept
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
				m_bStartupSucceeded = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen) > 0;
				
				if (m_bStartupSucceeded)
					break;
			}

			// cleanup
			freeaddrinfo(result);
		}

		SocketConnection::~SocketConnection() noexcept
		{
			Close();
		}

		bool SocketConnection::Send(const std::string &msg) noexcept
		{
			return send(m_socket, msg.c_str(), msg.size(), 0) > 0;
		}

		uint64_t SocketConnection::getReceivedBytes() const noexcept
		{
			u_long n;
			(void)ioctlsocket(m_socket, FIONREAD, &n);
			
			return n;
		}

		bool SocketConnection::isAvailable() const noexcept
		{
			return getReceivedBytes() > 0;
		}

		bool SocketConnection::startupSucceeded() const noexcept
		{
			return m_bStartupSucceeded;
		}

		std::string SocketConnection::Recv() noexcept
		{
			uint64_t n = getReceivedBytes();

			if (n > 0)
			{
				std::unique_ptr<char> buffer = std::unique_ptr<char>(new char[n]{});
				(void)recv(m_socket, buffer.get(), n, 0);

				return std::string(buffer.get(), n);
			}

			return std::string();
		}

		void SocketConnection::Close() noexcept
		{
			closesocket(m_socket);
		}

		SOCKET SocketConnection::getSocket() const noexcept
		{
			return m_socket;
		}
		
		IPAddress SocketConnection::getIPAddress() const noexcept
		{
			return m_ipAddr;
		}
		
		IOContext *SocketConnection::getIOContext() const noexcept
		{
			return m_pIOContext;
		}
	}
}
