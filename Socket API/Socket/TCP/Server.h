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
			IOContext *ioContext = nullptr;
			SOCKET     m_socket  = INVALID_SOCKET;
			int        m_nResult = 0;

		public:
			Server() noexcept = default;
			~Server() noexcept;

		public:
			bool Bind(const std::size_t &port) noexcept;
			SocketConnection *Accept()         noexcept;
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
			AutoManagedServer()                               noexcept;
			virtual ~AutoManagedServer()                      noexcept;

		private:
			void cleanup()                                    noexcept;
			void mainloop()                                   noexcept;
			void handler(SocketConnection *connection)        noexcept;
			virtual void handle(SocketConnection *connection) noexcept = 0; // need to be overridden

		public:
			void start(const std::size_t &port)               noexcept;
			size_t getCurrentConnections()              const noexcept;
		};
	}
}