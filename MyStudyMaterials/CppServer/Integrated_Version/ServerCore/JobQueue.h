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
	// CallbackType = std::function<void()>
	void DoAsync(CallbackType&& callback)
	{
		Push(ObjectPool<Job>::MakeShared(std::move(callback)));
	}

	/* 
	Ret(T::*memFunc)(Args...)는 Template 문법으로 함수 포인터를 구현한 것이다.
	템플릿인자 T의 멤버 함수에 대한 함수 포인터를 의미다.
	즉, 반환타입이 Ret이며, 가변 인자 Args...를 매개변수로 갖는 T클래스의 멤버 함수 포인터를 지칭한다.
	*/
	template<typename T, typename Ret, typename... Args>
	void DoAsync(Ret(T::*memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		Push(ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...));
	}

	void DoTimer(uint64 tickAfter, CallbackType&& callback)
	{
		JobRef job = ObjectPool<Job>::MakeShared(std::move(callback));
		GJobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	template<typename T, typename Ret, typename... Args>
	void DoTimer(uint64 tickAfter, Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		JobRef job = ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...);
		GJobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	void					ClearJobs() { _jobs.Clear(); }

public:
	// pushOnly == false : Job처리까지 수행
	void					Push(JobRef job, bool pushOnly = false);
	void					Execute();

protected:
	LockQueue<JobRef>		_jobs;
	Atomic<int32>			_jobCount = 0;
};

