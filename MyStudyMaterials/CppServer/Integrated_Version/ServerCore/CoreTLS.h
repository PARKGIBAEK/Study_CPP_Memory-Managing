#pragma once
#include <memory>
#include <stack>
#include "Types.h"

class SendBufferChunk;
class JobQueue;


extern thread_local uint32				tls_ThreadId;
extern thread_local uint64				tls_EndTickCount;

extern thread_local std::stack<int32>	tls_LockStack;
extern thread_local std::shared_ptr<SendBufferChunk> 	tls_SendBufferChunk;
extern thread_local JobQueue*		tls_CurrentJobQueue;