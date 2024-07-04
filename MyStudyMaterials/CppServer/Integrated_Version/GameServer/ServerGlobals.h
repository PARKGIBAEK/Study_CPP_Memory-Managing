#pragma once
#include "Core/CoreInitializer.h"
#include "Core/CoreMacro.h"
#include "Network/SocketUtils.h"

namespace GameServer
{
using namespace ServerCore;
class ServerGlobals
{
    
public:
    ServerGlobals()
    {
        CoreInitializer::Init();

        /*SocketUtils::Init();
        DEBUG_LOG("[ GDeadLockProfiler ]  -  Initialized");*/
    }
};

extern ServerGlobals globals;
}
