#include "Socket/TCP/Client.h"

int main()
{
	Socket::IOContext ioContext{};
	Socket::TCP::SocketConnection socket = Socket::TCP::SocketConnection(ioContext, Socket::IPAddress("192.168.178.52"), 5000);
	
	std::cout << "target machine ip address: " << socket.getIPAddress().asString() << std::endl;
	
	size_t nReceivedBytes = 0;
	double fTime = 0.0;
	constexpr double fBytesToMiB = 1.0 / (1024.0 * 1024.0);
	constexpr double fUpdateTime = 1.0;
	double fTimeToUpdate = fUpdateTime;

	std::chrono::time_point<std::chrono::system_clock> tpStart = std::chrono::system_clock::now(), tpEnd;

	while (socket.isConnected())
	{
		tpEnd = std::chrono::system_clock::now();
		double duration = std::chrono::duration_cast<std::chrono::nanoseconds>(tpEnd - tpStart).count() / 1e9;
		tpStart = tpEnd;
		fTime += duration;
		fTimeToUpdate -= duration;

		std::string sData = socket.RecvBlocking();
		nReceivedBytes += sData.size();
		socket.Send("a");

		if (fTimeToUpdate <= 0.0f)
		{
			fTimeToUpdate = fUpdateTime;
			double MiB = (double)nReceivedBytes * fBytesToMiB;
			std::cout << "Total: " << MiB << " MiB Speed: " << MiB / fTime << " MiB/s    \r";
		}
	}
}
