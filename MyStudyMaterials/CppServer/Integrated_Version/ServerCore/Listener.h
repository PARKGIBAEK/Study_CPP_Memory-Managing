 #pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class AcceptEvent;
class ServerService;

/*--------------
	Listener
---------------*/

class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

	/* �ܺο��� ��� */
	bool StartAccept(ServerServiceRef _service);
	void CloseSocket();

	/* �������̽� ���� */
	virtual HANDLE GetHandle() override;// ���� ��ȯ
	virtual void Dispatch(class IocpEvent* _iocpEvent, int32 _numOfBytes = 0) override;

private:
	/* ���� ���� */
	void RegisterAccept(AcceptEvent* _acceptEvent);
	void ProcessAccept(AcceptEvent* _acceptEvent);

protected:
	SOCKET listenSocket = INVALID_SOCKET;
	Vector<AcceptEvent*> acceptEvents;
	ServerServiceRef ownerService;
};

