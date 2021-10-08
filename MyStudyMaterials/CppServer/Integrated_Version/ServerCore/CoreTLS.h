#pragma once
#include <stack>

/*������Ͽ��� ������ ������ ��� �����߻��ϰ� �ǹǷ�,
 ���� ����& ���Ǵ� cpp���Ͽ��� �ؾ��Ѵ�.
 extern Ű����� �ܺ� ������� �ǹ̸� �����Ϸ����� �����ϴ� ���̹Ƿ�
 ������Ͽ��� extern���� �����ϴ°ͱ����� �����ϴ�. (������ ���Ǵ� �Ұ���)
*/
extern thread_local uint32				LThreadId;
extern thread_local uint64				LEndTickCount;

extern thread_local std::stack<int32>	LLockStack;
extern thread_local SendBufferChunkRef	LSendBufferChunk;
extern thread_local class JobQueue*		LCurrentJobQueue;