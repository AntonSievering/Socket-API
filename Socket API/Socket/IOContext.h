#pragma once

#ifdef _WIN32
	#include "IOContext_Win32.h"

#elif __linux__
	#include "IOCOntext_Linux.h"

#endif // _WIN32
