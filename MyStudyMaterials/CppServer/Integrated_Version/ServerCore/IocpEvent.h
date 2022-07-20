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
	// ��ӹ��� OVERLLAPED��ü�� ������ �ʱ�ȭ
	void			Init();

public:
	EventType		eventType;
	/* onwer pointer�� ����ִ� ����
	- GetQueuedCompletionStatus�Լ����� CompletionKey�� IocpObject�� ������ ���𰡸� �Ϸ��� �� ��
	  �ش� Session�� �Ҹ��ع��ȴٸ� �߸��� ������ �ϰ� �� ���̴�.
	  ���� ownerSession �����Ͱ� ��ȿ���� ������ �� Event�� ó���ϴ� ����� ä���� �������� ownerSession pointer ���.*/
	std::shared_ptr<ISession>	ownerSession;
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
	 
	//Vector<std::shared_ptr<SendBuffer>> sendBuffers;
	Vector<std::shared_ptr<SendBuffer>> sendBuffers;
};