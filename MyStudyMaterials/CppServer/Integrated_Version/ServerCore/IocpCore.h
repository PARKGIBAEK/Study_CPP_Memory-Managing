#pragma once

/*----------------
	IocpObject
-----------------*/

/* Every class carrying out 'Overlapped I/O', like Listener or Session, inherit this abstract class.
The function 'Dispatch()' processes the event corresponding to the purpose of its concrete class.*/
class IocpObject : public std::enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() =0;
	virtual void Dispatch(class IocpEvent* _iocpEvent, int32 _numOfBytes = 0) =0;
};

/*--------------
	IocpCore
---------------*/

// IOCP ���� �� GQCS �Լ� ȣ��
class IocpCore
{
public:
	// IOCP ���� 
	IocpCore();
	// IOCP �ڿ� ����
	~IocpCore();

	HANDLE		GetIOCPHandle() { return iocpHandle; }

	bool		RegisterSockToIOCP(IocpObjectRef iocpObject);
	bool		Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE		iocpHandle;
};