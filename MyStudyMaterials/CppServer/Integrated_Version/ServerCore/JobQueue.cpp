#include "JobQueue.h"

#include "CoreTLS.h"
#include "GlobalQueue.h"

/*--------------
	JobQueue
---------------*/

void JobQueue::Push(std::shared_ptr<Job> job, bool pushOnly)
{
    const int32 prevCount = _jobCount.fetch_add(1);
    _jobs.Push(job); // WRITE_LOCK

    // ù��° Job�� ���� �����尡 ������� ���
    if (prevCount == 0)
    {
        /* tls_CurrentJobQueue�� nullptr�� �ƴ϶�� ���� ����(�ٸ� Thread���� ����)�߻�.
          JobQueue::Execute()�� ���������� ��ȯ�ߴٸ� 
          tls_CurrentJobQueue�� nullptr�̾�� �Ѵ�.
        
        * pushOnly == true�̸�
          Job�������� �ʰ� GlobalQueue�� �־ �ٸ� �����尡 ������ �� �ְ� ��*/
        if (tls_CurrentJobQueue == nullptr && pushOnly == false)
        {
            Execute();
        }
        else
        {
            // ���� �ִ� �ٸ� �����尡 �����ϵ��� GlobalQueue�� �ѱ��
            GGlobalQueue->Push(shared_from_this());
        }
    }
}

void JobQueue::Execute()
{
    tls_CurrentJobQueue = this;

    while (true)
    {
        Vector<std::shared_ptr<Job>> jobs;
        _jobs.PopAll(OUT jobs);

        const int32 jobCount = static_cast<int32>(jobs.size());
        for (int32 i = 0; i < jobCount; i++)
            jobs[i]->Execute();

        // ������ Job�� �����ϰ� ���� �ϰ��� 0����� ����
        if (_jobCount.fetch_sub(jobCount) == jobCount)
        {
            tls_CurrentJobQueue = nullptr;
            return;
        }

        /* ó���ؾ��� �ϰ��� �������� 
            ���� �����忡�� �ϰ� ó���� �ð��� �ʹ� ���� �Ҿ� �ߴٸ�
            GlobalQueue�� �Ѱ� �ٸ� �����忡�� ó���� �� �ֵ����Ѵ�*/
        const uint64 now = ::GetTickCount64();
        if (now >= tls_EndTickCount)
        {
            tls_CurrentJobQueue = nullptr;
            // ���� �ִ� �ٸ� �����尡 �����ϵ��� GlobalQueue�� �ѱ��
            GGlobalQueue->Push(shared_from_this());
            break;
        }
    }
}
