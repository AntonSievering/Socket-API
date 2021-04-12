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
			Client(IOContext &ioContext) noexcept;
			~Client() noexcept;

		public:
			bool Send(const std::string &sContent, const ClientInfo &info) noexcept;
		};
	}
}
