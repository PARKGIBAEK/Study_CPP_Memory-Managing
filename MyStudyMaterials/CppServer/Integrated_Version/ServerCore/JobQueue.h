#pragma once
#include "Job.h"
#include "LockQueue.h"
#include "JobTimer.h"

/*--------------
	JobQueue
---------------*/


class JobQueue : public enable_shared_from_this<JobQueue>
{
public:
	JobQueue()
	{
		
	}
	// CallbackType = std::function<void()>
	void DoAsync(CallbackType&& callback)
	{
		Push( ObjectPool<Job>::MakeShared(std::move(callback)) );
	}

	/* 
	Ret(T::*memFunc)(Args...)�� Template �������� �Լ� �����͸� ������ ���̴�.
	���ø����� T�� ��� �Լ��� ���� �Լ� �����͸� �ǹ̴�.
	��, ��ȯŸ���� Ret�̸�, ���� ���� Args...�� �Ű������� ���� TŬ������ ��� �Լ� �����͸� ��Ī�Ѵ�.
	*/
	template<typename T, typename Ret, typename... Args>
	void DoAsync(Ret(T::*memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		Push(ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...));
	}

	void DoTimer(uint64 tickAfter, CallbackType&& callback)
	{
		std::shared_ptr<Job> job = ObjectPool<Job>::MakeShared(std::move(callback));
		GJobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	template<typename T, typename Ret, typename... Args>
	void DoTimer(uint64 tickAfter, Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		std::shared_ptr<Job> job = ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...);
		GJobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	void					ClearJobs() { _jobs.Clear(); }

public:
	/* _jobs�� job�� �ִ´�. ��, �ٻڸ� 
	pushOnly == false : Jobó������ ����*/

	void					Push(std::shared_ptr<Job> job, bool pushOnly = false);
	void					Execute();

protected:
	LockQueue<std::shared_ptr<Job>>		_jobs;
	Atomic<int32>			_jobCount = 0;
};

