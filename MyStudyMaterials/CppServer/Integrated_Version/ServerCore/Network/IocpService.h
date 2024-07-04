#pragma once
#include <memory>
#include "../Core/Types.h"

namespace ServerCore
{
class IDispatchable;

// IOCP ���� �� GQCS �Լ� ȣ��


class IocpService
{
    typedef void* HANDLE;

public:
    // IOCP ���� 
    IocpService();
    // IOCP �ڿ� ����
    ~IocpService();

    HANDLE GetIOCPHandle() const { return iocpHandle; }

    bool RegisterSocketToIocp(std::shared_ptr<IDispatchable> _iocpObject);
    bool Dispatch(uint32 _timeoutMs = UINF);

private:
    HANDLE iocpHandle;
};
}
