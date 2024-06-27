#pragma once
#include <memory>
#include "Types.h"

class ISession;

// IOCP 생성 및 GQCS 함수 호출


class IocpService
{
    typedef void* HANDLE;

public:
    // IOCP 생성 
    IocpService();
    // IOCP 자원 해제
    ~IocpService();

    HANDLE GetIOCPHandle() { return iocpHandle; }

    bool RegisterSockToIOCP(std::shared_ptr<ISession> iocpObject);
    bool Dispatch(uint32 timeoutMs = UINF);

private:
    HANDLE iocpHandle;
};
