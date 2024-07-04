#include "LibConfig.h"
#include "ServerGlobals.h" // 가장 먼저 포함 시켜야 전역 객체 초기화가 순서 보장됨
#include <memory>
#include "Core/CoreTLS.h"
#include "Core/CoreInitializer.h"
#include "Thread/ThreadManager.h"
#include "Network/ServerService.h"
#include "Network/IocpService.h"
#include "Memory/MemoryManager.h"
#include "GameSession.h"
#include "ClientPacketHandler.h"

using namespace ServerCore;
using namespace GameServer;

enum
{
	WORKER_TICK = 64
};

//GameSessionManager GSessionManager;
//shared_ptr<Room> GRoom = make_shared<Room>();
void DoWorkerJob(std::shared_ptr<ServerService>& service)
{
	while (true)
	{
		tls_EndTickCount = ::GetTickCount64() + WORKER_TICK;

		// 네트워크 입출력 처리 -> 인게임 로직까지 (패킷 핸들러에 의해)
		service->GetIocpCore()->Dispatch(10); // GQCS에서 최대 10ms까지 기다림

		// 예약된 일감 처리
		ThreadManager::CheckJobTimer();

		// GlobalQueue
		ThreadManager::DoGlobalQueueWork();
	}
}

int main()
{
	/*// DB Test Code
	ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={SQL Server Native Client 11.0};Server=(localdb)\\MSSQLLocalDB;Database=ServerDb;Trusted_Connection=Yes;"));

	DbConnection* dbConn = GDBConnectionPool->Pop();
	DBSynchronizer dbSync(*dbConn);
	dbSync.Synchronize(L"GameDB.xml");
	
	{
		WCHAR name[] = L"Test";

		SP::InsertGold insertGold(*dbConn);
		insertGold.In_Gold(100);
		insertGold.In_Name(name);
		insertGold.In_CreateDate(TIMESTAMP_STRUCT{ 2020, 6, 8 });
		insertGold.Execute();
	}

	{
		SP::GetGold getGold(*dbConn);
		getGold.In_Gold(100);

		int32 id = 0;
		int32 gold = 0;
		WCHAR name[100];
		TIMESTAMP_STRUCT date;

		getGold.Out_Id(OUT id);
		getGold.Out_Gold(OUT gold);
		getGold.Out_Name(OUT name);
		getGold.Out_CreateDate(OUT date);

		getGold.Execute();

		while (getGold.Fetch())
		{
			GConsoleLogger->WriteStdOut(Color::BLUE,
				L"ID[%d] Gold[%d] Name[%s]\n", id, gold, name);
		}
	}
	*/
	ClientPacketHandler::Init();

	std::shared_ptr<ServerService>service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpService>(),
		MakeShared<GameSession>, // TODO : SessionManager 등
		100);

	ASSERT_CRASH(service->Start())

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{
				DoWorkerJob(service);
			});
	}

	
	// Main Thread
	DoWorkerJob(service);

	GThreadManager->JoinAll();
}