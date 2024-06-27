#pragma once
#include <memory>
#include "Types.h"

class ISession;

// IOCP ���� �� GQCS �Լ� ȣ��


class IocpService
{
    typedef void* HANDLE;

public:
    // IOCP ���� 
    IocpService();
    // IOCP �ڿ� ����
    ~IocpService();

    HANDLE GetIOCPHandle() { return iocpHandle; }

    bool RegisterSockToIOCP(std::shared_ptr<ISession> iocpObject);
    bool Dispatch(uint32 timeoutMs = UINF);

private:
    HANDLE iocpHandle;
};
