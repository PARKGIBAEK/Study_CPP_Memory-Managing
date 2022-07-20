#pragma once

class SendBufferChunk;

/*----------------
	SendBuffer
-----------------*/

class SendBuffer
{
public:
	SendBuffer(std::shared_ptr<SendBufferChunk> _owner, BYTE* _buffer, uint32 _allocSize);
	~SendBuffer();

	BYTE*				Buffer() { return buffer; }
	uint32				AllocSize() { return allocSize; }
	uint32				WriteSize() { return writeSize; }
	void				Close(uint32 _writeSize);

private:
	BYTE*				buffer;
	uint32				allocSize = 0;
	uint32				writeSize = 0;
	std::shared_ptr<SendBufferChunk>	owner;
};

/*--------------------
	SendBufferChunk
--------------------*/

class SendBufferChunk : public enable_shared_from_this<SendBufferChunk>
{
	enum
	{
		SEND_BUFFER_CHUNK_SIZE = 6000
	};

public:
	SendBufferChunk();
	~SendBufferChunk();

	void					ResetBuffer();
	std::shared_ptr<SendBuffer>			Open(uint32 _allocSize);
	void					Close(uint32 _writeSize);

	bool					IsOpen() { return isOpen; }
	BYTE*					Buffer() { return &buffer[usedSize]; }
	uint32					FreeSize() { return static_cast<uint32>(buffer.size()) - usedSize; }

private:
	Array<BYTE, SEND_BUFFER_CHUNK_SIZE>		buffer = {};
	bool									isOpen = false;
	uint32									usedSize = 0; // buffer cursor
};

/*---------------------
	SendBufferManager
----------------------*/

/*
	vector �����̳��� sendBufferChunks�� Pooling������ ��.
	Pop�� sendBuffferChunks�� ������ ������ �����ְ�, ������ XNew�� ���� ����
*/
class SendBufferManager
{
public:
	std::shared_ptr<SendBuffer>				Open(uint32 _size);

private:
	std::shared_ptr<SendBufferChunk> 			Pop();
	void						Push(std::shared_ptr<SendBufferChunk>  _buffer);
	static void					PushGlobal(SendBufferChunk* _buffer);

private:
	USE_LOCK;
	Vector<std::shared_ptr<SendBufferChunk> >	sendBufferChunks;
};
