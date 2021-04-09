#pragma once
#include "../defines.h"

namespace Socket
{
	namespace TCP
	{
		// Client - manages the client-side SOCKETS
		class Client
		{
		private:
			WSAData m_wsaData;
			SOCKET m_socket = INVALID_SOCKET;
			int m_nResult = -1;
			char m_recvbuf[DEFAULT_BUFLEN];
			int m_nRecvbuflen = DEFAULT_BUFLEN;
			bool bConnected = false;

		public:
			Client()                                                     noexcept;
			~Client()                                                    noexcept;

		public:
			bool Connect(const std::string &ip, const std::size_t &port) noexcept;
			bool Send(const std::string &msg)                            noexcept;
			std::string Recv()                                           noexcept;

		public:
			void Close()                                                 noexcept;
			bool isConnected()                                     const noexcept;
		};

		// AsyncClient - asyncronous Client
		// receives everything and puts it into a queue
		class AsyncClient
		{
		private:
			Client m_client;
			std::queue<std::string> m_qMsgs;
			std::thread m_tReceiver;
			std::mutex m_lock;

		public:
			AsyncClient()                                                noexcept;
			~AsyncClient()                                               noexcept;

		private:
			void receiver_thread()                                       noexcept;

		public:
			bool Send(const std::string &msg)                            noexcept;
			bool Connect(const std::string &ip, const std::size_t &port) noexcept;
			std::string Recv()                                           noexcept;

		public:
			void Clear()                                                 noexcept;
			void Close()                                                 noexcept;
			bool isAvailable()                                           noexcept;
			bool isConnected()                                     const noexcept;
		};
	}
}