#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"
#include "RecvBuffer.h"

class Service;



/*--------------
	Session
---------------*/

class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;
	friend class Service;
	
	enum
	{
		BUFFER_SIZE = 0x10000, // 64KB
	};

public:
	Session();
	virtual ~Session();

public:				/* �ܺο��� ��� */
	void						Send(SendBufferRef sendBuffer);
	bool						Connect();
	void						Disconnect(const WCHAR* cause);

	shared_ptr<Service>			GetService() { return service.lock(); }
	void						SetService(shared_ptr<Service> _service) { service = _service; }

public:				/* ���� ���� */
	void						SetNetAddress(NetAddress address) { netAddress = address; }
	NetAddress					GetNetAddress() { return netAddress; }
	SOCKET						GetSocket() { return socket; }
	SessionRef					GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }
	bool						IsConnected() { return isConnected; }

private:				/* �������̽� ���� */
						
	virtual HANDLE				GetHandle() override;
	virtual void				Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
						/* ���� ���� */
	bool						RegisterConnect();
	bool						RegisterDisconnect();
	void						RegisterRecv();
	void						RegisterSend();

	void						ProcessConnect();
	void						ProcessDisconnect();
	void						ProcessRecv(int32 numOfBytes);
	void						ProcessSend(int32 numOfBytes);

	void						HandleError(int32 errorCode);

protected:
						/* ������ �ڵ忡�� ������ */
	virtual void				OnConnected() { }
	virtual int32				OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void				OnSend(int32 len) { }
	virtual void				OnDisconnected() { }

private:
	weak_ptr<Service>			service;
	SOCKET						socket = INVALID_SOCKET;
	NetAddress					netAddress = {};
	Atomic<bool>				isConnected = false;

private:
	USE_LOCK;

								/* ���� ���� */
	RecvBuffer					recvBuffer;

								/* �۽� ���� */
	Queue<SendBufferRef>		sendQueue;
	Atomic<bool>				isSendRegistered  = false;

private:
								/* IocpEvent ���� */
	ConnectEvent				connectEvent;
	DisconnectEvent				disconnectEvent;
	RecvEvent					recvEvent;
	SendEvent					sendEvent;
};


/*-----------------
	PacketSession
------------------*/

struct PacketHeader
{
	uint16 size;
	uint16 id; // ��������ID (ex. 1=�α���, 2=�̵���û)
};

class PacketSession : public Session
{
public:
	PacketSession();
	virtual ~PacketSession();

	PacketSessionRef	GetPacketSessionRef() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	virtual int32		OnRecv(BYTE* buffer, int32 len) sealed;
	virtual void		OnRecvPacket(BYTE* buffer, int32 len) =0;
	
};