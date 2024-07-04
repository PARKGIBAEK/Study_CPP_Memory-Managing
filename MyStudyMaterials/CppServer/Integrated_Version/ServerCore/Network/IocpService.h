#pragma once
#include <memory>
#include "../Core/Types.h"

namespace ServerCore
{
class IDispatchable;

// IOCP 생성 및 GQCS 함수 호출


class IocpService
{
    typedef void* HANDLE;

public:
    // IOCP 생성 
    IocpService();
    // IOCP 자원 해제
    ~IocpService();

    HANDLE GetIOCPHandle() const { return iocpHandle; }

    bool RegisterSocketToIocp(std::shared_ptr<IDispatchable> _iocpObject);
    bool Dispatch(uint32 _timeoutMs = UINF);

private:
    HANDLE iocpHandle;
};
}
