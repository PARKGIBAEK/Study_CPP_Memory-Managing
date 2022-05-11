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

public:				/* 외부에서 사용 */
	void						Send(SendBufferRef sendBuffer);
	bool						Connect();
	void						Disconnect(const WCHAR* cause);

	shared_ptr<Service>			GetService() { return service.lock(); }
	void						SetService(shared_ptr<Service> _service) { service = _service; }

public:				/* 정보 관련 */
	void						SetNetAddress(NetAddress address) { netAddress = address; }
	NetAddress					GetNetAddress() { return netAddress; }
	SOCKET						GetSocket() { return socket; }
	SessionRef					GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }
	bool						IsConnected() { return isConnected; }

private:				/* 인터페이스 구현 */
						
	virtual HANDLE				GetHandle() override;
	virtual void				Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
						/* 전송 관련 */
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
						/* 컨텐츠 코드에서 재정의 */
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

								/* 수신 관련 */
	RecvBuffer					recvBuffer;

								/* 송신 관련 */
	Queue<SendBufferRef>		sendQueue;
	Atomic<bool>				isSendRegistered  = false;

private:
								/* IocpEvent 재사용 */
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
	uint16 id; // 프로토콜ID (ex. 1=로그인, 2=이동요청)
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