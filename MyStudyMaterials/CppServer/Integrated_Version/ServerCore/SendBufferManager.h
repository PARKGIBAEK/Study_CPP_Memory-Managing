#pragma once
#include <memory>

#include "Container.h"
#include "CoreMacro.h"
#include "Types.h"


/*---------------------
    SendBufferManager
----------------------*/

class SendBufferChunk;
class SendBuffer;
/*
    vector 컨테이너인 sendBufferChunks가 Pooling역할을 함.
    Pop시 sendBuffferChunks에 여유분 있으면 꺼내주고, 없으면 XNew로 새로 생성
*/
class SendBufferManager
{
public:
    std::shared_ptr<SendBuffer>				Open(uint32 _size);

private:
    std::shared_ptr<SendBufferChunk> 			Pop();
    void						Push(std::shared_ptr<SendBufferChunk>  _buffer);
    static void					PushGlobal(SendBufferChunk* _buffer);

private:
    USE_LOCK;
    Vector<std::shared_ptr<SendBufferChunk> >	sendBufferChunks;
};