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
	/* �ܺο��� ServerCore���̺귯���� ��ũ�Ͽ� ����� ���
		Custom Memory Allocation �����Ͽ� ������ �߻��Ѵ�.

		������ ������ü GMemoryManager�� ���� �ʱ�ȭ ���� �ʾ�
		Memory Pool�� ����ִ� ��Ȳ���� Pop�Ϸ��� �߱� �����̴�.

		���� �Ʒ��� ���� �̱������� �����Ͽ� ����Ѵ�.
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