#pragma once

/* class의 extern 선언은 객체들은 전역으로 사용하기 위한 선언 방식이다.
* 하지만 결합도를 높이며, 해당 객체에 대한 접근 시점이 해당 객체가 초기화 되는 시점보다 이를 경우 오류가 발생한다.
* 이 오류는 원인을 찾기가 굉장히 어렵다.
* */
extern class ThreadManager*		GThreadManager;
extern class Memory*					GMemory;
extern class SendBufferManager*	GSendBufferManager;
extern class DeadLockProfiler*	GDeadLockProfiler;

