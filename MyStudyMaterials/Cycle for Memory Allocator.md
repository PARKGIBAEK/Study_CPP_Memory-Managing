# Memory Allocator   &   STOMP_ALLOCATOR



## * class 설명

## 	1. Memory 클래스( 전역 변수로 선언 : GMemory )

```c++
class Memory
{
	enum
	{
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096,
	};

public:
	Memory();
	~Memory();

	void*	Allocate(int32 size);
	void	Release(void* ptr);
private:
	vector<MemoryPool*> _pools;
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};

template<typename Type, typename... Args>
Type* XNew(Args&&... args)
{
	Type* memory = static_cast<Type*>(PoolAllocator::AllocateMemory(sizeof(Type)));
	new(memory)Type(std::forward<Args>(args)...);
	return memory;
}

template<typename Type>
void XDelete(Type* obj)
{
	obj->~Type();
	PoolAllocator::ReleaseMemory(obj);
} 

template<typename Type, typename... Args>
std::shared_ptr<Type> MakeShared(Args&&... args)
{
	return std::shared_ptr<Type>{ XNew<Type>(forward<Args>(args)...), XDelete<Type> };
}
```

- Memory클래스의 생성자에서는 사이즈 별 메모리 풀을 생성한다.( 32 bytes ~ 4096 bytes )

- Pool Table : 할당 크기별 MemoryPool의 참조를 들고있다.

- Allocate 메서드를 통해 MemoryPool의 메모리를 꺼내오거나(pop), 할당한다(메모리 풀에 여분이 없을 경우).

  메모리 풀에 메모리를 할당할 때에는 MemoryHeader

- MemoryPool에 할당된 메모리에는 MemoryHeader가 

## 	2. MemoryPool 클래스

```c++

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void			Push(MemoryHeader* ptr);
	MemoryHeader* Pop();

private:
	SLIST_HEADER	header;
	int32			allocSize = 0;
	std::atomic<int32>	usedCount = 0;
	std::atomic<int32>	reservedCount = 0;
};


```

- SLIST_HEADER :
   windows 운영체제에서 제공하는 "Interlocked Singly-Linked List"락프리 자료구조의 시작노드
  시작노드가 아닌 노드들은 SLIST_ENTRY라는 구조체를 사용. (이 자료구조를 이용하여 메모리 풀링)

- allocSize : 
  메모리풀은 최초 생성 시 할당 사이즈가 정해지며 고정된다.

- usedCount :
  메모리 풀에서 꺼내어 사용 중인 객체의 갯수

- reservedCount :
  메모리 풀에서 생성된 객체의 갯수

- -

  



```c++
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY
{
	MemoryHeader(int32 size) : allocSize(size) { }
	
	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header) MemoryHeader(size); 
		return reinterpret_cast<void*>(++header);
	}
	
	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}
	
	int32 allocSize;
};
```

- allocSize :
  Memory Pool이 생성 될 때 정해지는 allocSize를 그대로 받아와서 MemoryHeader에 기입한다.
  즉,  MemoryPool의 allocSize와 같음.

- 메모리 Layout :
    [ MemoryHeader ] [-----------------------------------Data-----------------------------------]
  MemoryHeader : 맨앞에 위치하며, 최초 할당 시 메모리 관련 정보를 입력.
  Data : MemoryHeader바로 뒤쪽부터 Read/Write할 메모리 영역.

- SLIST : 
  windows 운영체제에서 제공하는 "Interlocked Singly-Linked List"락프리 자료구조.
  SLIST를 이용하여 메모리를 풀링한다.
  
- AttachHeader :
  할당 받은 메모리의 앞쪽 사이즈를 입력하고,

  실사용 메모리의 위치를 반환.





## * 메모리 풀링 시 Memory 할당 Cycle

1. xNew 메서드 호출
   

   ```c++
   template<typename Type, typename... Args>
   Type* XNew(Args&&... args)
   {	
   	Type* memory = static_cast<Type*>(PoolAllocator::AllocateMemory(sizeof(Type)));
   	new(memory)Type(std::forward<Args>(args)...);
   	return memory;
   }
   ```

   

2. XNew  =>  PoolAllocator::AllocateMemory(sizeof(Type)) 호출

   - aligned_malloc을 통한 메모리 할당의 경우 :
      MemoryHeader사이즈 만큼을 추가로 할당하고,
      앞부분을 MemoryHeader사이즈 영역으로 초기화 한 뒤
      메모리 주소를 MemoryHeader사이즈 만큼을 뒤로 넘긴 후 반환한다.

   - **STOMP_ALLOCATOR** 모드일 경우 :

     할당할 메모리 size에 맞게 페이지를 할당하고,
     실사용 메모리를 최대한 뒤쪽에 위치하도록하여 반환한다.
     그 이유는 할당된 가상메모리 영역을 벗어날 경우 메모리 엑세스 위반이 발생하게 하므로써
     메모리 오염을 확인하기 위한 용도

3. xNex 메서드 내부에서 호출한 Memory->AllocateMemory(size)가 반환한 메모리 주소에
   - placement new를 이용하여 타입에 맞는 생성자 호출
   - 생성자 수행완료 후 메모리 주소 반환



## * 오브젝트 풀링 시 Memory 할당 Cycle

```c++
template<typename Type>
class ObjectPool
{
public:
	template<typename... Args>
	static Type* Pop(Args&&... args)
	{
#ifdef _STOMP_ALLOCATOR
		MemoryHeader* ptr = reinterpret_cast<MemoryHeader*>(StompAllocator::AllocateMemory(s_allocSize));
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(ptr, s_allocSize));
#else
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize));
#endif		
		new(memory)Type(std::forward<Args>(args)...);
		return memory;
	}

static void Push(Type* obj)
{
	obj->~Type();

#ifdef _STOMP_ALLOCATOR
		StompAllocator::ReleaseMemory(MemoryHeader::DetachHeader(obj));
#else
		s_pool.Push(MemoryHeader::DetachHeader(obj));
#endif
	}

template<typename... Args>
static std::shared_ptr<Type> MakeShared(Args&&... args)
{
	std::shared_ptr<Type> ptr = { Pop(std::forward<Args>(args)...), Push };
	return ptr;
}

private:
	static int32		s_allocSize;
	static MemoryPool	s_pool;
};

template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };
```

- 템플릿 클래스이기 때문에 static이라고 해도 각 타입별로 따로 생성된다.
- MakeShared메서드에서 shared_ptr을 생성할 때 소멸자를 Push매서드로 지정해주므로써 풀링이 되도록하는 방식이다.
