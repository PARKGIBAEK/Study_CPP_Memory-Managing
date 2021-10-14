# C++ IOCP 채팅 서버 프로젝트

- 프로젝트 출처
  인프런의 C++ 서버 강좌(Rookiss님 강의)를 따라서 만든 프로젝트입니다.

- 이 프로젝트의 목적

  C++ MMORPG 게임 서버의 코어 라이브러리 중 기본적인 것들을 배우는 것입니다.

  
  



최종 완성 프로젝트의 구성

## 1. ServerCore(코어 라이브러리)

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



## 3. DummyClient(더미 테스트 용)

