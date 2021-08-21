#pragma once

/*----------------
	IocpObject
-----------------*/

class IocpObject
{
public:
	virtual HANDLE GetHandle() abstract;
	// IOCP�� �ϰ��� �ִ��� ����͸�(GQCS �Լ� ȣ��)
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
	// Worker Thread���� IOCP�� �ϰ��� �������� ����͸�
	bool		Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE		_iocpHandle;
};

// TEMP
extern IocpCore GIocpCore;