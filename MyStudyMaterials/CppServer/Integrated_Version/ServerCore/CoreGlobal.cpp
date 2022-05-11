#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "MemoryManager.h"
#include "DeadLockProfiler.h"
#include "SocketUtils.h"
#include "SendBuffer.h"
#include "GlobalQueue.h"
#include "JobTimer.h"
#include "DBConnectionPool.h"
#include "ConsoleLog.h"

ThreadManager*		GThreadManager = nullptr;
MemoryManager*		GMemoryManager = nullptr;
SendBufferManager*	GSendBufferManager = nullptr;
GlobalQueue*		GGlobalQueue = nullptr;
JobTimer*			GJobTimer = nullptr;

DeadLockProfiler*	GDeadLockProfiler = nullptr;
DBConnectionPool*	GDBConnectionPool = nullptr;
ConsoleLog*			GConsoleLogger = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GMemoryManager = new MemoryManager();
		GSendBufferManager = new SendBufferManager();
		GGlobalQueue = new GlobalQueue();
		GJobTimer = new JobTimer();
		GDeadLockProfiler = new DeadLockProfiler();


		GDBConnectionPool = new DBConnectionPool();
		GConsoleLogger = new ConsoleLog();
		SocketUtils::Init();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemoryManager;
		delete GSendBufferManager;
		delete GGlobalQueue;
		delete GJobTimer;
		delete GDeadLockProfiler;


		delete GDBConnectionPool;
		delete GConsoleLogger;
		SocketUtils::Clear();
	}
} GCoreGlobal;