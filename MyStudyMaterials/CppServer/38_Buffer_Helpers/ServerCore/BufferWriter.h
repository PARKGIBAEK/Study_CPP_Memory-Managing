#pragma once

/*----------------
	BufferWriter
-----------------*/

class BufferWriter
{
public:
	BufferWriter();
	BufferWriter(BYTE* buffer, uint32 size, uint32 pos = 0);
	~BufferWriter();

	BYTE*			Buffer() { return _buffer; }
	uint32			Size() { return _size; }
	uint32			WriteSize() { return _pos; }
	uint32			FreeSize() { return _size - _pos; }

	template<typename T>
	bool			Write(T* src) { return Write(src, sizeof(T)); }
	bool			Write(void* src, uint32 len);

	template<typename T>
	T*				Reserve();// T의 사이즈 만큼 커서 넘기기

	template<typename T>
	BufferWriter&	operator<<(const T& src);

	template<typename T>
	BufferWriter&	operator<<(T&& src);

private:
	BYTE*				_buffer = nullptr;
	uint32			_size = 0;
	uint32			_pos = 0;
};

template<typename T>
T* BufferWriter::Reserve()
{
	if (FreeSize() < sizeof(T))
		return nullptr;
	// 현재 버퍼 커서위치의 주소를 반환하고
	T* ret = reinterpret_cast<T*>(&_buffer[_pos]);
	_pos += sizeof(T);// T의 크기만큼 커서 이동
	return ret;
}

template<typename T>
BufferWriter& BufferWriter::operator<<(const T& src)
{
	*reinterpret_cast<T*>(&_buffer[_pos]) = src;
	_pos += sizeof(T);
	return *this;
}

template<typename T>
BufferWriter& BufferWriter::operator<<(T&& src)
{
	*reinterpret_cast<T*>(&_buffer[_pos]) = std::move(src);
	_pos += sizeof(T);
	return *this;
}