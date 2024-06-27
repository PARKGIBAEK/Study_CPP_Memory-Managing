#pragma once
#include "ISession.h"
#include <memory>
#include "Types.h"
#include "Container.h"
#include "NetAddress.h"
// #include <WinSock2.h>

class AcceptEvent;
class ServerService;
class IocpEvent;

/*--------------
	Listener
---------------*/

class Listener : public ISession
{
    typedef void* HANDLE;

public:
    Listener();
    ~Listener();

    /* �������̽� ���� */
    virtual HANDLE GetHandle() override; // ���� ��ȯ
    virtual void DispatchEvent(IocpEvent* _iocpEvent, int32 _numOfBytes = 0) override;

    /* �ܺο��� ��� */
    bool StartAccept(std::shared_ptr<ServerService> _service);
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
