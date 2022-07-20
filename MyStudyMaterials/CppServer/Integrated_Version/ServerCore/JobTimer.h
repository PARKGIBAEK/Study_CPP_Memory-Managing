#pragma once

struct JobData
{
	JobData(std::weak_ptr<JobQueue> owner, JobRef job) : owner(owner), job(job)
	{
		
	}

	std::weak_ptr<JobQueue>	owner;// Job�� �����ؾ��� JobQueue
	JobRef				job;
};

struct TimerItem
{
	bool operator<(const TimerItem& other) const
	{
		return executeTick > other.executeTick;
	}

	uint64 executeTick = 0;
	/* jobData�� shared_ptr�� ������ �ʰ�, raw pointer�� ���� ������ ?
	  - JobData�� �̸����� �Űܴٴϸ� Ref count �������� ���� ��� �߻� */
	JobData* jobData = nullptr;
};

/*--------------
	JobTimer
---------------*/
/*
  ���� ��ü�̸�, TimerItem�� executeTick�� ���� Job��
  - TimerItem�� ����ð��� �������� ����� �� �ְ� JobQueue�� �־� �ִ� ����
*/
class JobTimer
{
public:
	/* Priority_Queue�� Job�� executeTick�������� �ֱ�*/
	void			Reserve(uint64 tickAfter, std::weak_ptr<JobQueue> owner, JobRef job);
	// Job ���� �ð��� �������� owner(JobQueue)���� ����
	void			Distribute(uint64 now);

	void			Clear();

private:
	USE_LOCK;
	PriorityQueue<TimerItem>	_items;
	Atomic<bool>				_distributing = false;
};