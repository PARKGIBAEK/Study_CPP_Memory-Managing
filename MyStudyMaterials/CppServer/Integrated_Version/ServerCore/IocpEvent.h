#pragma once
#include <memory>
#include "Container.h"
#include "Core.h"
#include <WS2tcpip.h>

class ISession;
class SendBuffer;

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
	  따라서 ownerSession 포인터가 유효한지 검증한 뒤 Event를 처리하는 방식을 채용할 목적으로 ownerSession pointer 사용.*/
	std::shared_ptr<ISession>	ownerSession;
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