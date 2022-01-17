#pragma once
#include "Types.h"

/*----------------
	 RW SpinLock
-----------------*/

/*--------------------------------------------
32비트 변수(비트 플래그 용도) : [WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
W : WriteFlag (Exclusive Lock Owner ThreadId)
R : ReadFlag (Shared Lock Count)
---------------------------------------------*/

class Lock
{
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};

public:
	void WriteLock(const char* name);
	void WriteUnlock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};

/*----------------
	 LockGuards
-----------------*/

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& _lock, const char* name) : lock(_lock), _name(name) { lock.ReadLock(name); }
	~ReadLockGuard() { lock.ReadUnlock(_name); }

private:
	Lock& lock;
	const char* _name;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) : lock(lock), _name(name) { lock.WriteLock(name); }
	~WriteLockGuard() { lock.WriteUnlock(_name); }

private:
	Lock& lock;
	const char* _name;
};