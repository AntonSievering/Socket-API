#pragma once
#include "../defines.h"
#include "SocketConnection.h"

namespace Socket
{
	namespace TCP
	{
		// Server - manages the SOCKETS
		class Server
		{
		private:
			IOContext *m_ioContext = nullptr;
			SOCKET     m_socket    = INVALID_SOCKET;
			int        m_nResult   = 0;

		public:
			Server() noexcept = default;

			Server(IOContext &context) noexcept
			{
				m_ioContext = &context;
			}
			
			virtual ~Server() noexcept
			{
				closesocket(m_socket);
			}

		public:
			bool Bind(const std::size_t &port) noexcept
			{
				addrinfo hints{}, *result;
				hints.ai_family = AF_INET;
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_protocol = IPPROTO_TCP;
				hints.ai_flags = AI_PASSIVE;

				getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
				m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
				bind(m_socket, result->ai_addr, (int)result->ai_addrlen);
				freeaddrinfo(result);
				listen(m_socket, SOMAXCONN);

				return m_socket != SOCKET_ERROR;
			}
			
			SocketConnection *Accept() noexcept
			{
				sockaddr_in addr{};
				int addrlen = sizeof(sockaddr_in);
				SOCKET sock = accept(m_socket, (sockaddr *)&addr, &addrlen);

				return new SocketConnection(*m_ioContext, sock, addr);
			}
		};

		// AutoManagedServer - accepts connections and calls the specified function in a new thread
		// you need to override it
		class AutoManagedServer
		{
		protected:
			std::queue<std::thread *> m_qThreads;
			std::thread m_cleanup;
			std::thread m_accepter;
			Server m_sock;

		public:
			AutoManagedServer() noexcept = default;
			
			AutoManagedServer(IOContext &ioContext) noexcept
			{
				m_sock = Server(ioContext);
			}

			virtual ~AutoManagedServer() noexcept
			{
				if (m_accepter.joinable())
					m_accepter.detach();

				if (m_cleanup.joinable())
					m_cleanup.detach();
			}

		private:
			void cleanup() noexcept
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

			void mainloop() noexcept
			{
				while (true)
					m_qThreads.push(new std::thread(&AutoManagedServer::handler, this, m_sock.Accept()));
			}
			
			void handler(SocketConnection *connection) noexcept
			{
				handle(connection);
				delete connection;
			}
			
			virtual void handle(SocketConnection *connection) noexcept = 0; // need to be overridden

		public:
			void start(const std::size_t &port) noexcept
			{
				m_sock = Socket::TCP::Server();
				m_sock.Bind(port);

				m_cleanup = std::thread(&AutoManagedServer::cleanup, this);
				m_accepter = std::thread(&AutoManagedServer::mainloop, this);
			}

			size_t getCurrentConnections() const noexcept
			{
				return m_qThreads.size();
			}

			void join() noexcept
			{
				while (!m_accepter.joinable()) std::this_thread::sleep_for(std::chrono::microseconds(1));

				m_accepter.join();
				m_cleanup.join();
			}
		};
	}
}