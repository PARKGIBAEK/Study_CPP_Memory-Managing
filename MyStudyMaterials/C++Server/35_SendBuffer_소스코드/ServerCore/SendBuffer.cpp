#include "pch.h"
#include "SendBuffer.h"

/*----------------
	SendBuffer
-----------------*/

SendBuffer::SendBuffer(int32 bufferSize)
{
	_buffer.resize(bufferSize);
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::CopyData(void* data, int32 len)
{
	ASSERT_CRASH(Capacity() >= len);//버퍼 사이즈 체크
	::memcpy(_buffer.data(), data, len);//데이터를 버퍼로 복사
	_writeSize = len;//버퍼에 데이터 길이 기록
}