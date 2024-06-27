#include "ThreadManager.h"

#include "CoreTLS.h"
#include "CoreGlobal.h"
#include "GlobalQueue.h"
#include "JobQueue.h"
#include "JobTimer.h"
#include "Time.h"
#include <atomic>
/*------------------
	ThreadManager
-------------------*/

ThreadManager::ThreadManager()
{
	// Main Thread
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(std::function<void(void)> callback)
{
	LockGuard guard(mtxLock);

	threads.push_back(std::thread([=]()
		{
			InitTLS();
			callback();
			DestroyTLS();
		}));
}

void ThreadManager::Join()
{
	for (std::thread& t : threads)
	{
		if (t.joinable())
			t.join();
	}
	threads.clear();
}

void ThreadManager::InitTLS()
{
	static std::atomic<uint32> SThreadId = 1;
	tls_ThreadId = SThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{

}

void ThreadManager::DoGlobalQueueWork()
{
	while (true)
	{
		uint64 now = Time::GetTickCount64();
		if (now > tls_EndTickCount)
			break;

		std::shared_ptr<JobQueue> jobQueue = GGlobalQueue->Pop();
		if (jobQueue == nullptr)
			break;

		jobQueue->Execute();
	}
}

void ThreadManager::CheckJobTimer()
{
	const uint64 now = Time::GetTickCount64();

	GJobTimer->Distribute(now);
}


