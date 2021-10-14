#include "pch.h"
#include "SendBuffer.h"

/*----------------
	SendBuffer
-----------------*/

SendBuffer::SendBuffer(SendBufferChunkRef _owner, BYTE* _buffer, uint32 _allocSize)
	: owner(_owner), buffer(_buffer), allocSize(_allocSize)
{
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::Close(uint32 _writeSize)
{
	ASSERT_CRASH(allocSize >= _writeSize);
	_writeSize = _writeSize;
	owner->Close(_writeSize);
}

/*--------------------
	SendBufferChunk
--------------------*/

SendBufferChunk::SendBufferChunk()
{
}

SendBufferChunk::~SendBufferChunk()
{
}

void SendBufferChunk::Reset()
{
	isOpen = false;
	usedSize = 0;
}

SendBufferRef SendBufferChunk::Open(uint32 _allocSize)
{
	ASSERT_CRASH(_allocSize <= SEND_BUFFER_CHUNK_SIZE);
	ASSERT_CRASH(isOpen == false);

	if (_allocSize > FreeSize())
		return nullptr;

	isOpen = true;
	return ObjectPool<SendBuffer>::MakeShared(shared_from_this(), Buffer(), _allocSize);
}

void SendBufferChunk::Close(uint32 _writeSize)
{
	ASSERT_CRASH(isOpen == true);
	isOpen = false;
	usedSize += _writeSize;
}

/*--------------------------------------
				SendBufferManager
--------------------------------------*/

SendBufferRef SendBufferManager::Open(uint32 _size)
{
	if (LSendBufferChunk == nullptr)
	{
		LSendBufferChunk = SendBufferManager::Pop(); // WRITE_LOCK
		LSendBufferChunk->Reset();
	}		

	ASSERT_CRASH(LSendBufferChunk->IsOpen() == false);

	// 다 썼으면 버리고 새거로 교체
	if (LSendBufferChunk->FreeSize() < _size)
	{
		LSendBufferChunk = SendBufferManager::Pop(); // WRITE_LOCK
		LSendBufferChunk->Reset();
	}

	return LSendBufferChunk->Open(_size);
}

SendBufferChunkRef SendBufferManager::Pop()
{
	{
		WRITE_LOCK;
		if (sendBufferChunks.empty() == false)
		{
			SendBufferChunkRef sendBufferChunk = sendBufferChunks.back();
			sendBufferChunks.pop_back();
			return sendBufferChunk;
		}
	}

	return SendBufferChunkRef(XNew<SendBufferChunk>(), PushGlobal);
}

void SendBufferManager::Push(SendBufferChunkRef _buffer)
{
	WRITE_LOCK;
	sendBufferChunks.push_back(_buffer);
}

void SendBufferManager::PushGlobal(SendBufferChunk* _buffer)
{
	cout << "PushGlobal SENDBUFFERCHUNK" << endl;

	GSendBufferManager->Push(SendBufferChunkRef(_buffer, PushGlobal));
}