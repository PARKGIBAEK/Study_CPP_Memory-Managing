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

    /* 인터페이스 구현 */
    virtual HANDLE GetHandle() override; // 소켓 반환
    virtual void DispatchEvent(IocpEvent* _iocpEvent, int32 _numOfBytes = 0) override;

    /* 외부에서 사용 */
    bool StartAccept(std::shared_ptr<ServerService> _ownerService);
    void CloseSocket();

private:
    /* 수신 관련 */
    void RegisterAccept(AcceptEvent* _acceptEvent); // AcceptEx 걸기
    void ProcessAccept(AcceptEvent* _acceptEvent);

protected:
    SOCKET listenSocket; // = INVALID_SOCKET;
    Vector<AcceptEvent*> acceptEvents;
    std::shared_ptr<ServerService> ownerService;
};
}