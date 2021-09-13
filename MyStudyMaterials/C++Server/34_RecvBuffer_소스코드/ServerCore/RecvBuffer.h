#pragma once

/*--------------
	RecvBuffer
----------------*/

/*
	read cursor와 write cursor가 겹칠 경우 두 커서의 offset을 0으로 만들어준다.
	커서가 겹치는 상황이 오지 않고 버퍼의 공간이 모자랄 때까지 갈 경우
	read cursor와 write cursor사이에 있는 아직 처리하지 않은 데이터를 offset 0위치로 복사하고,
	커서도 그에 맞게 이동해준다.

	다만 버퍼의 공간이 모자랄 때까지 커서가 겹치지 않을 확률을 줄이기 위해
	버퍼의 크기를 굉장히 크게 잡는 방법을 사용한다.

*/
class RecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	RecvBuffer(int32 bufferSize);
	~RecvBuffer();
	// 커서 리셋 가능한지 확인 & 리셋
	void			Clean();
	// _readPos 이동
	bool			OnRead(int32 numOfBytes);
	// _writePos 이동
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