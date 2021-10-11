#pragma once

#include <thread>
#include <functional>

/*------------------
	ThreadManager
-------------------*/

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	/* 쓰레드 컨테이너(vector<thread>	threads)에 쓰레드를 추가하고,
	동시에 해당 쓰레에서 함수객체 callback을 수행한다*/
	void	Launch(function<void(void)> callback);
	/* 쓰레드 컨테이너(vector<thread>	threads)에 있는 모든 쓰레드들에 대하여 join을 수행하고,
	join이 반환하면 쓰레드 컨테이너를 비운다*/
	void	Join();
	// 쓰레드 ID를 부여하고 각쓰레드의 TLS에 해당 쓰레드 ID를 전달해 줌
	static void InitTLS();
	static void  DestroyTLS();

	static void DoGlobalQueueWork();
	static void DistributeReservedJobs();

private:
	Mutex			mtxLock;
	vector<thread>	threads;
};

