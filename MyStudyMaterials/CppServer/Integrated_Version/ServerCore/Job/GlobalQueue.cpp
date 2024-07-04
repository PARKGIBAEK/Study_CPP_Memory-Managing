#include "GlobalQueue.h"
#include "../Job/JobQueue.h"


namespace ServerCore
{
void GlobalQueue::Push(std::shared_ptr<JobQueue> jobQueue)
{
	_jobQueues.Push(jobQueue);
}

std::shared_ptr<JobQueue> GlobalQueue::Pop()
{
	return _jobQueues.Pop();
}
}