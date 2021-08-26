#pragma once

#include "defines.h"
#include "IOContext_Base.h"

#define IO_CONTEXT_LINUX

#ifdef IO_CONTEXT_WIN32
	#error Either Win32 or Linux io contexts can be included.
#endif

namespace Socket
{
	// not used yet
	class IOContext final : public IOContextBase
	{
	public:
		IOContext() noexcept = default;

	public:
		bool startupSucceeded() const noexcept override
		{
			return true;
		}
	};
}
