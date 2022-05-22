# C++ IOCP 채팅 서버 프로젝트



- 이 프로젝트의 목적 :

  C++ MMORPG 게임 서버의 코어 라이브러리 중 기본적인 것들을 배우는 것입니다.


- Flow chart와 Class diagram은 draw.io에서 작성하였습니다.


------

# 최종 완성 프로젝트(Integrated_Version)의 구성



## 1. ServerCore(코어 라이브러리)



### Main

- CoreGlobal
- CoreMacro
- CorePch
- CoreTLS
- Types
- pch

### Memory

- Allocator
- Container
- Memory
- MemoryPool
- ObjectPool
- RefCounting
- TypeCast



### Network

- IocpCore
- IocpEvent
- Listener
- NetAddress
- RecvBuffer
- SendBuffer
- Service
- Session
- SocketUtils

### Thread

- DeadLockProfiler
- Lock
- ThreadManager

### Utils

- BufferReader
- BufferWriter
- LockQueue
- LockFreeStack   (only for learning)
- LockFreeQueue   (only for learning)

### Job

- GlobalQueue
- Job
- JobQueue
- JobTimer

### DB ( ODBC )

- DBBind
- DBConnection
- DBConnectionPool
- DBModel
- DBSynchronizer 

### XML ( xml Parser)





## 2. GameServer(서버)

- ClientPakcetHandler
- GameSession
- GameSessionManager
- GameServer ( 메인 )

## 3. DummyClient(더미 테스트 용)

- ServerPacketHandler
- DummyClient ( 메인 )
