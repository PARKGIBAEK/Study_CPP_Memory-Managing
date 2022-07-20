#pragma once
#include <stack>

/*헤더파일에서 변수를 선언할 경우 에러발생하게 되므로,
 실제 선언& 정의는 cpp파일에서 해야한다.
 extern 키워드는 외부 참조라는 의미를 컴파일러에게 전달하는 것이므로
 헤더파일에서 extern으로 선언하는것까지는 가능하다. (하지만 정의는 불가능)
*/
extern thread_local uint32				tls_ThreadId;
extern thread_local uint64				tls_EndTickCount;

extern thread_local std::stack<int32>	tls_LockStack;
extern thread_local std::shared_ptr<class SendBufferChunk> 	tls_SendBufferChunk;
extern thread_local class JobQueue*		tls_CurrentJobQueue;