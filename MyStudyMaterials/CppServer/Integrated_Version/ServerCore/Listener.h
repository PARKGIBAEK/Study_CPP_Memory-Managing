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

	/* 인터페이스 구현 */
	virtual HANDLE GetHandle() override;// 소켓 반환
	virtual void DispatchEvent(class IocpEvent* _iocpEvent, int32 _numOfBytes = 0) override;

	/* 외부에서 사용 */
	bool StartAccept(std::shared_ptr<ServerService>_service);
	void CloseSocket();

private:
	/* 수신 관련 */
	void RegisterAccept(AcceptEvent* _acceptEvent); // AcceptEx 걸기
	void ProcessAccept(AcceptEvent* _acceptEvent);

protected:
	SOCKET listenSocket = INVALID_SOCKET;
	Vector<AcceptEvent*> acceptEvents;
	std::shared_ptr<ServerService> ownerService;
};

