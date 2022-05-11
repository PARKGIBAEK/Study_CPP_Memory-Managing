#include "pch.h"
#include "RecvBuffer.h"
//#include <iterator> // 필요 없어보여서 제거 함, 추후 확인


/*--------------
	RecvBuffer
----------------*/

RecvBuffer::RecvBuffer(int32 _bufferSize) : bufferSize(_bufferSize)
{
	capacity = _bufferSize * BUFFER_COUNT;
	buffer.resize(capacity);
}

RecvBuffer::~RecvBuffer()
{
}

void RecvBuffer::Clean()
{
	int32 dataSize = DataSize();
	
	if (dataSize == 0)
	{
		// 딱 마침 읽기+쓰기 커서가 동일한 위치라면, 둘 다 리셋.
		readCursor = writeCursor = 0;
	}
	else
	{
		// 여유 공간이 버퍼 1개 크기 미만이면, 데이터를 앞으로 땅긴다.
		if (FreeSize() < bufferSize)
		{
			
			::memcpy(&buffer[0], &buffer[readCursor], dataSize);
			readCursor = 0;
			writeCursor = dataSize;
		}
	}
}

bool RecvBuffer::OnRead(int32 _numOfBytes)
{
	if (_numOfBytes > DataSize())
		return false;

	readCursor += _numOfBytes;
	return true;
}

bool RecvBuffer::OnWrite(int32 _numOfBytes)
{
	if (_numOfBytes > FreeSize())
		return false;

	writeCursor += _numOfBytes;
	return true;
}