#pragma once

#include <Winsock2.h>
#include "IOContext_Base.h"

#define IO_CONTEXT_WIN32

#ifdef IO_CONTEXT_LINUX
#error Either Win32 or Linux io contexts can be included.
#endif

namespace Socket
{
	class IOContext final : public IOContextBase
	{
	private:
		WSAData m_wsaData;
		bool bSucceded = false;

	public:
		IOContext() noexcept
		{
			bSucceded = WSAStartup(MAKEWORD(2, 2), &m_wsaData) == 0;
		}

		~IOContext() noexcept
		{
			WSACleanup();
		}

	public:
		bool startupSucceeded() const noexcept override
		{
			return bSucceded;
		}
	};
}
