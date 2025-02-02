#pragma once

#include "defines.h"


namespace net
{
	class IPAddress
	{
	private:
		union
		{
			in_addr m_addr;
			uint8_t m_rawData[4];
		};
		
	public:
		constexpr IPAddress() noexcept
			: IPAddress(127, 0, 0, 1) {}

		constexpr IPAddress(const uint8_t b1, const uint8_t b2, const uint8_t b3, const uint8_t b4) noexcept
		{
			m_addr.S_un.S_un_b.s_b1 = b1;
			m_addr.S_un.S_un_b.s_b2 = b2;
			m_addr.S_un.S_un_b.s_b3 = b3;
			m_addr.S_un.S_un_b.s_b4 = b4;
		}

		constexpr IPAddress(const uint8_t addr[4]) noexcept
			: IPAddress(addr[0], addr[1], addr[2], addr[3]) {}

		constexpr IPAddress(const sockaddr_in &sockaddr) noexcept
		{
			m_addr = sockaddr.sin_addr;
		}

		IPAddress(const std::string &sIP) noexcept
		{
			inet_pton(AF_INET, sIP.c_str(), &m_addr);
		}

	public:
		std::string asString() const noexcept
		{
			return std::to_string(m_rawData[0]) + "." + std::to_string(m_rawData[1]) + "." + std::to_string(m_rawData[2]) + "." + std::to_string(m_rawData[3]);
		}

		constexpr operator in_addr() const noexcept
		{
			return m_addr;
		}

		uint8_t operator[](const uint8_t i) const noexcept
		{
			return m_rawData[i];
		}

		uint8_t &operator[](const uint8_t i) noexcept
		{
			return m_rawData[i];
		}

		static IPAddress getIPByHostname(const std::string &sHostname) noexcept
		{
			addrinfo hints{}, *result;
			hints.ai_family   = AF_UNSPEC;
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_flags    = AI_PASSIVE;

			if (getaddrinfo(sHostname.c_str(), nullptr, &hints, &result) != 0)
				return IPAddress(255, 255, 255, 255); // Error

			if (result)
			{
				IPAddress ip = IPAddress(*(sockaddr_in *)result->ai_addr);
				freeaddrinfo(result);
				return ip;
			}

			return IPAddress(255, 255, 255, 255); // no IP found
		}

	public:
#ifdef _IOSFWD_
		friend std::ostream &operator << (std::ostream &stream, const IPAddress &addr) noexcept
		{
			return stream << addr.asString();
		}
#endif
	};
}
