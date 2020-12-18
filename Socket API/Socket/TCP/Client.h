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
			Client();
			~Client();

		public:
			bool Connect(const std::string &ip, const std::size_t &port);
			bool Send(const std::string &msg);
			std::string Recv();

		public:
			void Close();
			bool isConnected();
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
			AsyncClient();
			~AsyncClient();

		private:
			void receiver_thread();

		public:
			bool Send(const std::string &msg);
			bool Connect(const std::string &ip, const std::size_t &port);
			bool isAvailable();
			std::string Recv();

		public:
			void Clear();
			void Close();
			bool isConnected();
		};
	}
}