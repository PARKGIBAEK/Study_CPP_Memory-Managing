#include "pch.h"
#include "RecvBuffer.h"
//#include <iterator> // �ʿ� ������� ���� ��, ���� Ȯ��


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
		// �� ��ħ �б�+���� Ŀ���� ������ ��ġ���, �� �� ����.
		readCursor = writeCursor = 0;
	}
	else
	{
		// ���� ������ ���� 1�� ũ�� �̸��̸�, �����͸� ������ �����.
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