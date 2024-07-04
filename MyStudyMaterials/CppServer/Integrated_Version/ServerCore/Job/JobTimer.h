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
  ���� ��ü�̸�, TimerItem�� executeTick�� ���� Job��
  - TimerItem�� ����ð��� �������� ����� �� �ְ� JobQueue�� �־� �ִ� ����
*/
class JobTimer
{
public:
	/* Priority_Queue�� Job�� executeTick�������� �ֱ�*/
	void			Reserve(uint64 tickAfter, std::weak_ptr<JobQueue> owner, std::shared_ptr<Job> job);
	// Job ���� �ð��� �������� owner(JobQueue)���� ����
	void			Distribute(uint64 now);

	void			Clear();

private:
	USE_LOCK;
	PriorityQueue<TimerItem>		_items;
	std::atomic<bool>				_distributing;
};
}