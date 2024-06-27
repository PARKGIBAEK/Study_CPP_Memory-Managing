#pragma once
#include "CoreMacro.h"
#include "Types.h"
/*----------------
	BufferReader
-----------------*/

class BufferReader
{
public:
	BufferReader(BYTE* _buffer, uint32 _size, uint32 _pos = 0);

	BYTE*			Buffer() { return buffer; }
	uint32			Size() { return size; }
	uint32			ReadSize() { return pos; }
	uint32			FreeSize() { return size - pos; }

	template<typename T>
	bool			Peek(T* dest) { return Peek(dest, sizeof(T)); }
	bool			Peek(void* dest, uint32 len);

	template<typename T>
	bool			Read(T* dest) { return Read(dest, sizeof(T)); }
	bool			Read(void* dest, uint32 len);

	template<typename T>
	BufferReader&	operator>>(OUT T& dest);

private:
	BYTE*			buffer = nullptr;
	uint32			size = 0;
	uint32			pos = 0;
};

template<typename T>
inline BufferReader& BufferReader::operator>>(OUT T& dest)
{
	dest = *reinterpret_cast<T*>(&buffer[pos]);
	pos += sizeof(T);
	return *this;
}