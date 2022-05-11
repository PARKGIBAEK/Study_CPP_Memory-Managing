#pragma once
#include "Container.h"
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

/*--------------
	IocpEvent
---------------*/

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
	  따라서 owner 포인터가 유효한지 검증한 뒤 Event를 처리하는 방식을 채용할 목적으로 owner pointer 사용.*/
	IocpObjectRef	owner;
};

/*----------------
	ConnectEvent
-----------------*/

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect) { }
};


/*--------------------
	DisconnectEvent
----------------------*/

class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() : IocpEvent(EventType::Disconnect) { }
};


/*----------------
	AcceptEvent
-----------------*/

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) { }

public:
	SessionRef	ownerSession = nullptr;
};


/*----------------
	RecvEvent
-----------------*/

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) { }
};


/*----------------
	SendEvent
-----------------*/

class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) { }
	 
	//Vector<SendBufferRef> sendBuffers;
	Vector<SendBufferRef> sendBuffers;
};