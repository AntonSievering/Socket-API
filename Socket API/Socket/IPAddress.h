#pragma once
#include "defines.h"

namespace Socket
{
	struct IPAddress
	{
		union
		{
			uint8_t addr[4];
			uint32_t chunk;
		};

	public:
		IPAddress() noexcept
		{
			// default IP address = 127.0.0.1
			addr[0] = 127;
			addr[1] = 0;
			addr[2] = 0;
			addr[3] = 1;
		}

		IPAddress(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) noexcept
		{
			addr[0] = d0;
			addr[1] = d1;
			addr[2] = d2;
			addr[3] = d3;
		}

		IPAddress(uint8_t addr[4]) noexcept
		{
			this->addr[0] = addr[0];
			this->addr[1] = addr[1];
			this->addr[2] = addr[2];
			this->addr[3] = addr[3];
		}
	};
}
