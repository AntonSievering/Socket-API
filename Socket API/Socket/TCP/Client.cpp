#include "Client.h"

namespace Socket
{
	namespace TCP
	{
		// AsnycClient Implementation
		AsyncClient::AsyncClient() noexcept
		{
			m_qMsgs = std::queue<std::string>();
		}

		Socket::TCP::AsyncClient::~AsyncClient() noexcept
		{
			if (m_tReceiver.joinable())
				m_tReceiver.detach();
		}

		void AsyncClient::receiver_thread() noexcept
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

		bool AsyncClient::Send(const std::string &msg) noexcept
		{
			return m_client.Send(msg);
		}

		bool AsyncClient::Connect(const std::string &ip, const std::size_t &port) noexcept
		{
			m_client = SocketConnection(ip, port);
			
			if (m_client.startupSucceeded())
			{
				m_tReceiver = std::thread(&AsyncClient::receiver_thread, this);

				return true;
			}

			return false;
		}

		bool AsyncClient::isConnected() const noexcept
		{
			return m_client.startupSucceeded();
		}

		bool AsyncClient::isAvailable() noexcept
		{
			m_lock.lock();
			bool bVal = m_qMsgs.size() > 0;
			m_lock.unlock();

			return bVal;
		}

		std::string AsyncClient::Recv() noexcept
		{
			m_lock.lock();
			std::string sFront = m_qMsgs.front();
			m_qMsgs.pop();
			m_lock.unlock();

			return sFront;
		}

		void AsyncClient::Clear() noexcept
		{
			m_lock.lock();
			while (!m_qMsgs.empty())
				m_qMsgs.pop();
			m_lock.unlock();
		}

		void AsyncClient::Close() noexcept
		{
			m_client.Close();
			Clear();
		}
	}
}