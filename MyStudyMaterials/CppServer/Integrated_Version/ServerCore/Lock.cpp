#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"
#include "DeadLockProfiler.h"

void Lock::WriteLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif

	const uint32 lockThreadId = (lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (tls_ThreadId == lockThreadId)
	{ // ������ �����尡 WriteLock�� ��� �ִ� ���(��ͷ� Lock�� ���� ���)
		writeCount++;
		return;
	}

	// WriteLock�� ��� ���� ����
	const int64 beginTick = ::GetTickCount64();
	const uint32 desired = ((tls_ThreadId << 16) & WRITE_THREAD_MASK);
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = EMPTY_FLAG;
			if (lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				writeCount++; // WriteLock�� ���� ������ ���� 1 ����
				return;
			}
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::WriteUnlock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif

	// WriteLock�� ���� �ִ� ���¿��� ReadLock�� �Բ� �����ִٸ� ���� ����
	if ((lockFlag.load() & READ_COUNT_MASK) != 0)
		CRASH("INVALID_UNLOCK_ORDER");

	const int32 lockCount = --writeCount;
	if (lockCount == 0)
		lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif

	
	const uint32 lockThreadId = (lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (tls_ThreadId == lockThreadId)
	{ // WriteLock�� ����ִ� �����尡 ReadLock�� �������� �ϴ� ���
		lockFlag.fetch_add(1); // ReadLock�� ���� ������ ���� ����
		return;
	}

	// �ƹ��� �����ϰ� ���� ���� �� �����ؼ� ���� ī��Ʈ�� �ø���.
	const int64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = (lockFlag.load() & READ_COUNT_MASK);
			if (lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::ReadUnlock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif

	if ((lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("MULTIPLE_UNLOCK");
}
