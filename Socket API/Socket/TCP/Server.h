#pragma once
#include "../defines.h"

namespace Socket
{
	namespace TCP
	{
		// ServerConnection - manages a server-side connection
		class ServerConnection
		{
		private:
			SOCKET ClientSocket;
			char buf[DEFAULT_BUFLEN];

		public:
			ServerConnection()                   noexcept;
			ServerConnection(const SOCKET &sock) noexcept;
			~ServerConnection()                  noexcept;

		public:
			void Send(const std::string &msg)    noexcept;
			std::string Recv()                   noexcept;

			SOCKET getSocket()             const noexcept;
		};

		// Server - manages the SOCKETS
		class Server
		{
		private:
			WSAData m_wsaData;
			SOCKET m_socket = INVALID_SOCKET;
			int m_nResult;

			addrinfo *result, hints;

		public:
			Server()                           noexcept;
			~Server()                          noexcept;

		public:
			bool Bind(const std::size_t &port) noexcept;
			ServerConnection *Accept()         noexcept;
		};

		// AutoManagedServer - accepts connections and calls the specified function in a new thread
		// overriding it is a good idea
		class AutoManagedServer
		{
		private:
			std::queue<std::thread *> m_qThreads;
			std::thread m_cleanup;
			std::thread m_accepter;
			Server m_sock;

		public:
			AutoManagedServer(const std::size_t &port, std::function<void(ServerConnection *)> function) noexcept;
			~AutoManagedServer()                                            noexcept;

		private:
			void cleanup()                                                  noexcept;
			void mainloop(std::function<void(ServerConnection *)> function) noexcept;

		public:
			size_t getCurrentConnections()                            const noexcept;
		};
	}
}