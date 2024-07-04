#include "IocpEvent.h"
#include "../Network/IDispatchable.h"
#include "../Network/SendBuffer.h"


namespace ServerCore
{
IocpEvent::IocpEvent(EventType _type) : eventType(_type)
{
    Init();
}

void IocpEvent::Init()
{
    OVERLAPPED::hEvent = 0;
    OVERLAPPED::Internal = 0;
    OVERLAPPED::InternalHigh = 0;
    OVERLAPPED::Offset = 0;
    OVERLAPPED::OffsetHigh = 0;
}

ConnectEvent::ConnectEvent()
    : IocpEvent(EventType::Connect)
{
}

DisconnectEvent::DisconnectEvent()
    : IocpEvent(EventType::Disconnect)
{
}

AcceptEvent::AcceptEvent()
    : IocpEvent(EventType::Accept), session(nullptr)
{
}

RecvEvent::RecvEvent()
    : IocpEvent(EventType::Recv)
{
}

SendEvent::SendEvent()
    : IocpEvent(EventType::Send)
{
}
}
