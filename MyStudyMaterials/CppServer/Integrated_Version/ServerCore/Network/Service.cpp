#include "Service.h"
#include "../Network/SendBuffer.h"
#include "../Network/IocpService.h"
#include "../Network/Session.h"
#include "../Network/NetAddress.h"

namespace ServerCore
{
Service::Service(ServiceType _type, NetAddress _address, std::shared_ptr<IocpService> _iocpService,
                 SessionFactory _factory, int32 _maxSessionCount)
    : type(_type), netAddress(_address), iocpService(_iocpService), sessionCount(0), sessionFactory(_factory),
      maxSessionCount(_maxSessionCount)
{
}

Service::~Service()
{
}

bool Service::CanStart()
{
    return sessionFactory != nullptr;
}

void Service::CloseService()
{
    // TODO
}

void Service::SetSessionFactory(SessionFactory _func)
{
    sessionFactory = _func;
}

void Service::Broadcast(std::shared_ptr<SendBuffer> _sendBuffer)
{
    WRITE_LOCK;
    for (const auto& session : sessions)
    {
        session->Send(_sendBuffer);
    }
}

std::shared_ptr<Session> Service::CreateSession()
{
    std::shared_ptr<Session> session = sessionFactory();
    session->SetService(shared_from_this());

    if (iocpService->RegisterSocketToIocp(session) == false)
        return nullptr;

    return session;
}

void Service::AddSession(std::shared_ptr<Session> _session)
{
    WRITE_LOCK;
    sessionCount++;
    sessions.insert(_session);
}

void Service::ReleaseSession(std::shared_ptr<Session> _session)
{
    WRITE_LOCK;
    // set::erase()는 삭제된 원소 갯수를 반환
    ASSERT_CRASH(sessions.erase(_session) != 0);
    sessionCount--;
}

int32 Service::GetCurrentSessionCount()
{
    return sessionCount;
}

int32 Service::GetMaxSessionCount()
{
    return maxSessionCount;
}

ServiceType Service::GetServiceType()
{
    return type;
}

NetAddress Service::GetNetAddress()
{
    return netAddress;
}

std::shared_ptr<IocpService>& Service::GetIocpCore()
{
    return iocpService;
}
}