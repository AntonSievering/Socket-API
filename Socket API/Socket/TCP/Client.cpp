#include "Client.h"

namespace Socket
{
	namespace TCP
	{
		Client::Client() noexcept
		{
			// just start the Windows Sockets
			m_nResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		}

		Client::~Client() noexcept
		{
			// cleanup
			Close();
		}

		bool Client::Connect(const std::string &ip, const std::size_t &port) noexcept
		{
			addrinfo *result, *ptr, hints;

			// IPv4 TCP connection
			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			memset(&m_recvbuf, 0, DEFAULT_BUFLEN);

			// convert string ip into 
			m_nResult = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &result);

			// try to connect to one of the possible addresses
			for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
			{
				m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				m_nResult = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);

				// did connection succeed?
				bConnected = m_nResult >= 0;

				// if connection succeeded, exit
				if (bConnected)
					break;
			}

			// cleanup
			freeaddrinfo(result);

			return bConnected;
		}

		bool Client::Send(const std::string &msg) noexcept
		{
			bConnected = send(m_socket, msg.c_str(), msg.size(), 0) > 0 && m_socket > 0;

			return bConnected;
		}

		std::string Client::Recv() noexcept
		{
			// receive
			ZeroMemory(m_recvbuf, m_nRecvbuflen);
			int length = recv(m_socket, m_recvbuf, m_nRecvbuflen, 0);
			bConnected = length > 0;

			// copy to string
			std::string str = std::string();
			if (bConnected)
			{
				str.resize((length > 0) ? length : 0);
				str.assign(m_recvbuf);
			}

			return str;
		}
		
		void Client::Close() noexcept
		{
			// cleanup
			closesocket(m_socket);
		}

		bool Client::isConnected() const noexcept
		{
			return bConnected;
		}

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
			if (m_client.Connect(ip, port))
			{
				m_tReceiver = std::thread(&AsyncClient::receiver_thread, this);

				return true;
			}

			return false;
		}

		bool AsyncClient::isConnected() const noexcept
		{
			return m_client.isConnected();
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