#pragma once
#include <stack>

/*������Ͽ��� ������ ������ ��� �����߻��ϰ� �ǹǷ�,
 ���� ����& ���Ǵ� cpp���Ͽ��� �ؾ��Ѵ�.
 extern Ű����� �ܺ� ������� �ǹ̸� �����Ϸ����� �����ϴ� ���̹Ƿ�
 ������Ͽ��� extern���� �����ϴ°ͱ����� �����ϴ�. (������ ���Ǵ� �Ұ���)
*/
extern thread_local uint32				tls_ThreadId;
extern thread_local uint64				tls_EndTickCount;

extern thread_local std::stack<int32>	tls_LockStack;
extern thread_local std::shared_ptr<class SendBufferChunk> 	tls_SendBufferChunk;
extern thread_local class JobQueue*		tls_CurrentJobQueue;