#pragma once
#include "../defines.h"
#include "../IOContext.h"
#include "../IPAddress.h"
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
			SocketConnection        m_client;
			std::queue<std::string> m_qMsgs;
			std::thread             m_tReceiver;
			std::mutex              m_lock;

		public:
			AsyncClient() noexcept = default;

			virtual ~AsyncClient() noexcept
			{
				if (m_tReceiver.joinable())
					m_tReceiver.detach();
			}

		private:
			void receiver_thread() noexcept
			{
				while (true)
				{
					std::string recv = m_client.Recv();

					m_lock.lock();
					m_qMsgs.push(recv);
					m_lock.unlock();

					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
			}

		public:
			bool Send(const std::string &msg) noexcept
			{
				return m_client.Send(msg);
			}
			
			bool Connect(const IPAddress &ip, const std::size_t &port) noexcept
			{
				m_client = SocketConnection(*m_client.getIOContext(), ip, port);

				if (m_client.startupSucceeded())
				{
					m_tReceiver = std::thread(&AsyncClient::receiver_thread, this);

					return true;
				}

				return false;
			}
			
			std::string Recv() noexcept
			{
				m_lock.lock();
				std::string sFront = m_qMsgs.front();
				m_qMsgs.pop();
				m_lock.unlock();

				return sFront;
			}

		public:
			void Clear() noexcept
			{
				m_lock.lock();
				while (!m_qMsgs.empty())
					m_qMsgs.pop();
				m_lock.unlock();
			}
			
			void Close() noexcept
			{
				m_client.Close();
				Clear();
			}
			
			bool isAvailable() noexcept
			{
				m_lock.lock();
				bool bVal = m_qMsgs.size() > 0;
				m_lock.unlock();

				return bVal;
			}
		};
	}
}
