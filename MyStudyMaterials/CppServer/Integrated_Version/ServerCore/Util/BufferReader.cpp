#include "BufferReader.h"



namespace ServerCore
{
BufferReader::BufferReader(BYTE* _buffer, uint32 _size, uint32 _pos)
	: buffer(_buffer), size(_size), pos(_pos)
{

}


bool BufferReader::Peek(void* dest, uint32 len)
{
	if (FreeSize() < len)
		return false;

	::memcpy(dest, &buffer[pos], len);
	return true;
}

bool BufferReader::Read(void* dest, uint32 len)
{
	if (Peek(dest, len) == false)
		return false;

	pos += len;
	return true;
}
}