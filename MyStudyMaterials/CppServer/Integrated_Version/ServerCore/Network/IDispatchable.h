#pragma once
#include <memory>
#include "../Core/Types.h"

namespace ServerCore
{
class IocpEvent;

/* Every class carrying out 'Overlapped I/O', like Listener or Session, inherit this abstract class.
The function 'DispatchEvent()' processes the event corresponding to the purpose of its concrete class.*/
class IDispatchable : public std::enable_shared_from_this<IDispatchable>
{
	typedef void* HANDLE;
public:
	virtual ~IDispatchable() = default;
	virtual HANDLE GetHandle() = 0;
	virtual void DispatchEvent(IocpEvent* _iocpEvent, int32 _numOfBytes = 0) = 0;
};
}
//inline ISession::~ISession() {} // 순수 가상 소멸자의 구현