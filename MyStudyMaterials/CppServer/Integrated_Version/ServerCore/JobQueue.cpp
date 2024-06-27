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

    // 첫번째 Job을 넣은 쓰레드가 실행까지 담당
    if (prevCount == 0)
    {
        /* tls_CurrentJobQueue가 nullptr이 아니라면 로직 오류(다른 Thread에서 간섭)발생.
          JobQueue::Execute()가 정상적으로 반환했다면 
          tls_CurrentJobQueue는 nullptr이어야 한다.
        
        * pushOnly == true이면
          Job실행하지 않고 GlobalQueue에 넣어서 다른 쓰레드가 가져갈 수 있게 함*/
        if (tls_CurrentJobQueue == nullptr && pushOnly == false)
        {
            Execute();
        }
        else
        {
            // 여유 있는 다른 쓰레드가 실행하도록 GlobalQueue에 넘긴다
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

        // 가져온 Job을 제외하고 남은 일감이 0개라면 종료
        if (_jobCount.fetch_sub(jobCount) == jobCount)
        {
            tls_CurrentJobQueue = nullptr;
            return;
        }

        /* 처리해야할 일감이 남았지만 
            현재 쓰레드에서 일감 처리에 시간을 너무 오래 할애 했다면
            GlobalQueue에 넘겨 다른 쓰레드에서 처리할 수 있도록한다*/
        const uint64 now = ::GetTickCount64();
        if (now >= tls_EndTickCount)
        {
            tls_CurrentJobQueue = nullptr;
            // 여유 있는 다른 쓰레드가 실행하도록 GlobalQueue에 넘긴다
            GGlobalQueue->Push(shared_from_this());
            break;
        }
    }
}
