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
	/* 한 번에 한 쓰레드만 통과
	 - 쓰레드A가 _items에서 먼저 처리해야할 Job을 꺼낸 다음 처리하려고 하는 순간
	   다른 쓰레드 B가 _items에서 다음 순서로 처리해야할 Job을 받아와서
	   쓰레드A가 받아간 Job보다 먼저 처리해버리면 문제가 생길 수 있기 때문
	*/
	if (_distributing.exchange(true) == true)
		return;

	Vector<TimerItem> items;

	{
		WRITE_LOCK;

		while (_items.empty() == false)
		{ // 실행해야할 TimerItem(Job)을 꺼낸다
			const TimerItem& timerItem = _items.top();
			if (now < timerItem.executeTick)
				break;

			items.push_back(timerItem);
			_items.pop();
		}
	}

	for (TimerItem& item : items)
	{ // 꺼내온 Job을 소유하고 있는 JobQueue가 유효한 경우 소유자인 JoqQueue에 넣어준다
		if (JobQueueRef owner = item.jobData->owner.lock())
			owner->Push(item.jobData->job, true);// JobQueue에 Push만하고 실행은 시키지 않음

		ObjectPool<JobData>::Push(item.jobData);
	}

	// 끝났으면 풀어준다
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
