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

	/* 외부에서 사용 */
	bool StartAccept(ServerServiceRef _service);
	void CloseSocket();

	/* 인터페이스 구현 */
	virtual HANDLE GetHandle() override;// 소켓 반환
	virtual void Dispatch(class IocpEvent* _iocpEvent, int32 _numOfBytes = 0) override;

private:
	/* 수신 관련 */
	void RegisterAccept(AcceptEvent* _acceptEvent);
	void ProcessAccept(AcceptEvent* _acceptEvent);

protected:
	SOCKET listenSocket = INVALID_SOCKET;
	Vector<AcceptEvent*> acceptEvents;
	ServerServiceRef ownerService;
};

