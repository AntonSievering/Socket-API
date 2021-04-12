#pragma once
#include "../defines.h"
#include "../IOContext.h"
#include "ClientInfo.h"

namespace Socket
{
	namespace UDP
	{
		class Client
		{
		private:
			IOContext *m_pIOContext;
			SOCKET     m_socket;

		public:
			Client() noexcept = default;
			
			Client(IOContext &ioContext) noexcept
			{
				m_pIOContext = &ioContext;
				m_socket = socket(AF_INET, SOCK_DGRAM, 0);
			}

			~Client() noexcept
			{
				closesocket(m_socket);
			}

		public:
			bool Send(const std::string &sContent, const ClientInfo &info) noexcept
			{
				sockaddr_in addr = info.getSockaddr();
				return sendto(m_socket, sContent.c_str(), sContent.size(), 0, (sockaddr *)&addr, sizeof(addr)) > 0;
			}
		};
	}
}
