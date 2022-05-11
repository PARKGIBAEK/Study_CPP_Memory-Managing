#pragma once
#include "NetAddress.h"
#include "IocpCore.h"
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

using SessionFactory = std::function<SessionRef(void)>;

class Service : public enable_shared_from_this<Service>
{
public:
	Service(ServiceType _type, NetAddress _address, IocpCoreRef _core, 
		SessionFactory _factory, int32 _maxSessionCount = 1);
	virtual ~Service();

	virtual bool		Start() =0;
	bool				CanStart() { return sessionFactory != nullptr; }

	virtual void		CloseService();
	void				SetSessionFactory(SessionFactory _func) { sessionFactory = _func; }

	void				Broadcast(SendBufferRef _sendBuffer);
	SessionRef			CreateSession();
	void				AddSession(SessionRef _session);
	void				ReleaseSession(SessionRef _session);
	int32				GetCurrentSessionCount() { return sessionCount; }
	int32				GetMaxSessionCount() { return maxSessionCount; }

public:
	ServiceType			GetServiceType() { return type; }
	NetAddress			GetNetAddress() { return netAddress; }
	IocpCoreRef&		GetIocpCore() { return iocpCore; }

protected:
	USE_LOCK;
	ServiceType			type;
	NetAddress			netAddress = {};
	IocpCoreRef			iocpCore;

	Set<SessionRef>		sessions;
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
	ClientService(NetAddress _targetAddress, IocpCoreRef _core,
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
	ServerService(NetAddress _targetAddress, IocpCoreRef _core, 
		SessionFactory _factory, int32 _maxSessionCount = 1);
	virtual ~ServerService() {}

	virtual bool	Start() override;
	virtual void	CloseService() override;

private:
	ListenerRef		listener = nullptr;
};