#pragma once
#include "Types.h"
/*----------------
	BufferWriter
-----------------*/

class BufferWriter
{
public:
	BufferWriter();
	BufferWriter(BYTE* _buffer, uint32 size, uint32 pos = 0);
	~BufferWriter();

	BYTE*				Buffer() { return buffer; }
	uint32			Size() { return size; }
	uint32			WriteSize() { return pos; }
	uint32			FreeSize() { return size - pos; }

	template<typename T>
	bool				Write(T* src) { return Write(src, sizeof(T)); }
	bool				Write(void* src, uint32 len);

	template<typename T>
	T*					Reserve(uint16 count = 1);

	template<typename T>
	BufferWriter&	operator<<(T&& src);

private:
	BYTE*			buffer = nullptr;
	uint32			size = 0;
	uint32			pos = 0;
};

template<typename T>
T* BufferWriter::Reserve(uint16 count)
{
	if (FreeSize() < (sizeof(T) * count))
		return nullptr;

	T* ret = reinterpret_cast<T*>(&buffer[pos]);
	pos += (sizeof(T) * count);
	return ret;
}

template<typename T>
BufferWriter& BufferWriter::operator<<(T&& src)
{
	using DataType = std::remove_reference_t<T>;
	*reinterpret_cast<DataType*>(&buffer[pos]) = std::forward<DataType>(src);
	pos += sizeof(T);
	return *this;
}