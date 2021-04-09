#include "Server.h"

namespace Socket
{
	namespace TCP
	{
		// Server implementation

		Server::Server() noexcept
		{
			ZeroMemory(&m_hints, sizeof(m_hints));
			m_hints.ai_family   = AF_INET;
			m_hints.ai_socktype = SOCK_STREAM;
			m_hints.ai_protocol = IPPROTO_TCP;
			m_hints.ai_flags    = AI_PASSIVE;

			m_nResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		}

		Server::~Server() noexcept
		{
			closesocket(m_socket);
			WSACleanup();
		}

		bool Server::Bind(const std::size_t &port) noexcept
		{
			addrinfo *result;
			getaddrinfo(NULL, std::to_string(port).c_str(), &m_hints, &result);
			m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			bind(m_socket, result->ai_addr, (int)result->ai_addrlen);
			freeaddrinfo(result);
			listen(m_socket, SOMAXCONN);

			return m_socket != SOCKET_ERROR;
		}

		SocketConnection *Server::Accept() noexcept
		{
			sockaddr_in addr{};
			int addrlen = sizeof(sockaddr_in);
			SOCKET sock = accept(m_socket, (sockaddr*)&addr, &addrlen);

			return new SocketConnection(sock, addr);
		}


		// Auto Managed Server implementation

		AutoManagedServer::AutoManagedServer() noexcept
		{
		}

		AutoManagedServer::~AutoManagedServer() noexcept
		{
			if (m_accepter.joinable())
				m_accepter.detach();

			if (m_cleanup.joinable())
				m_cleanup.detach();
		}

		void AutoManagedServer::start(const std::size_t &port) noexcept
		{
			m_sock = Socket::TCP::Server();
			m_sock.Bind(port);

			m_cleanup  = std::thread(&AutoManagedServer::cleanup,  this);
			m_accepter = std::thread(&AutoManagedServer::mainloop, this);
		}
		
		void AutoManagedServer::cleanup() noexcept
		{
			while (true)
			{
				if (m_qThreads.size() > 0)
				{
					while (!m_qThreads.front()->joinable())
						std::this_thread::sleep_for(std::chrono::nanoseconds(1));
					
					try
					{
						m_qThreads.front()->join();
						delete m_qThreads.front();
						m_qThreads.pop();
					}
					catch (...) { }
				}
				else
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
			}
		}

		void AutoManagedServer::mainloop() noexcept
		{
			while (true)
				m_qThreads.push(new std::thread(&AutoManagedServer::handler, this, m_sock.Accept()));
		}

		void AutoManagedServer::handler(SocketConnection *connection) noexcept
		{
			handle(connection);
			delete connection;
		}

		size_t AutoManagedServer::getCurrentConnections() const noexcept
		{
			return m_qThreads.size();
		}
	}
}
