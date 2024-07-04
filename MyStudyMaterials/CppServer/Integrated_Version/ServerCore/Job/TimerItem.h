#pragma once
#include "../Core/Types.h"

namespace ServerCore
{
struct JobData;

struct TimerItem
{
    // TimerItem();
    // TimerItem(uint64 uint64, JobData* job_data);
    bool operator<(const TimerItem& _other) const;

    uint64 executeTick;
    /* jobData를 shared_ptr로 만들지 않고, raw pointer로 만든 이유는 ?
      - JobData가 이리저리 옮겨다니면 Ref count 변경으로 인한 비용 발생 */
    JobData* jobData;
};
}