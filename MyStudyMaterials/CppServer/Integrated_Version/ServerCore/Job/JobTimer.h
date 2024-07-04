#pragma once
#include <memory>
#include "../Memory/Container.h"
#include "../Core/CoreMacro.h"
#include "../Core/Types.h"
#include "../Job/TimerItem.h"

namespace ServerCore
{
class Job;
class JobQueue;
/*--------------
	JobTimer
---------------*/
/*
  전역 객체이며, TimerItem의 executeTick에 맞춰 Job을
  - TimerItem의 실행시각이 지났으면 실행될 수 있게 JobQueue에 넣어 주는 역할
*/
class JobTimer
{
public:
	/* Priority_Queue에 Job을 executeTick기준으로 넣기*/
	void			Reserve(uint64 tickAfter, std::weak_ptr<JobQueue> owner, std::shared_ptr<Job> job);
	// Job 실행 시각이 지났으면 owner(JobQueue)에게 전달
	void			Distribute(uint64 now);

	void			Clear();

private:
	USE_LOCK;
	PriorityQueue<TimerItem>		_items;
	std::atomic<bool>				_distributing;
};
}