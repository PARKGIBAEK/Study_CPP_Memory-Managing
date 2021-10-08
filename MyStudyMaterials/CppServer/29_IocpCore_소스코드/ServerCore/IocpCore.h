#pragma once

/*----------------
	IocpObject
-----------------*/

class IocpObject
{
public:
	virtual HANDLE GetHandle() abstract;
	// IOCP에 일감이 있는지 모니터링(GQCS 함수 호출)
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};

/*--------------
	IocpCore
---------------*/

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE		GetHandle() { return _iocpHandle; }

	bool		Register(class IocpObject* iocpObject);
	// Worker Thread들이 IOCP에 일감이 들어오는지 모니터링
	bool		Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE		_iocpHandle;
};

// TEMP
extern IocpCore GIocpCore;