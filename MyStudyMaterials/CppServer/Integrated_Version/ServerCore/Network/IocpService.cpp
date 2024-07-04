#include "IocpService.h"
#include <ws2tcpip.h>
#include <winsock2.h>
#include "../Core/CoreMacro.h"
#include "../Network/IocpEvent.h"
#include "../Network/IDispatchable.h"

namespace ServerCore
{
IocpService::IocpService()
{
	// IOCP ����
	iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(iocpHandle != INVALID_HANDLE_VALUE);
}

IocpService::~IocpService()
{
	::CloseHandle(iocpHandle);
}

bool IocpService::RegisterSocketToIocp(std::shared_ptr<IDispatchable> _iocpObject)
{
	// GQCS���� overlapped�� �޾ƿ� �� event
	return ::CreateIoCompletionPort(
		_iocpObject->GetHandle(), iocpHandle, /*key*/0, 0);
}

bool IocpService::Dispatch(uint32 _timeoutMs)
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;
	IocpEvent* iocpEvent = nullptr; // ���� ~Event Ŭ������ OVERLAPPED����ü�� ��� ����

	if (::GetQueuedCompletionStatus(
		iocpHandle, OUT &numOfBytes, OUT &key,
		OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), _timeoutMs))
	{
		std::shared_ptr<IDispatchable> dispatcher = iocpEvent->ownerDispatcher; // ��� Event�� ownerIocpObject�� ����
		dispatcher->DispatchEvent(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errCode = ::WSAGetLastError();
		switch (errCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			// TODO : �α� ���
				std::shared_ptr<IDispatchable> dispatcher = iocpEvent->ownerDispatcher;
			dispatcher->DispatchEvent(iocpEvent, numOfBytes);
			break;
		}
	}

	return true;
}
}