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
	/* ����Ʈ�����Ͱ� �ƴ϶� Raw Pointer�� ����
	  Job���� �������� TimerItem�� �̸����� �Űܴٴϰ� �� �� �ִµ�
	  �� �� reference counting�� �߻��ϴ� ���� ���� ����*/
	JobData* jobData = nullptr;
};

/*--------------
	JobTimer
---------------*/
/*
  �������� 1���� �����ϴ� ��ü�� ���
  - TimerItem�� ����ð��� �������� ����� �� �ְ� JobQueue�� �־� �ִ� ����
 */
class JobTimer
{
public:
	/* Priority_Queue�� Job�� �ֱ�*/
	void			Reserve(uint64 tickAfter, std::weak_ptr<JobQueue> owner, JobRef job);
	// Job ���� �ð��� �������� owner(JobQueue)���� ����
	void			Distribute(uint64 now);

	void			Clear();

private:
	USE_LOCK;
	PriorityQueue<TimerItem>	_items;
	Atomic<bool>				_distributing = false;
};