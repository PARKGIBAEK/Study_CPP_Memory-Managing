#include "pch.h"
#include "JobTimer.h"
#include "JobQueue.h"

/*--------------
	JobTimer
---------------*/

void JobTimer::Reserve(uint64 tickAfter, weak_ptr<JobQueue> owner, JobRef job)
{
	const uint64 executeTick = ::GetTickCount64() + tickAfter;
	JobData* jobData = ObjectPool<JobData>::Pop(owner, job);

	WRITE_LOCK;

	_items.push(TimerItem{ executeTick, jobData });
}

void JobTimer::Distribute(uint64 now)
{
	/* �� ���� �� �����常 ���
	 - ������A�� _items���� ���� ó���ؾ��� Job�� ���� ���� ó���Ϸ��� �ϴ� ����
	   �ٸ� ������ B�� _items���� ���� ������ ó���ؾ��� Job�� �޾ƿͼ�
	   ������A�� �޾ư� Job���� ���� ó���ع����� ������ ���� �� �ֱ� ����
	*/
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
		if (JobQueueRef owner = item.jobData->owner.lock())
			owner->Push(item.jobData->job, true);// JobQueue�� Push���ϰ� ������ ��Ű�� ����

		ObjectPool<JobData>::Push(item.jobData);
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
