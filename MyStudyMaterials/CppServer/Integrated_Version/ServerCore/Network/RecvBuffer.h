#pragma once
#include "../Memory/Container.h"
#include "../Core/Types.h"



namespace ServerCore
{


class RecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	RecvBuffer(int32 _bufferSize);
	~RecvBuffer();

	void			Clean();
	bool			OnRead(int32 _numOfBytes);
	bool			OnWrite(int32 _numOfBytes);

	BYTE*			ReadPos() { return &buffer[readCursor]; }
	BYTE*			WritePos() { return &buffer[writeCursor]; }
	int32			DataSize() { return writeCursor - readCursor; }
	int32			FreeSize () { return capacity - writeCursor; }

private:
	int32			capacity = 0;
	int32			bufferSize = 0;
	int32			readCursor = 0;
	int32			writeCursor = 0;
	Vector<BYTE>	buffer;
};
}