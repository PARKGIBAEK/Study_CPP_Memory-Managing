#include "SendBuffer.h"

#include "../Core/CoreMacro.h"
#include "../Network/SendBufferChunk.h"


namespace ServerCore
{
SendBuffer::SendBuffer(std::shared_ptr<SendBufferChunk>  _owner, BYTE* _buffer, uint32 _allocSize)
	: buffer(_buffer), allocSize(_allocSize), writeSize(0), owner(_owner)
{
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::Close(uint32 _writeSize)
{
	ASSERT_CRASH(allocSize >= _writeSize);
	writeSize = _writeSize;
	owner->Close(_writeSize);
}
}