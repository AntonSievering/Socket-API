#include "SocketConnection.h"

namespace Socket
{
	namespace TCP
	{
		SocketConnection::SocketConnection() noexcept
		{
			m_socket = SOCKET();
			ZeroMemory(buf, DEFAULT_BUFLEN);
		}

		SocketConnection::SocketConnection(const SOCKET &sock, const sockaddr_in &addr) noexcept
		{
			m_socket = sock;
			ZeroMemory(buf, DEFAULT_BUFLEN);

			// Client Address
			clientAddr[0] = addr.sin_addr.S_un.S_un_b.s_b1;
			clientAddr[1] = addr.sin_addr.S_un.S_un_b.s_b2;
			clientAddr[2] = addr.sin_addr.S_un.S_un_b.s_b3;
			clientAddr[3] = addr.sin_addr.S_un.S_un_b.s_b4;
		}

		SocketConnection::SocketConnection(const std::string &ip, const std::size_t &port) noexcept
		{
			initializeWinsock();

			addrinfo *result, hints;
			int nResult{};

			// IPv4 TCP connection
			ZeroMemory(&hints, sizeof(hints));
			ZeroMemory(buf, DEFAULT_BUFLEN);
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			// convert string ip into 
			nResult = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &result);

			// try to connect to one of the possible addresses
			for (addrinfo *ptr = result; ptr != nullptr; ptr = ptr->ai_next)
			{
				m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				nResult = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
				
				if (nResult >= 0)
					break;
			}

			// cleanup
			freeaddrinfo(result);
		}

		SocketConnection::~SocketConnection() noexcept
		{
			closesocket(m_socket);
		}

		void SocketConnection::Send(const std::string &msg) noexcept
		{
			send(m_socket, msg.c_str(), msg.size(), 0);
		}

		uint64_t SocketConnection::getReceivedBytes() const noexcept
		{
			u_long n;
			ioctlsocket(m_socket, FIONREAD, &n);
			
			return n;
		}

		bool SocketConnection::isAvailable() const noexcept
		{
			return getReceivedBytes() > 0;
		}

		std::string SocketConnection::RecvNonBlocking() noexcept
		{
			uint64_t n = getReceivedBytes();

			if (n > 0)
			{
				char *buffer = new char[n];
				recv(m_socket, buffer, n, 0);
				std::string msg = std::string(buffer, n);

				return msg;
			}

			return std::string();
		}

		std::string SocketConnection::Recv(const uint64_t &nMaxLength) noexcept
		{
			char *buffer = new char[nMaxLength];
			int nReceivedLength = recv(m_socket, buffer, nMaxLength, 0);
			return std::string(buffer, nReceivedLength);
		}

		SOCKET SocketConnection::getSocket() const noexcept
		{
			return m_socket;
		}
		
		void SocketConnection::initializeWinsock() noexcept
		{
			auto winsockInitialized = []() -> bool
			{
				SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

				if (sock == INVALID_SOCKET)
					return false;

				closesocket(sock);
				return true;
			};

			if (!winsockInitialized())
			{
				WSAData data;
				(void)WSAStartup(MAKEWORD(2, 2), &data);
			}
		}
	}
}
