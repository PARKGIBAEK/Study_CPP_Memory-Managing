#pragma once
#include <memory>


namespace ServerCore
{
class JobQueue;
class Job;

struct JobData
{
    JobData(std::weak_ptr<JobQueue> _owner, std::shared_ptr<Job> _job);

    std::weak_ptr<JobQueue>		owner;// Job을 실행해야할 JobQueue
    std::shared_ptr<Job>		job;
};
}