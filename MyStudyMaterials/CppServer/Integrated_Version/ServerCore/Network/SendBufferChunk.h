#pragma once

#include <array>
#include <memory>
#include "../Core/Types.h"


namespace ServerCore
{


class SendBuffer;

class SendBufferChunk : public std::enable_shared_from_this<SendBufferChunk>
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
    BYTE*					Buffer();
    uint32					FreeSize();

private:
    std::array<BYTE, SEND_BUFFER_CHUNK_SIZE>		buffer;
    bool									isOpen;
    uint32									usedSize; // buffer cursor
};
}