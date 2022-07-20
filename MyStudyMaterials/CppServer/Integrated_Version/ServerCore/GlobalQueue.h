#pragma once

/*----------------
	GlobalQueue
-----------------*/

/* std::shared_ptr<JobQueue>�� �Ѱ� �ٸ� ���� �ִ� �����忡�� 
   JobQueue�� �ϰ��� ó���� �� �ֵ����ϴ� ������ �л� �ý���*/
class GlobalQueue
{
public:
	GlobalQueue();
	~GlobalQueue();

	void					Push(std::shared_ptr<JobQueue> jobQueue);
	std::shared_ptr<JobQueue> 				Pop();

private: 
	LockQueue<std::shared_ptr<JobQueue> > _jobQueues;
};