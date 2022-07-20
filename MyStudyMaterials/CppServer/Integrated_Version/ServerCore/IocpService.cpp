#include "pch.h"
#include "IocpService.h"
#include "IocpEvent.h"

/*--------------
	IocpService
---------------*/

IocpService::IocpService()
{
	// IOCP 생성
	iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(iocpHandle != INVALID_HANDLE_VALUE);
}

IocpService::~IocpService()
{
	::CloseHandle(iocpHandle);
}

bool IocpService::RegisterSockToIOCP(std::shared_ptr<ISession> _iocpObject)
{
	// GQCS에서 overlapped를 받아올 때 event
	return ::CreateIoCompletionPort(
		_iocpObject->GetHandle(), iocpHandle, /*key*/0, 0);
}

bool IocpService::Dispatch(uint32 _timeoutMs)
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;
	IocpEvent* iocpEvent = nullptr; // 각종 ~Event 클래스는 OVERLAPPED구조체를 상속 받음

	if (::GetQueuedCompletionStatus(
		iocpHandle, OUT &numOfBytes, OUT &key,
		OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), _timeoutMs))
	{
		std::shared_ptr<ISession> iocpObject = iocpEvent->ownerSession; // 모든 Event는 ownerIocpObject를 가짐
		iocpObject->DispatchEvent(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errCode = ::WSAGetLastError();
		switch (errCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			// TODO : 로그 찍기
			std::shared_ptr<ISession> iocpObject = iocpEvent->ownerSession;
			iocpObject->DispatchEvent(iocpEvent, numOfBytes);
			break;
		}
	}

	return true;
}