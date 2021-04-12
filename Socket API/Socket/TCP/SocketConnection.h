#include "../defines.h"
#include "../IPAddress.h"
#include "../IOContext.h"

namespace Socket
{
	namespace TCP
	{
		class SocketConnection
		{
		private:
			IOContext *m_pIOContext        = nullptr;
			SOCKET     m_socket            = INVALID_SOCKET;
			bool       m_bStartupSucceeded = false;

		public:
			IPAddress  m_ipAddr{};

		public:
			SocketConnection() noexcept = default;
			SocketConnection(IOContext &ioContext, const SOCKET &sock, const sockaddr_in &addr) noexcept;
			SocketConnection(IOContext &ioContext, const std::string &ip, const std::size_t &port) noexcept;
			~SocketConnection() noexcept;

		public:
			bool Send(const std::string &msg) noexcept;

			uint64_t getReceivedBytes() const noexcept;
			bool isAvailable() const noexcept;
			bool startupSucceeded() const noexcept;
			std::string Recv() noexcept;
			void Close() noexcept;

			SOCKET getSocket() const noexcept;
			IPAddress getIPAddress() const noexcept;
			IOContext *getIOContext() const noexcept;
		};
	}
}
