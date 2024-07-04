#include "ClientService.h"
#include "../Network/NetAddress.h"
#include "../Network/IocpService.h"
#include "../Network/Session.h"


namespace ServerCore
{
ClientService::ClientService(NetAddress _targetAddress, std::shared_ptr<IocpService> _core, SessionFactory _factory,
                             int32 _maxSessionCount)
    : Service(ServiceType::Client, _targetAddress, _core, _factory,
              _maxSessionCount)
{
}

bool ClientService::Start()
{
    if (CanStart() == false)
        return false;

    const int32 sessionCount = GetMaxSessionCount();
    for (int32 i = 0; i < sessionCount; i++)
    {
        std::shared_ptr<Session> session = CreateSession();
        if (session->Connect() == false)
            return false;
        sessions.emplace_back(session);
    }

    return true;
}
}