#pragma once

/*----------------
	ISession
-----------------*/

/* Every class carrying out 'Overlapped I/O', like Listener or Session, inherit this abstract class.
The function 'DispatchEvent()' processes the event corresponding to the purpose of its concrete class.*/
class ISession : public std::enable_shared_from_this<ISession>
{
public:
	virtual HANDLE GetHandle() =0;
	virtual void DispatchEvent(class IocpEvent* _iocpEvent, int32 _numOfBytes = 0) =0;
};

/*--------------
	IocpService
---------------*/

// IOCP 생성 및 GQCS 함수 호출
class IocpService
{
public:
	// IOCP 생성 
	IocpService();
	// IOCP 자원 해제
	~IocpService();

	HANDLE		GetIOCPHandle() { return iocpHandle; }
	
	bool		RegisterSockToIOCP(std::shared_ptr<ISession> iocpObject);
	bool		Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE		iocpHandle;
};