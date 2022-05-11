#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.


#ifdef _DEBUG
/*(Additional) Library Directories에 설정된 경로들을 기준으로 탐색한다*/
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "Core.h"
#include "Enum.pb.h"

using GameSessionRef = shared_ptr<class GameSession>;
using PlayerRef = shared_ptr<class Player>;