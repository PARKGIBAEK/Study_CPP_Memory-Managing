#include "TimerItem.h"
#include "JobData.h"
// TimerItem::TimerItem(): executeTick(0), jobData(nullptr)
// {
// }
//
// TimerItem::TimerItem(uint64 uint64, JobData* job_data)
// : executeTick(0), jobData(nullptr)
// {
// }

bool TimerItem::operator<(const TimerItem& _other) const
{
    return executeTick > _other.executeTick;
}
