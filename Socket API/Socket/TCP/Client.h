#pragma once
#include "../defines.h"
#include "../IOContext.h"
#include "SocketConnection.h"

namespace Socket
{
	namespace TCP
	{
		using Client = SocketConnection;

		// AsyncClient - asyncronous Client
		// receives everything and puts it into a queue
		class AsyncClient
		{
		private:
			SocketConnection m_client;
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
