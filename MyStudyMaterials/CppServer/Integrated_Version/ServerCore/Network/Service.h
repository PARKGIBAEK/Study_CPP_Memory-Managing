#pragma once
#include <memory>
#include <functional>
#include "../Memory/Container.h"
#include "../Core/CoreMacro.h"
#include "../Network/NetAddress.h"

namespace ServerCore
{
class SendBuffer;
class IocpService;
class Session;

enum class ServiceType : uint8
{
	Server,
	Client
};

using SessionFactory = std::function< std::shared_ptr<Session>(void) >;

class Service : public std::enable_shared_from_this<Service>
{
public:

	Service(ServiceType _type, NetAddress _address, std::shared_ptr<IocpService> _iocpService,
		SessionFactory _factory, int32 _maxSessionCount = 1);
	virtual ~Service();

	virtual bool		Start() =0;
	bool				CanStart();

	virtual void		CloseService();
	void				SetSessionFactory(SessionFactory _func);

	void				Broadcast(std::shared_ptr<SendBuffer> _sendBuffer);
	std::shared_ptr<Session>			CreateSession();
	void				AddSession(std::shared_ptr<Session> _session);
	void				ReleaseSession(std::shared_ptr<Session> _session);
	int32				GetCurrentSessionCount();
	int32				GetMaxSessionCount();

public:
	ServiceType			GetServiceType();
	NetAddress			GetNetAddress();
	std::shared_ptr<IocpService>&		GetIocpCore();

protected:
	USE_LOCK;
	ServiceType			type;
	NetAddress			netAddress;
	std::shared_ptr<IocpService>			iocpService;

	Set<std::shared_ptr<Session>>		sessions;
	int32				sessionCount;
	int32				maxSessionCount;// listener 동접자 수
	SessionFactory		sessionFactory;
};
}