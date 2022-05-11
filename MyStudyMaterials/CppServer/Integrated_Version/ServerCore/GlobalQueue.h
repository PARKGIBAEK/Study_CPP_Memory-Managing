#pragma once

/*----------------
	GlobalQueue
-----------------*/

/* shared_ptr<JobQueue>�� �Ѱ� �ٸ� ���� �ִ� �����忡�� 
   JobQueue�� �ϰ��� ó���� �� �ֵ����ϴ� ������ �л� �ý���*/
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