#include "JobData.h"
#include "JobQueue.h"
#include "Job.h"
#include <utility>

JobData::JobData(std::weak_ptr<JobQueue> _owner, std::shared_ptr<Job> _job)
    : owner(std::move(_owner)), job(std::move(_job))
{
}
