#pragma once
#include <memory>
#include "../Memory/Container.h"
#include "../Core/Core.h"
#include <WS2tcpip.h>


namespace ServerCore
{
class IDispatchable;
class SendBuffer;
class Session;

enum class EventType : uint8
{
	Connect,
	Disconnect,
	Accept,
	//PreRecv,
	Recv,
	Send
};

class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType _type);
	// 상속받은 OVERLLAPED객체의 내용을 초기화
	void			Init();

public:
	EventType		eventType;
	/* onwer pointer를 들고있는 이유
	- GetQueuedCompletionStatus함수에서 CompletionKey로 IocpObject를 꺼내서 무언가를 하려고 할 때
	  해당 Session이 소멸해버렸다면 잘못된 동작을 하게 될 것이다.
	  따라서 ownerDispatcher 포인터가 유효한지 검증한 뒤 Event를 처리하는 방식을 채용할 목적으로 ownerDispatcher pointer 사용.*/
	std::shared_ptr<IDispatchable>	ownerDispatcher;
};

/*----------------
	ConnectEvent
-----------------*/

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent();
};


/*--------------------
	DisconnectEvent
----------------------*/

class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent();
};


/*----------------
	AcceptEvent
-----------------*/

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent();
	std::shared_ptr<Session> session;
};


/*----------------
	RecvEvent
-----------------*/

class RecvEvent : public IocpEvent
{
public:
	RecvEvent();
};


/*----------------
	SendEvent
-----------------*/

class SendEvent : public IocpEvent
{
public:
	SendEvent();
	 
	//Vector<std::shared_ptr<SendBuffer>> sendBuffers;
	Vector<std::shared_ptr<SendBuffer>> sendBuffers;
};
}