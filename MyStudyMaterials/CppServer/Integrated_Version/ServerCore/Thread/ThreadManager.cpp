#include "ThreadManager.h"

#include "../Core/CoreTLS.h"
#include "../Core/CoreInitializer.h"
#include "../Job/GlobalQueue.h"
#include "../Job/JobQueue.h"
#include "../Job/JobTimer.h"
#include "../Util/Time.h"
#include <atomic>


namespace ServerCore
{
ThreadManager::ThreadManager()
{
	// Main Thread
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	JoinAll();
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

void ThreadManager::JoinAll()
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
}