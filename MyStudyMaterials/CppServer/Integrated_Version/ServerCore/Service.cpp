#include "pch.h"
#include "Service.h"
#include "Session.h"
#include "Listener.h"

/*-------------
	Service
--------------*/

Service::Service(ServiceType _type, NetAddress _address, IocpCoreRef _core, SessionFactory _factory, int32 _maxSessionCount)
	: type(_type), netAddress(_address), iocpCore(_core), sessionFactory(_factory), maxSessionCount(_maxSessionCount)
{

}

Service::~Service()
{
}

void Service::CloseService()
{
	// TODO
}

void Service::Broadcast(SendBufferRef _sendBuffer)
{
	WRITE_LOCK;
	for (const auto& session : sessions)
	{
		session->Send(_sendBuffer);
	}
}

SessionRef Service::CreateSession()
{
	SessionRef session = sessionFactory();
	session->SetService(shared_from_this());

	if (iocpCore->RegisterSockToIOCP(session) == false)
		return nullptr;

	return session;
}

void Service::AddSession(SessionRef _session)
{
	WRITE_LOCK;
	sessionCount++;
	sessions.insert(_session);
}

void Service::ReleaseSession(SessionRef _session)
{
	WRITE_LOCK;
	// set::erase()는 삭제된 원소 갯수를 반환
	ASSERT_CRASH(sessions.erase(_session) != 0);
	sessionCount--;
}

/*-----------------
	ClientService
------------------*/

ClientService::ClientService(NetAddress _targetAddress, IocpCoreRef _core, SessionFactory _factory, int32  _maxSessionCount)
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
		SessionRef session = CreateSession();
		if (session->Connect() == false)
			return false;
	}

	return true;
}

ServerService::ServerService(NetAddress _address, IocpCoreRef _core,
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
	ServerServiceRef service = 
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
