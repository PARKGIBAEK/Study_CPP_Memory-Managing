 #pragma once
#include "IocpService.h"
#include "NetAddress.h"

class AcceptEvent;
class ServerService;

/*--------------
	Listener
---------------*/

class Listener : public ISession
{
public:
	Listener() = default;
	~Listener();

	/* �������̽� ���� */
	virtual HANDLE GetHandle() override;// ���� ��ȯ
	virtual void DispatchEvent(class IocpEvent* _iocpEvent, int32 _numOfBytes = 0) override;

	/* �ܺο��� ��� */
	bool StartAccept(std::shared_ptr<ServerService>_service);
	void CloseSocket();

private:
	/* ���� ���� */
	void RegisterAccept(AcceptEvent* _acceptEvent); // AcceptEx �ɱ�
	void ProcessAccept(AcceptEvent* _acceptEvent);

protected:
	SOCKET listenSocket = INVALID_SOCKET;
	Vector<AcceptEvent*> acceptEvents;
	std::shared_ptr<ServerService> ownerService;
};

