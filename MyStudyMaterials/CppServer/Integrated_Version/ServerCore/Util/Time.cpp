#include "Time.h"
#include <chrono>

namespace ServerCore
{
uint64_t Time::GetTickCount64()
{
    // 현재 시간 가져오기
    auto now = std::chrono::steady_clock::now();

    // 부팅 이후 경과 시간을 밀리초 단위로 변환
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

    // 밀리초 단위의 경과 시간을 uint64_t로 반환
    return duration.count();
}
}