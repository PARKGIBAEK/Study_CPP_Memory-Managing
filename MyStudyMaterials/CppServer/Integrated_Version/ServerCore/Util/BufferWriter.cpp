#include "BufferWriter.h"

namespace ServerCore
{
BufferWriter::BufferWriter()
{

}

BufferWriter::BufferWriter(BYTE* _buffer, uint32 _size, uint32 _pos)
	: buffer(_buffer), size(_size), pos(_pos)
{

}

BufferWriter::~BufferWriter()
{

}

bool BufferWriter::Write(void* src, uint32 len)
{
	if (FreeSize() < len)
		return false;

	::memcpy(&buffer[pos], src, len);
	pos += len;
	return true;
}
}