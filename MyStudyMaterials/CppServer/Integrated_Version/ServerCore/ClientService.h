#pragma once
#include <memory>
#include "Service.h"

class NetAddress;
class IocpService;
class ClientService : public Service
{
public:
    // _masSessionCount는 client 갯수
    ClientService(NetAddress _targetAddress, std::shared_ptr<IocpService> _core,
        SessionFactory _factory, int32 _maxSessionCount = 1);
    ~ClientService() override = default;

    virtual bool	Start() override;
};
