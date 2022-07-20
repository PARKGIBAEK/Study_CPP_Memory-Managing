#include "pch.h"
#include "Service.h"
#include "Session.h"
#include "Listener.h"

/*-------------
	Service
--------------*/

Service::Service(ServiceType _type, NetAddress _address, std::shared_ptr<IocpService> _iocpService, SessionFactory _factory, int32 _maxSessionCount)
	: type(_type), netAddress(_address), iocpService(_iocpService), sessionFactory(_factory), maxSessionCount(_maxSessionCount)
{

}

Service::~Service()
{

}

void Service::CloseService()
{
	// TODO
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

	if (iocpService->RegisterSockToIOCP(session) == false)
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
	// set::erase()�� ������ ���� ������ ��ȯ
	ASSERT_CRASH(sessions.erase(_session) != 0);
	sessionCount--;
}

/*-----------------
	ClientService
------------------*/

ClientService::ClientService(NetAddress _targetAddress, std::shared_ptr<IocpService> _core, SessionFactory _factory, int32  _maxSessionCount)
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
	}

	return true;
}

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

	/* static_pointer_cast�� ����Ʈ ������ ����ȯ std::shared_ptr<Service>�� std::shared_ptr<ServerService>�� ��ȯ*/
	std::shared_ptr<ServerService>service = 
		static_pointer_cast<ServerService>(shared_from_this());
	
	if (listener->StartAccept(service) == false)
		return false;

	return true;
}

void ServerService::CloseService()
{
	// TODO

	Service::CloseService();
}
