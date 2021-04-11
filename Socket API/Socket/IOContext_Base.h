#pragma once

namespace Socket
{
	class IOContextBase
	{
	public:
		IOContextBase() noexcept = default;

	public:
		virtual bool startupSucceeded() const noexcept = 0;
	};
}
