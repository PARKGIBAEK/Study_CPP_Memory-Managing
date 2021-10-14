#pragma once

/*----------------
	IocpObject
-----------------*/

class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* _iocpEvent, int32 _numOfBytes = 0) abstract;
};

/*--------------
	IocpCore
---------------*/

// IOCP 생성 및 GQCS 함수 호출
class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE		GetHandle() { return iocpHandle; }

	bool		Register(IocpObjectRef iocpObject);
	bool		Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE		iocpHandle;
};