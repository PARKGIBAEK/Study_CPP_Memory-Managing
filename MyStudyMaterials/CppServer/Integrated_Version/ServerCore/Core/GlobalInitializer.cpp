#include "CoreInitializer.h"
#include "../Thread/DeadLockProfiler.h"
#include "../Thread/ThreadManager.h"
#include "../Memory/MemoryManager.h"
#include "../Network/SendBufferManager.h"
#include "../Job/GlobalQueue.h"
#include "../Job/JobTimer.h"
// #include "../DB/DBConnectionPool.h"
#include "../Log/ConsoleLog.h"
#include "../Network/SocketUtils.h"


namespace ServerCore
{
//MemoryManager* GMemoryManager = new MemoryManager();

DeadLockProfiler* GDeadLockProfiler = nullptr;
ThreadManager* GThreadManager = nullptr;
MemoryManager* GMemoryManager = nullptr;
SendBufferManager* GSendBufferManager = nullptr;
GlobalQueue* GGlobalQueue = nullptr;
JobTimer* GJobTimer = nullptr;
// DBConnectionPool* GDBConnectionPool = nullptr;
ConsoleLog* GConsoleLogger = nullptr;



std::atomic<bool> CoreInitializer::isInitialized = false; 

CoreInitializer::CoreInitializer()
{
	DEBUG_LOG("Initializing Core global variables...");

	GDeadLockProfiler = new DeadLockProfiler();
	DEBUG_LOG("[ GDeadLockProfiler ]  -  Initialized");

	GThreadManager = new ThreadManager();
	DEBUG_LOG("[ GThreadManager ]  -  Initialized");

	GMemoryManager = new MemoryManager();
	DEBUG_LOG("[ GMemoryManager ]  -  Initialized");
	
	GSendBufferManager = new SendBufferManager();
	DEBUG_LOG("[ GSendBufferManager ]  -  Initialized");
	
	GGlobalQueue = new GlobalQueue();
	DEBUG_LOG("[ GGlobalQueue ]  -  Initialized");
	
	GJobTimer = new JobTimer();
	DEBUG_LOG("[ GJobTimer ]  -  Initialized");

	// GDBConnectionPool = new DBConnectionPool();
	//DEBUG_LOG("[ GDBConnectionPool ]  -  Initialized");

	GConsoleLogger = new ConsoleLog();
	DEBUG_LOG("[ GConsoleLogger ]  -  Initialized");

	 SocketUtils::Init();
	 DEBUG_LOG("[ GDeadLockProfiler ]  -  Initialized");

	DEBUG_LOG("Completed initialization of Core global variables");
}

CoreInitializer::~CoreInitializer()
{
    delete GThreadManager;
    GThreadManager = nullptr;
    delete GGlobalQueue;
    GGlobalQueue = nullptr;
    delete GSendBufferManager;
    GSendBufferManager = nullptr;
    delete GJobTimer;
    GJobTimer = nullptr;
    delete GDeadLockProfiler;
    GDeadLockProfiler = nullptr;


    // delete GDBConnectionPool;
    // GDBConnectionPool = nullptr;
    delete GConsoleLogger;
    GConsoleLogger = nullptr;

    delete GMemoryManager; // 풀링 때문에 마지막에 소멸시켜야 함(먼저 소멸시키면 SendBuffer 같은 풀링 객체 소멸자에서 Heap 터짐)
    GMemoryManager = nullptr;

    SocketUtils::Clear();
}

//class CoreGlobal
//{
//public:
//	/* 외부에서 ServerCore라이브러리를 링크하여 사용할 경우
//		Custom Memory Allocation 관련하여 오류가 발생한다.
//
//		이유는 전역객체 GMemoryManager가 먼저 초기화 되지 않아
//		Memory Pool이 비어있는 상황에서 Pop하려고 했기 때문이다.
//
//		따라서 아래와 같이 싱글톤으로 구현하여 사용한다.
//	*/
//	static CoreGlobal* GetInstance() {
//		static std::mutex mtx;
//		if (!G_CoreGlobal) {
//			std::lock_guard<std::mutex> guard(mtx);
//			if (!G_CoreGlobal)
//				G_CoreGlobal = new CoreGlobal();
//		}
//		return G_CoreGlobal;
//	}
//	CoreGlobal()
//	{
//		GDeadLockProfiler = new DeadLockProfiler();
//		GThreadManager = new ThreadManager();
//		GMemoryManager = new MemoryManager();
//		GSendBufferManager = new SendBufferManager();
//		GGlobalQueue = new GlobalQueue();
//		GJobTimer = new JobTimer();
//
//		GDBConnectionPool = new DBConnectionPool();
//		GConsoleLogger = new ConsoleLog();
//		SocketUtils::Init();
//	}
//
//	~CoreGlobal()
//	{
//		delete GThreadManager;
//		GThreadManager = nullptr;
//		delete GGlobalQueue;
//		GGlobalQueue = nullptr;
//		delete GSendBufferManager;
//		GSendBufferManager = nullptr;
//		delete GJobTimer;
//		GJobTimer = nullptr;
//		delete GDeadLockProfiler;
//		GDeadLockProfiler = nullptr;
//
//
//		delete GDBConnectionPool;
//		GDBConnectionPool = nullptr;
//		delete GConsoleLogger;
//		GConsoleLogger = nullptr;
//
//		delete GMemoryManager; // 풀링 때문에 마지막에 소멸시켜야 함(먼저 소멸시키면 SendBuffer 같은 풀링 객체 소멸자에서 Heap 터짐)
//		GMemoryManager = nullptr;
//
//		SocketUtils::Clear();
//	}
//
//private:
//	CoreGlobal(const CoreGlobal&) = delete;
//	CoreGlobal(CoreGlobal&&) = delete;
//	CoreGlobal& operator=(const CoreGlobal&) = delete;
//	CoreGlobal& operator=(CoreGlobal&&) = delete;
//	//CoreGlobal* operator=(CoreGlobal*) = delete;
//	std::mutex mtx;
//};


//
// CoreGlobal::CoreGlobal()
// {
// 	Init();
// }
//
// void CoreGlobal:: Init() {
// 	GDeadLockProfiler = new DeadLockProfiler();
// 	GThreadManager = new ThreadManager();
// 	GMemoryManager = new MemoryManager();
// 	GSendBufferManager = new SendBufferManager();
// 	GGlobalQueue = new GlobalQueue();
// 	GJobTimer = new JobTimer();
//
// 	// GDBConnectionPool = new DBConnectionPool();
// 	GConsoleLogger = new ConsoleLog();
// 	SocketUtils::Init();
// }
//
// CoreGlobal::~CoreGlobal()
// {
// 	delete GThreadManager;
// 	GThreadManager = nullptr;
// 	delete GGlobalQueue;
// 	GGlobalQueue = nullptr;
// 	delete GSendBufferManager;
// 	GSendBufferManager = nullptr;
// 	delete GJobTimer;
// 	GJobTimer = nullptr;
// 	delete GDeadLockProfiler;
// 	GDeadLockProfiler = nullptr;
//
//
// 	// delete GDBConnectionPool;
// 	// GDBConnectionPool = nullptr;
// 	delete GConsoleLogger;
// 	GConsoleLogger = nullptr;
//
// 	delete GMemoryManager; // 풀링 때문에 마지막에 소멸시켜야 함(먼저 소멸시키면 SendBuffer 같은 풀링 객체 소멸자에서 Heap 터짐)
// 	GMemoryManager = nullptr;
//
// 	SocketUtils::Clear();
// }


/*
class CoreGlobal
{
public:

	static CoreGlobal& GetInstance()
	{
		//static CoreGlobal coreGlobal;
		return G_CoreGlobal;
	}

	CoreGlobal()
	{
		GDeadLockProfiler = new DeadLockProfiler();
		GThreadManager = new ThreadManager();
		//GMemoryManager = new MemoryManager();
		GSendBufferManager = new SendBufferManager();
		GGlobalQueue = new GlobalQueue();
		GJobTimer = new JobTimer();

		GDBConnectionPool = new DBConnectionPool();
		GConsoleLogger = new ConsoleLog();
		SocketUtils::Init();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		GThreadManager = nullptr;
		delete GGlobalQueue;
		GGlobalQueue = nullptr;
		delete GSendBufferManager;
		GSendBufferManager = nullptr;
		delete GJobTimer;
		GJobTimer = nullptr;
		delete GDeadLockProfiler;
		GDeadLockProfiler = nullptr;


		delete GDBConnectionPool;
		GDBConnectionPool = nullptr;
		delete GConsoleLogger;
		GConsoleLogger = nullptr;

		delete GMemoryManager; // 풀링 때문에 마지막에 소멸시켜야 함(먼저 소멸시키면 SendBuffer 같은 풀링 객체 소멸자에서 Heap 터짐)
		GMemoryManager = nullptr;

		SocketUtils::Clear();
	}
private:
	CoreGlobal(const CoreGlobal&) = delete;
	CoreGlobal(CoreGlobal&&) = delete;
	CoreGlobal& operator=(const CoreGlobal&) = delete;
	CoreGlobal& operator=(CoreGlobal&&) = delete;
}G_CoreGlobal;
*/
}
