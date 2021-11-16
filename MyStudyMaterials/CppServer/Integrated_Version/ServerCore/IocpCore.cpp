#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

/*--------------
	IocpCore
---------------*/

IocpCore::IocpCore()
{
	// IOCP 생성
	iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(iocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
	::CloseHandle(iocpHandle);
}

bool IocpCore::Register(IocpObjectRef _iocpObject)
{
	return ::CreateIoCompletionPort(_iocpObject->GetHandle(), iocpHandle, /*key*/0, 0);
}

bool IocpCore::Dispatch(uint32 _timeoutMs)
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;	
	IocpEvent* iocpEvent = nullptr;

	if (::GetQueuedCompletionStatus(iocpHandle, OUT &numOfBytes, OUT &key, OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), _timeoutMs))
	{
		IocpObjectRef iocpObject = iocpEvent->owner;
		iocpObject->Dispatch(iocpEvent, numOfBytes);
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
			IocpObjectRef iocpObject = iocpEvent->owner;
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

	return true;
}
