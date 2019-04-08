#pragma once


namespace NServerNetLib {
    struct ServerConfig {
        unsigned short Port = 32451;
        int BackLogCount = 64;

        int MaxClientCount = 900;
        int ExtraClientCount = 99;

        short MaxClientSockOptRecvBufferSize = 8012;
        short MaxClientSockOptSendBufferSize = 8012;
        short MaxClientRecvBufferSize = 4096;
        short MaxClientSendBufferSize = 4096;

        bool IsLoginCheck = false;

        int MaxLobbyCount = 2;
        int MaxLobbyUserCount = 64;
        int MaxRoomCountByLobby = 32;
        int MaxRoomUserCount = 4;
    };

    const int MAX_IP_LEN = 32;
    const int MAX_PACKET_BODY_SIZE = 1024;

    struct ClientSession {
        bool IsConnected() { return SocketFD != 0 ? true : false; }

        void Clear() {
            Seq = 0;
            SocketFD = 0;
            IP[0] = '\0';
            RemainingDataSize = 0;
            PrevReadPosInRecvBuffer = 0;
            SendSize = 0;
        }

        int Index = 0;
        long long Seq = 0;
        unsigned long long SocketFD = 0;
        char IP[MAX_IP_LEN] = {0,};

        char *pRecvBuffer = nullptr;
        int RemainingDataSize = 0;
        int PrevReadPosInRecvBuffer = 0;

        char *pSendBuffer = nullptr;
        int SendSize = 0;
    };

    struct RecvPacketInfo {
        int SessionIndex = 0;
        short PacketId = 0;
        short PacketBodySize = 0;
        char *pRefData = 0;
    };
    struct RecvPacketInfo2 {
        int SessionIndex = 0;
        short PacketId = 0;
        short PacketBodySize = 0;
        char *pRefData = 0;
    };


    enum class SOCKET_CLOSE_CASE : short {
        SESSION_POOL_EMPTY = 1,
        SELECT_ERROR = 2,
        SOCKET_RECV_ERROR = 3,
        SOCKET_RECV_BUFFER_PROCESS_ERROR = 4,
        SOCKET_SEND_ERROR = 5,
        FORCING_CLOSE = 6,
    };


    enum class PACKET_ID : short {
        NTF_SYS_CONNECT_SESSION = 2,
        NTF_SYS_CLOSE_SESSION = 3,

    };

#pragma pack(push, 1)
    struct PacketHeader {
        short Id;
        short BodySize;
    };

    const int PACKET_HEADER_SIZE = sizeof(PacketHeader);


    struct PktNtfSysCloseSession : PacketHeader {
        int SockFD;
    };
#pragma pack(pop)
}