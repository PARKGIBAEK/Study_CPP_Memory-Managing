#pragma once
#include <memory>
#include "../Util/LockQueue.h"


namespace ServerCore
{
class JobQueue;
/* std::shared_ptr<JobQueue>�� �Ѱ� �ٸ� ���� �ִ� �����忡�� 
   JobQueue�� �ϰ��� ó���� �� �ֵ����ϴ� ������ �л� �ý���*/
class GlobalQueue
{
public:
	void					Push(std::shared_ptr<JobQueue> jobQueue);
	std::shared_ptr<JobQueue> 				Pop();

private: 
	LockQueue<std::shared_ptr<JobQueue> > _jobQueues;
};
}