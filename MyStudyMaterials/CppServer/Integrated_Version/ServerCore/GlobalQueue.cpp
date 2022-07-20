#include "pch.h"
#include "GlobalQueue.h"

/*----------------
	GlobalQueue
-----------------*/

GlobalQueue::GlobalQueue()
{

}

GlobalQueue::~GlobalQueue()
{

}

void GlobalQueue::Push(std::shared_ptr<JobQueue> jobQueue)
{
	_jobQueues.Push(jobQueue);
}

std::shared_ptr<JobQueue> GlobalQueue::Pop()
{
	return _jobQueues.Pop();
}
