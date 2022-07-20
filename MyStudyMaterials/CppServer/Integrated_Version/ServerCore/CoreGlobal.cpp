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

ThreadManager* GThreadManager = nullptr;
//MemoryManager* GMemoryManager = new MemoryManager();
MemoryManager* GMemoryManager = nullptr;
SendBufferManager* GSendBufferManager = nullptr;
GlobalQueue* GGlobalQueue = nullptr;
JobTimer* GJobTimer = nullptr;

DeadLockProfiler* GDeadLockProfiler = nullptr;
DBConnectionPool* GDBConnectionPool = nullptr;
ConsoleLog* GConsoleLogger = nullptr;

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
		GMemoryManager = new MemoryManager();
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

		delete GMemoryManager; // Ǯ�� ������ �������� �Ҹ���Ѿ� ��(���� �Ҹ��Ű�� SendBuffer ���� Ǯ�� ��ü �Ҹ��ڿ��� Heap ����)
		GMemoryManager = nullptr;

		SocketUtils::Clear();
	}
private:
	CoreGlobal(const CoreGlobal&) = delete;
	CoreGlobal(CoreGlobal&&) = delete;
	CoreGlobal& operator=(const CoreGlobal&) = delete;
	CoreGlobal& operator=(CoreGlobal&&) = delete;
}G_CoreGlobal;