# C++ IOCP 채팅 서버


- 목적 :

  C++ MMORPG 게임 서버의 코어 라이브러리 중 기본적인 것들을 배우는 것.


- 참고 자료 :  Flow chart, Class diagram( draw.io로 작성)

- 기타 :  00 ~ 58은 유닛 테스트 용입니다. 
            메인 프로젝트는 Integrated_Version입니다.

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

- rapidxml 라이브러리



## 2. GameServer(서버)

- ClientPakcetHandler
- GameSession
- GameSessionManager
- GameServer ( main )

## 3. DummyClient(더미 테스트 용)

- ServerPacketHandler
- DummyClient ( 메인 )

## 외부 라이브러리

- Protobuf
- RapidXML(XML Parser)