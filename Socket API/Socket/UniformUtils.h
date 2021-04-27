#pragma once

#include "defines.h"


namespace Socket
{
	namespace Uniform
	{
		// ioctlsocket (Windows) == ioctl (Linux)
		static int __stdcall ioctl_socket(SOCKET s, long cmd, u_long *argp) noexcept
		{
#ifdef _WIN32
			return ioctlsocket(s, cmd, argp);
#elif __linux__
			return ioctl(s, cmd, argp);
#endif
		}

		// closesocket (Windows) == shutdown (Linux)
		static int __stdcall socketsocket(SOCKET s) noexcept
		{
#ifdef _WIN32
			return closesocket(s);
#elif __linux__
			return shutdown(s, 2);
#endif
		}
	}
}
