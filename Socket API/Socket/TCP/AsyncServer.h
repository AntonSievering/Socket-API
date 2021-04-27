#pragma once

#include "../ThreadSafe.h"
#include "SocketConnection.h"
#include "Server.h"


namespace Socket
{
	namespace TCP
	{
		class AsyncServer
		{
			struct ThreadInfo
			{
				std::thread                          thread{};
				ThreadSafe::Vector<SocketConnection> vActiveConnections{};
			};

		private:
			Server                  m_acceptingServer{};
			std::vector<ThreadInfo> m_vThreadInfoPool{};

		public:
			AsyncServer() noexcept = default;

			AsyncServer(IOContext &ioContext) noexcept
			{
				m_acceptingServer = Server(ioContext);
			}

		public:
			size_t findLowestLoad() const noexcept
			{
				size_t nLowestIndex = 0;
				size_t nLowestValue = 1'000'000'000;

				for (size_t i = 0; i < m_vThreadInfoPool.size(); i++)
				{
					size_t nConnections = m_vThreadInfoPool.at(i).vActiveConnections.size();

					if (nConnections < nLowestValue)
					{
						nLowestIndex = i;
						nLowestValue = nConnections;
					}
				}

				return nLowestIndex;
			}

			void start(const uint16_t nPort, const uint16_t nThreads) noexcept
			{
				m_acceptingServer.Bind(nPort);

				for (uint16_t i = 0; i < nThreads; i++)
					m_vThreadInfoPool.push_back({ std::thread(&AsyncServer::threadHandle, this, i) });

				while (true)
				{
					SocketConnection conn = m_acceptingServer.Accept();
					m_vThreadInfoPool.at(findLowestLoad()).vActiveConnections.push_back(conn);
				}
			}

			void threadHandle(const uint16_t nId) noexcept
			{
				ThreadInfo *pOwnThreadInfo = &m_vThreadInfoPool.at(nId);

				while (true)
				{
					size_t i = 0;
					size_t nLoad = 0;
					while (i < pOwnThreadInfo->vActiveConnections.size())
					{
						SocketConnection *pConnection = &pOwnThreadInfo->vActiveConnections.at(i);

						if (!pConnection->isConnected())
						{
							pOwnThreadInfo->vActiveConnections.remove(i);
							i--;
						}

						std::string sQuery = pConnection->Recv();
						if (sQuery.size() > 0)
						{
							std::string sAnswer = handleQuery(sQuery);
							pConnection->Send(sAnswer);
							nLoad++;
						}

						i++;
					}

					if (nLoad == 0)
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
			}

			virtual std::string handleQuery(const std::string &sQuery) noexcept = 0;
		};
	}
}
