#pragma once
#include "NetAddress.h"
#include "IocpService.h"
#include "Listener.h"
#include <functional>

enum class ServiceType : uint8
{
	Server,
	Client
};

/*-------------
	Service
--------------*/

using SessionFactory = std::function< std::shared_ptr<Session>(void) >;

class Service : public enable_shared_from_this<Service>
{
public:

	Service(ServiceType _type, NetAddress _address, std::shared_ptr<IocpService> _iocpService,
		SessionFactory _factory, int32 _maxSessionCount = 1);
	virtual ~Service();

	virtual bool		Start() =0;
	bool				CanStart() { return sessionFactory != nullptr; }

	virtual void		CloseService();
	void				SetSessionFactory(SessionFactory _func) { sessionFactory = _func; }

	void				Broadcast(std::shared_ptr<SendBuffer> _sendBuffer);
	std::shared_ptr<Session>			CreateSession();
	void				AddSession(std::shared_ptr<Session> _session);
	void				ReleaseSession(std::shared_ptr<Session> _session);
	int32				GetCurrentSessionCount() { return sessionCount; }
	int32				GetMaxSessionCount() { return maxSessionCount; }

public:
	ServiceType			GetServiceType() { return type; }
	NetAddress			GetNetAddress() { return netAddress; }
	std::shared_ptr<IocpService>&		GetIocpCore() { return iocpService; }

protected:
	USE_LOCK;
	ServiceType			type;
	NetAddress			netAddress = {};
	std::shared_ptr<IocpService>			iocpService;

	Set<std::shared_ptr<Session>>		sessions;
	int32				sessionCount = 0;
	int32				maxSessionCount = 0;// listener 동접자 수
	SessionFactory		sessionFactory;
};

/*-----------------
	ClientService
------------------*/

class ClientService : public Service
{
public:
	// _masSessionCount는 client 갯수
	ClientService(NetAddress _targetAddress, std::shared_ptr<IocpService> _core,
		SessionFactory _factory, int32 _maxSessionCount = 1);
	virtual ~ClientService() {}

	virtual bool	Start() override;
};


/*-----------------
	ServerService
------------------*/

class ServerService : public Service
{
public:
	// _masSessionCount는 AcceptEx함수 동시 호출 유지 갯수
	ServerService(NetAddress _targetAddress, std::shared_ptr<IocpService> _core, 
		SessionFactory _factory, int32 _maxSessionCount = 1);
	virtual ~ServerService() {}

	virtual bool	Start() override;
	virtual void	CloseService() override;

private:
	std::shared_ptr<Listener>		listener = nullptr;
};