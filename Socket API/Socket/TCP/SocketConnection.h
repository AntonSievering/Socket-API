#include "../defines.h"

namespace Socket
{
	namespace TCP
	{
		class SocketConnection
		{
		private:
			SOCKET m_socket;
			char buf[DEFAULT_BUFLEN];

		public:
			uint8_t clientAddr[4];

		public:
			SocketConnection() noexcept;
			SocketConnection(const SOCKET &sock, const sockaddr_in &addr) noexcept;
			SocketConnection(const std::string &ip, const std::size_t &port) noexcept;
			~SocketConnection() noexcept;

		public:
			void Send(const std::string &msg) noexcept;

			uint64_t getReceivedBytes() const noexcept;
			bool isAvailable() const noexcept;
			std::string RecvNonBlocking() noexcept;
			std::string Recv(const uint64_t &nMaxLength = 1024) noexcept;

			SOCKET getSocket() const noexcept;

		private:
			void initializeWinsock() noexcept;
		};
	}
}
