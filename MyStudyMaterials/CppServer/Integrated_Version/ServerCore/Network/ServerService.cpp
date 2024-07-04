#include "ServerService.h"
#include "../Network/Listener.h"
#include "../Network/NetAddress.h"
#include "../Network/IocpService.h"
#include "../Memory/MemoryManager.h"


namespace ServerCore
{


ServerService::ServerService(NetAddress _address, std::shared_ptr<IocpService> _core,
                             SessionFactory _factory, int32 _maxSessionCount)
    : Service(ServiceType::Server, _address, _core,
              _factory, _maxSessionCount)
{
}

bool ServerService::Start()
{
    if (CanStart() == false)
        return false;

    listener = MakeShared<Listener>();
    if (listener == nullptr)
        return false;

    /* static_pointer_cast는 스마트 포인터 형변환 std::shared_ptr<Service>를 std::shared_ptr<ServerService>로 변환*/
    std::shared_ptr<ServerService> service =
        std::static_pointer_cast<ServerService>(shared_from_this());

    if (listener->StartAccept(service) == false)
        return false;

    return true;
}

void ServerService::CloseService()
{
    Service::CloseService();
}
}