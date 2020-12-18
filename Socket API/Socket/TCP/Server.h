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
			ServerConnection();
			ServerConnection(const SOCKET &sock);
			~ServerConnection();

		public:
			void Send(const char *buf);
			std::string Recv();

			SOCKET getSocket();
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
			Server();
			~Server();

		public:
			bool Bind(const char *port);
			ServerConnection *Accept();
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
			AutoManagedServer(const char *port, std::function<void(ServerConnection *)> function);
			~AutoManagedServer();

		private:
			void cleanup();
			void mainloop(std::function<void(ServerConnection *)> function);

		public:
			size_t getCurrentConnections();
		};
	}
}