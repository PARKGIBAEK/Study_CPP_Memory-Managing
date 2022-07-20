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
	/* jobData를 shared_ptr로 만들지 않고, raw pointer로 만든 이유는 ?
	  - JobData가 이리저리 옮겨다니면 Ref count 변경으로 인한 비용 발생 */
	JobData* jobData = nullptr;
};

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
	void			Reserve(uint64 tickAfter, std::weak_ptr<JobQueue> owner, JobRef job);
	// Job 실행 시각이 지났으면 owner(JobQueue)에게 전달
	void			Distribute(uint64 now);

	void			Clear();

private:
	USE_LOCK;
	PriorityQueue<TimerItem>	_items;
	Atomic<bool>				_distributing = false;
};