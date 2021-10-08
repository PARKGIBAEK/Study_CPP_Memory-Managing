#pragma once

/*--------------
	RecvBuffer
----------------*/

/*
	read cursor�� write cursor�� ��ĥ ��� �� Ŀ���� offset�� 0���� ������ش�.
	Ŀ���� ��ġ�� ��Ȳ�� ���� �ʰ� ������ ������ ���ڶ� ������ �� ���
	read cursor�� write cursor���̿� �ִ� ���� ó������ ���� �����͸� offset 0��ġ�� �����ϰ�,
	Ŀ���� �׿� �°� �̵����ش�.

	�ٸ� ������ ������ ���ڶ� ������ Ŀ���� ��ġ�� ���� Ȯ���� ���̱� ����
	������ ũ�⸦ ������ ũ�� ��� ����� ����Ѵ�.

*/
class RecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	RecvBuffer(int32 bufferSize);
	~RecvBuffer();
	// Ŀ�� ���� �������� Ȯ�� & ����
	void			Clean();
	// _readPos �̵�
	bool			OnRead(int32 numOfBytes);
	// _writePos �̵�
	bool			OnWrite(int32 numOfBytes);

	BYTE*			ReadPos() { return &_buffer[_readPos]; }
	BYTE*			WritePos() { return &_buffer[_writePos]; }
	// _writePos - _readPos;
	int32			DataSize() { return _writePos - _readPos; }
	// _capacity - _writePos
	int32			FreeSize() { return _capacity - _writePos; }

private:
	int32			_capacity = 0;
	int32			_bufferSize = 0;
	int32			_readPos = 0;
	int32			_writePos = 0;
	Vector<BYTE>	_buffer;
};