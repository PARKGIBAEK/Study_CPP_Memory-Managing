#pragma once

extern class ThreadManager*		GThreadManager;
extern class MemoryManager*		GMemoryManager;
extern class SendBufferManager* GSendBufferManager;
extern class GlobalQueue*		GGlobalQueue;
extern class JobTimer*			GJobTimer;
extern class DeadLockProfiler*	GDeadLockProfiler;


extern class DBConnectionPool*	GDBConnectionPool;
extern class ConsoleLog*		GConsoleLogger;


class CoreGlobal
{
public:
	/* 외부에서 ServerCore라이브러리를 링크하여 사용할 경우
		Custom Memory Allocation 관련하여 오류가 발생한다.

		이유는 전역객체 GMemoryManager가 먼저 초기화 되지 않아
		Memory Pool이 비어있는 상황에서 Pop하려고 했기 때문이다.

		따라서 아래와 같이 싱글톤으로 구현하여 사용한다.
	*/
	/*static CoreGlobal* GetInstance() {
		static std::mutex mtx;
		if (!G_CoreGlobal) {
			std::lock_guard<std::mutex> guard(mtx);
			if (!G_CoreGlobal)
				G_CoreGlobal = new CoreGlobal();
		}
		return G_CoreGlobal;
	}*/
	CoreGlobal();
	~CoreGlobal();

	void Init();

private:
	//CoreGlobal(const CoreGlobal&) = delete;
	//CoreGlobal(CoreGlobal&&) = delete;
	//CoreGlobal& operator=(const CoreGlobal&) = delete;
	//CoreGlobal& operator=(CoreGlobal&&) = delete;
	//CoreGlobal* operator=(CoreGlobal*) = delete;
	std::mutex mtx;
};

//extern CoreGlobal* G_CoreGlobal;