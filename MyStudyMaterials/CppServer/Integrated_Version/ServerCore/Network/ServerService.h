#pragma once
#include <memory>
#include "../Core/Types.h"
#include "../Network/Service.h"



namespace ServerCore
{
class Listener;
class NetAddress;
class IocpService;

class ServerService : public Service
{
public:
    // _masSessionCount는 AcceptEx함수 동시 호출 유지 갯수
    ServerService(NetAddress _targetAddress, std::shared_ptr<IocpService> _core,
                  SessionFactory _factory, int32 _maxSessionCount = 1);

    virtual ~ServerService()
    {
    }

    virtual bool Start() override;
    virtual void CloseService() override;

private:
    std::shared_ptr<Listener> listener = nullptr;
};
}
