#include <iostream>
#include <BasicLog.h>

#include "TcpNetwork.h"
//#include "LobbyManager.h"
//#include "UserManager.h

using LOG_TYPE = NServerNetLib::LOG_TYPE;
using NET_ERROR_CODE = NServerNetLib::NET_ERROR_CODE;

int main() {
    std::cout << "Start...." << std::endl;

    NServerNetLib::ServerConfig config;
    NServerNetLib::BasicLog logger;

    NServerNetLib::TcpNetwork netLib;
    auto result = netLib.Init(&config, &logger);

    if (result != NET_ERROR_CODE::NONE)
    {
        logger.Write(LOG_TYPE::L_ERROR, "%s | Init Fail. NetErrorCode(%d)", __FUNCTION__, (short)result);
        return 0;
    }

    /*
    NServerNetLib::UserManager UserMgr;
    UserMgr.Init(m_pServerConfig->MaxClientCount);

    m_pLobbyMgr = std::make_unique<LobbyManager>();
    m_pLobbyMgr->Init({ m_pServerConfig->MaxLobbyCount,
                        m_pServerConfig->MaxLobbyUserCount,
                        m_pServerConfig->MaxRoomCountByLobby,
                        m_pServerConfig->MaxRoomUserCount },
                      m_pNetwork.get(), m_pLogger.get());

    m_pPacketProc = std::make_unique<PacketProcess>();
    m_pPacketProc->Init(m_pNetwork.get(), m_pUserMgr.get(), m_pLobbyMgr.get(), m_pServerConfig.get(), m_pLogger.get());

    m_pLogger->Write(LOG_TYPE::L_INFO, "%s | Init Success. Server Run", __FUNCTION__);
    */
    while (true)
    {
        netLib.Run();

        while (true)
        {
            auto packetInfo = netLib.GetPacketInfo();

            if (packetInfo.PacketId == 0)
            {
                //printf("dsdsd5\n");
                break;
            }
            else
            {
                printf("recv Packet: sessionIndex(%d), packetId(%d), bodySize(%d)\n",
                       packetInfo.SessionIndex, packetInfo.PacketId, packetInfo.PacketBodySize);
                //m_pPacketProc->Process(packetInfo);
            }
        }

        //m_pPacketProc->StateCheck();
    }

    //std::cout << "press any key to exit...";
    //getchar();
    return 0;
}