#pragma once
#include <mutex>
#include <iostream>
#include <atomic>

namespace ServerCore
{
/* <sql.h>나 ,<sqlext.h>같은 ODBC API가
 * 크로스 플래폼 기반임에도 불구하고
 * 플래폼 버전에 따른 종속성을 보이고 있어서 임시로 제거해 둠
 * 관련 라이브러리는 DB~로 시작함
 * DBConnection
 */

class DeadLockProfiler;
class ThreadManager;
class MemoryManager;
class SendBufferManager;
class GlobalQueue;
class JobTimer;
// class DBConnectionPool;
class ConsoleLog;

extern DeadLockProfiler* GDeadLockProfiler;
extern ThreadManager* GThreadManager;
extern MemoryManager* GMemoryManager;
extern SendBufferManager* GSendBufferManager;
extern GlobalQueue* GGlobalQueue;
extern JobTimer* GJobTimer;
// extern DBConnectionPool*	GDBConnectionPool;
extern ConsoleLog* GConsoleLogger;

class CoreInitializer
{
public:
    static void Init()
    {
        static CoreInitializer instance; // scoped static
        if (isInitialized)
            std::cout << "CoreInitializer has already been initialized" << std::endl;
        else
        {
            isInitialized.store(true);
            std::cout << "CoreInitializer is initialized" << std::endl;
        }
        instance.DummyMethod();
    }

private:
    static std::atomic<bool> isInitialized;
    CoreInitializer();
    ~CoreInitializer();

    static void DummyMethod()
    {
    }

    CoreInitializer(const CoreInitializer&) = delete;
    CoreInitializer& operator=(const CoreInitializer&) = delete;
};
}
