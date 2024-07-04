#pragma once
#include "../Network/IDispatchable.h"
#include <memory>
#include "../Core/Types.h"
#include "../Memory/Container.h"
#include "../Network/NetAddress.h"
// #include <WinSock2.h>


namespace ServerCore
{
class AcceptEvent;
class ServerService;
class IocpEvent;


class Listener : public IDispatchable
{
    typedef void* HANDLE;

public:
    Listener();
    ~Listener() override;

    /* �������̽� ���� */
    virtual HANDLE GetHandle() override; // ���� ��ȯ
    virtual void DispatchEvent(IocpEvent* _iocpEvent, int32 _numOfBytes = 0) override;

    /* �ܺο��� ��� */
    bool StartAccept(std::shared_ptr<ServerService> _ownerService);
    void CloseSocket();

private:
    /* ���� ���� */
    void RegisterAccept(AcceptEvent* _acceptEvent); // AcceptEx �ɱ�
    void ProcessAccept(AcceptEvent* _acceptEvent);

protected:
    SOCKET listenSocket; // = INVALID_SOCKET;
    Vector<AcceptEvent*> acceptEvents;
    std::shared_ptr<ServerService> ownerService;
};
}