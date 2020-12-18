#include "Server.h"

namespace Socket
{
	namespace TCP
	{
		// Server Connection implementation

		ServerConnection::ServerConnection()
		{
			ClientSocket = SOCKET();
			ZeroMemory(buf, DEFAULT_BUFLEN);
		}

		ServerConnection::ServerConnection(const SOCKET &sock)
		{
			ClientSocket = sock;
			ZeroMemory(buf, DEFAULT_BUFLEN);
		}

		ServerConnection::~ServerConnection()
		{
			closesocket(ClientSocket);
		}

		void ServerConnection::Send(const char *buf)
		{
			send(ClientSocket, buf, sizeof(buf), 0);
		}

		std::string ServerConnection::Recv()
		{
			std::string msg;

			ZeroMemory(buf, DEFAULT_BUFLEN);
			int length = recv(ClientSocket, buf, DEFAULT_BUFLEN, 0);

			if (length > 0)
			{
				msg.resize(length);
				msg.assign(buf);
			}

			return msg;
		}

		SOCKET ServerConnection::getSocket()
		{
			return ClientSocket;
		}


		// Server implementation

		Server::Server()
		{
			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE;

			m_nResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		}

		Server::~Server()
		{
			closesocket(m_socket);
			WSACleanup();
		}

		bool Server::Bind(const char *port)
		{
			getaddrinfo(NULL, port, &hints, &result);
			m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			bind(m_socket, result->ai_addr, (int)result->ai_addrlen);
			freeaddrinfo(result);
			listen(m_socket, SOMAXCONN);

			return m_socket != SOCKET_ERROR;
		}

		ServerConnection *Server::Accept()
		{
			SOCKET clientSocket = accept(m_socket, NULL, NULL);
			return new ServerConnection(clientSocket);
		}


		// Auto Managed Server implementation

		AutoManagedServer::AutoManagedServer(const char *port, std::function<void(ServerConnection *)> function)
		{
			m_qThreads = std::queue<std::thread *>();

			m_sock = Socket::TCP::Server();
			m_sock.Bind(port);

			m_cleanup = std::thread(&AutoManagedServer::cleanup, this);
			m_accepter = std::thread(&AutoManagedServer::mainloop, this, function);
		}

		AutoManagedServer::~AutoManagedServer()
		{
			if (m_cleanup.joinable())
				m_cleanup.detach();

			if (m_accepter.joinable())
				m_accepter.detach();
		}

		void AutoManagedServer::cleanup()
		{
			while (true)
			{
				if (m_qThreads.size() > 0)
				{
					if (m_qThreads.front()->joinable())
					{
						try
						{
							m_qThreads.front()->join();
							delete m_qThreads.front();
						}
						catch (...) { }
					}

					m_qThreads.pop();
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}

		void AutoManagedServer::mainloop(std::function<void(ServerConnection *)> function)
		{
			while (true)
			{
				m_qThreads.push(new std::thread(function, m_sock.Accept()));
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}

		size_t AutoManagedServer::getCurrentConnections()
		{
			return m_qThreads.size();
		}
	}
}