#pragma once

/*----------------
	GlobalQueue
-----------------*/

/* shared_ptr<JobQueue>를 넘겨 다른 여유 있는 쓰레드에서 
   JobQueue의 일감을 처리할 수 있도록하는 일종의 분산 시스템*/
class GlobalQueue
{
public:
	GlobalQueue();
	~GlobalQueue();

	void					Push(JobQueueRef jobQueue);
	JobQueueRef				Pop();

private: 
	LockQueue<JobQueueRef> _jobQueues;
};