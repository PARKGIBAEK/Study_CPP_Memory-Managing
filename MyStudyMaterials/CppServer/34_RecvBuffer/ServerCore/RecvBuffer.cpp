#include "pch.h"
#include "RecvBuffer.h"

/*--------------
	RecvBuffer
----------------*/

RecvBuffer::RecvBuffer(int32 bufferSize) : _bufferSize(bufferSize)
{
	_capacity = bufferSize * BUFFER_COUNT;
	_buffer.resize(_capacity);
}

RecvBuffer::~RecvBuffer()
{
}

void RecvBuffer::Clean()
{
	int32 dataSize = DataSize();
	if (dataSize == 0)// _readPos == _writePos
	{
		// 딱 마침 읽기+쓰기 커서가 동일한 위치라면, 둘 다 리셋.
		_readPos = _writePos = 0;

	}/*
	else if (FreeSize() < _bufferSize)
	{// 여유 공간이 버퍼 1개 크기 미만이면, 데이터를 앞으로 땅긴다.
		::memcpy(&_buffer[0], &_buffer[_readPos], dataSize);
		_readPos = 0;
		_writePos = dataSize;
	}	*/
	else
	{
		// 여유 공간이 버퍼 1개 크기 미만이면, 데이터를 앞으로 땅긴다.
		if (FreeSize() < _bufferSize)
		{
			//std::copy(_buffer[_readPos],_buffer[_readPos+dataSize],_buffer);
			::memcpy(&_buffer[0], &_buffer[_readPos], dataSize);
			_readPos = 0;
			_writePos = dataSize;
		}
	}
	
}

bool RecvBuffer::OnRead(int32 numOfBytes)
{
	if (numOfBytes > DataSize())
		return false;

	_readPos += numOfBytes;
	return true;
}

bool RecvBuffer::OnWrite(int32 numOfBytes)
{
	//버퍼에 여유공간이 없으면 false
	if (numOfBytes > FreeSize())
		return false;

	_writePos += numOfBytes;
	return true;
}