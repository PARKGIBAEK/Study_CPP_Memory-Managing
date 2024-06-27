#include "JobTimer.h"
#include "Job.h"
#include "JobData.h"
#include "JobQueue.h"

/*--------------
	JobTimer
---------------*/

void JobTimer::Reserve(uint64 tickAfter, std::weak_ptr<JobQueue> owner, std::shared_ptr<Job> job)
{
	const uint64 executeTick = ::GetTickCount64() + tickAfter;
	JobData* jobData = ObjectPool<JobData>::Pop(owner, job);

	WRITE_LOCK;

	 _items.emplace(TimerItem{ executeTick, jobData });
}

void JobTimer::Distribute(uint64 now)
{
	/* atomic lock (�� ���� �� �����常 ���) */
	if (_distributing.exchange(true) == true)
		return;

	Vector<TimerItem> items;

	{
		WRITE_LOCK;

		while (_items.empty() == false)
		{ // �����ؾ��� TimerItem(Job)�� ������
			const TimerItem& timerItem = _items.top();
			if (now < timerItem.executeTick)
				break;

			items.push_back(timerItem);
			_items.pop();
		}
	}

	for (TimerItem& item : items)
	{ // ������ Job�� �����ϰ� �ִ� JobQueue�� ��ȿ�� ��� �������� JoqQueue�� �־��ش�
		if (std::shared_ptr<JobQueue> owner = item.jobData->owner.lock())
			owner->Push(item.jobData->job, true);// JobQueue�� Push���ϰ� ������ ��Ű�� ����

		ObjectPool<JobData>::Push(item.jobData); // pool�� ��ȯ
	}

	// �������� Ǯ���ش�
	_distributing.store(false);
}

void JobTimer::Clear()
{
	WRITE_LOCK;

	while (_items.empty() == false)
	{
		const TimerItem& timerItem = _items.top();
		ObjectPool<JobData>::Push(timerItem.jobData);
		_items.pop();
	}
}