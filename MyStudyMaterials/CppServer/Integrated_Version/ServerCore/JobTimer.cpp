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
	/* atomic lock (한 번에 한 쓰레드만 통과) */
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
		if (std::shared_ptr<JobQueue> owner = item.jobData->owner.lock())
			owner->Push(item.jobData->job, true);// JobQueue에 Push만하고 실행은 시키지 않음

		ObjectPool<JobData>::Push(item.jobData); // pool에 반환
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