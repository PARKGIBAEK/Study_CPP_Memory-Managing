#pragma once

/*----------------
	IocpObject
-----------------*/

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
	IocpCore();
	~IocpCore();

	HANDLE		GetHandle() { return iocpHandle; }

	bool		Register(IocpObjectRef iocpObject);
	bool		Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE		iocpHandle;
};