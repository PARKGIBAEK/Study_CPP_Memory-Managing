#pragma once

struct JobData
{
	JobData(std::weak_ptr<JobQueue> owner, JobRef job) : owner(owner), job(job)
	{
		
	}

	std::weak_ptr<JobQueue>	owner;// Job을 실행해야할 JobQueue
	JobRef				job;
};

struct TimerItem
{
	bool operator<(const TimerItem& other) const
	{
		return executeTick > other.executeTick;
	}

	uint64 executeTick = 0;
	/* 스마트포인터가 아니라 Raw Pointer인 이유
	  Job제어 과정에서 TimerItem이 이리저리 옮겨다니게 될 수 있는데
	  그 때 reference counting이 발생하는 것을 막기 위함*/
	JobData* jobData = nullptr;
};

/*--------------
	JobTimer
---------------*/
/*
  전역으로 1개만 존재하는 객체로 사용
  - TimerItem의 실행시각이 지났으면 실행될 수 있게 JobQueue에 넣어 주는 역할
 */
class JobTimer
{
public:
	/* Priority_Queue에 Job을 넣기*/
	void			Reserve(uint64 tickAfter, std::weak_ptr<JobQueue> owner, JobRef job);
	// Job 실행 시각이 지났으면 owner(JobQueue)에게 전달
	void			Distribute(uint64 now);

	void			Clear();

private:
	USE_LOCK;
	PriorityQueue<TimerItem>	_items;
	Atomic<bool>				_distributing = false;
};