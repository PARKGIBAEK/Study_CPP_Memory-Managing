#pragma once
#include <memory>
#include "../Core/Types.h"

namespace ServerCore
{

class SendBufferChunk;

class SendBuffer
{
public:
	SendBuffer(std::shared_ptr<SendBufferChunk> _owner, BYTE* _buffer, uint32 _allocSize);
	~SendBuffer();

	BYTE*				Buffer() { return buffer; }
	uint32				AllocSize() { return allocSize; }
	uint32				WriteSize() { return writeSize; }
	void				Close(uint32 _writeSize);

private:
	BYTE*				buffer;
	uint32				allocSize;
	uint32				writeSize;
	std::shared_ptr<SendBufferChunk>	owner;
};
}