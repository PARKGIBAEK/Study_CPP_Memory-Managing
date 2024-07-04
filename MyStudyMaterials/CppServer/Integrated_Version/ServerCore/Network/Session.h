#pragma once
#include <memory>
#include <atomic>
#include "../Memory/Container.h"
#include "../Core/CoreMacro.h"
#include "../Network/IocpEvent.h"
#include "../Network/IDispatchable.h"
#include "../Network/NetAddress.h"
#include "../Network/RecvBuffer.h"

namespace ServerCore
{
class Service;
class SendBuffer;

class Session : public IDispatchable
{
    using HANDLE = void*;
    // typedef void *HANDLE;
    
    friend class Listener;
    friend class IocpService;
    friend class Service;

    enum
    {
        BUFFER_SIZE = 0x10000, // 64KB
    };

public:
    Session();
    virtual ~Session();

public: /* 외부에서 사용 */
    void Send(std::shared_ptr<SendBuffer> sendBuffer);
    bool Connect();
    void Disconnect(const WCHAR* cause);

    std::shared_ptr<Service> GetService();
    void SetService(std::shared_ptr<Service> _service);

public: /* 정보 관련 */
    void SetNetAddress(NetAddress address);
    NetAddress GetNetAddress() const;
    SOCKET GetSocket() const;
    std::shared_ptr<Session> GetSessionRef();
    bool IsConnected();

private: /* 인터페이스 구현 */

    virtual HANDLE GetHandle() override;
    virtual void DispatchEvent(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
    /* 전송 관련 */
    bool RegisterConnect();
    bool RegisterDisconnect();
    void RegisterRecv();
    void RegisterSend(); // Gathering write

    void ProcessConnect();
    void ProcessDisconnect();
    void ProcessRecv(int32 numOfBytes);
    void ProcessSend(int32 numOfBytes);

    void HandleError(int32 errorCode);

protected:
    /* 컨텐츠 코드에서 재정의 */
    virtual void OnConnected();
    virtual int32 OnRecv(BYTE* buffer, int32 len);

    virtual void OnSend(int32 len);

    virtual void OnDisconnected();

    virtual void PrintAddress();

private:
    std::weak_ptr<Service> service;
    SOCKET socket;
    NetAddress netAddress;
    std::atomic<bool> isConnected;

private:
    USE_LOCK;

    /* 수신 관련 */
    RecvBuffer recvBuffer;

    /* 송신 관련 */
    Queue<std::shared_ptr<SendBuffer>> sendQueue;
    std::atomic<bool> isSendRegistered;

private:
    /* IocpEvent 재사용 */
    ConnectEvent connectEvent;
    DisconnectEvent disconnectEvent;
    RecvEvent recvEvent;
    SendEvent sendEvent;
};
}