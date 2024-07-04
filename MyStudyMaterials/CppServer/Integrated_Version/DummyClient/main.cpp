#include "LibConfig.h"
#include "ClientGlobals.h" // 가장 먼저 포함 시켜야 전역 객체 초기화가 순서 보장됨
#include "Thread/ThreadManager.h"
#include "Network/Service.h"
#include "Network/Session.h"
#include "Util/BufferReader.h"
#include "Memory/MemoryManager.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"
#include "Network/IocpService.h"
char sendData[] = "Hello World";

using namespace DummyClient;
using namespace std::chrono_literals;

int main()
{
    ServerPacketHandler::Init();
    std::this_thread::sleep_for(1s);

    std::shared_ptr<ClientService> clientService = MakeShared<ClientService>(
        NetAddress(L"127.0.0.1", 7777),
        MakeShared<IocpService>(),
        MakeShared<ServerSession>, // TODO : SessionManager 등
        500);

    ASSERT_CRASH(clientService->Start())

    for (int32 i = 0; i < 2; i++)
    {
        GThreadManager->Launch([=]()
        {
            while (true)
            {
                clientService->GetIocpCore()->Dispatch();
            }
        });
    }

    // Test Code
    Protocol::C_CHAT chatPkt;
    // std::string msg = new std::string(u8"Hello world!");
    chatPkt.mutable_msg()->assign(u8"Hi~! (chatting from dummy client)");
    auto sendBuffer = ServerPacketHandler::MakeSendBuffer(chatPkt);

    while (true)
    {
        clientService->Broadcast(sendBuffer);
        std::this_thread::sleep_for(1s);
    }

    GThreadManager->JoinAll();
}
