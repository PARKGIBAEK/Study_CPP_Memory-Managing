#include "JobData.h"
#include "../Job/JobQueue.h"
#include "../Job/Job.h"
#include <utility>


namespace ServerCore
{
JobData::JobData(std::weak_ptr<JobQueue> _owner, std::shared_ptr<Job> _job)
    : owner(std::move(_owner)), job(std::move(_job))
{
}
}