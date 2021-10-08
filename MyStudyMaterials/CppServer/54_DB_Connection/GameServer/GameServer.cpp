#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"
#include <tchar.h>
#include "Protocol.pb.h"
#include "Job.h"
#include "Room.h"
#include "Player.h"
#include "DBConnectionPool.h"

enum
{
	WORKER_TICK = 64
};

void DoWorkerJob(ServerServiceRef& service)
{
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		// 네트워크 입출력 처리 -> 인게임 로직까지 (패킷 핸들러에 의해)
		service->GetIocpCore()->Dispatch(10);

		// 예약된 일감 처리
		ThreadManager::DistributeReservedJobs();

		// 글로벌 큐
		ThreadManager::DoGlobalQueueWork();
	}
}

int main()
{
	// DB Connection Pool에 Connection 생성하기
	// connectionString은 Google에 Connection String 검색
	ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={SQL Server Native Client 11.0};Server=(localdb)\\MSSQLLocalDB;Database=ServerDb;Trusted_Connection=Yes;"));

	// Create Table
	{
		auto query = L"										\
			DROP TABLE IF EXISTS [dbo].[Gold];			\
			CREATE TABLE [dbo].[Gold]						\
			(														\
				[id] INT NOT NULL PRIMARY KEY IDENTITY,\
				[gold] INT NULL								\
			);";
		// Db Connection Pool에서 Connection을 하나 꺼내오기
		DBConnection* dbConn = GDBConnectionPool->Pop();
		// Query 실행하기
		ASSERT_CRASH(dbConn->Execute(query));
		// Db Connection Pool에 Connection 반납
		GDBConnectionPool->Push(dbConn);
	}

	// Add Data
	for (int32 i = 0; i < 3; i++)
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();
		
		// 기존에 바인딩 된 데이터 날림
		dbConn->Unbind();

		// Query에 연동하여 넘길 인자 바인딩
		int32 gold = 100;
		SQLLEN len = 0;

		// 넘길 인자 바인딩
		ASSERT_CRASH(dbConn->BindParam(1, SQL_C_LONG, SQL_INTEGER, sizeof(gold), &gold, &len));

		// SQL 실행
		// 물음표(?)자리에는 BindParam함수를 통해 바이딩된 인자들이 순서대로 들어간다
		ASSERT_CRASH(dbConn->Execute(L"INSERT INTO [dbo].[Gold]([gold]) VALUES(?)"));

		GDBConnectionPool->Push(dbConn);
	}

	// Read
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();
		// 기존에 바인딩 된 데이터 날림
		dbConn->Unbind();

		int32 gold = 100;
		SQLLEN len = 0;

		// Query에 연동하여 넘길 인자 바인딩
		ASSERT_CRASH(dbConn->BindParam(1, SQL_C_LONG, SQL_INTEGER, sizeof(gold), &gold, &len));

		// 결과를 받아올 인자 바인딩
		int32 outId = 0;
		SQLLEN outIdLen = 0;
		ASSERT_CRASH(dbConn->BindCol(1, SQL_C_LONG, sizeof(outId), &outId, &outIdLen));

		int32 outGold = 0;
		SQLLEN outGoldLen = 0;
		ASSERT_CRASH(dbConn->BindCol(2, SQL_C_LONG, sizeof(outGold), &outGold, &outGoldLen));

		// SQL Query 실행( gold가 100인 row을 전부 보여달라는 뜻
		ASSERT_CRASH(dbConn->Execute(L"SELECT id, gold FROM [dbo].[Gold] WHERE gold = (?)"));

		// 모든 Row를 한번에 가져올 수 없어서 반복문을 통해 한Row씩 가져와야 함
		while (dbConn->Fetch())
		{
			cout << "Id: " << outId << " Gold : " << outGold << endl;
		}

		GDBConnectionPool->Push(dbConn);
	}

	//

	ClientPacketHandler::Init();

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>, // TODO : SessionManager 등
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{
				DoWorkerJob(service);
			});
	}

	// Main Thread
	DoWorkerJob(service);

	GThreadManager->Join();
}