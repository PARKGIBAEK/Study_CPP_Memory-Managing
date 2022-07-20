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
	writeSize = _writeSize;
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

void SendBufferChunk::ResetBuffer()
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

	return ObjectPool<SendBuffer>::MakeShared(shared_from_this(), Buffer(),  _allocSize);
}

void SendBufferChunk::Close(uint32 _writeSize)
{
	ASSERT_CRASH(isOpen == true);
	isOpen = false;
	usedSize += _writeSize;
}


/*-----------------------------------------------
				SendBufferManager
-----------------------------------------------*/

SendBufferRef SendBufferManager::Open(uint32 _reqSize)
{
	if (tls_SendBufferChunk == nullptr)
	{
		tls_SendBufferChunk = SendBufferManager::Pop(); // WRITE_LOCK
		tls_SendBufferChunk->ResetBuffer();
	}		

	ASSERT_CRASH(tls_SendBufferChunk->IsOpen() == false);

	// 다 썼으면 버리고 새걸 교체
	if (tls_SendBufferChunk->FreeSize() < _reqSize)
	{
		tls_SendBufferChunk = SendBufferManager::Pop(); // WRITE_LOCK
		tls_SendBufferChunk->ResetBuffer();
	}

	return tls_SendBufferChunk->Open(_reqSize);
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

	/*PushGlobal은 shared_ptr의 소멸자
	- SendBufferChunk를 SendBufferManager::sendBufferChunks에 다시 반환(push_back)*/
	return SendBufferChunkRef(XNew<SendBufferChunk>(), PushGlobal); 
}

void SendBufferManager::Push(SendBufferChunkRef _buffer)
{
	WRITE_LOCK;
	sendBufferChunks.emplace_back(_buffer);
}

void SendBufferManager::PushGlobal(SendBufferChunk* _buffer)
{
	//cout << "Log : PushGlobal SendBufferChunkRef" << endl;
	GSendBufferManager->Push(SendBufferChunkRef(_buffer, PushGlobal));
}
