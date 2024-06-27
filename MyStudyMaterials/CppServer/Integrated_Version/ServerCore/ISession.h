#pragma once
#include <memory>
#include "Types.h"

class IocpEvent;

/* Every class carrying out 'Overlapped I/O', like Listener or Session, inherit this abstract class.
The function 'DispatchEvent()' processes the event corresponding to the purpose of its concrete class.*/
class ISession : public std::enable_shared_from_this<ISession>
{
    typedef void *HANDLE;
public:
    virtual HANDLE GetHandle() =0;
    virtual void DispatchEvent(IocpEvent* _iocpEvent, int32 _numOfBytes = 0) =0;
};