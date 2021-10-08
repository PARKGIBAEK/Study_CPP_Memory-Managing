#pragma once

#include <mutex>

//간단한 Stack ( thread-safe )
template<typename T>
class LockStack
{
public:
	LockStack() { }

	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;

		// empty -> top -> pop
		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] { return _stack.empty() == false; });
		value = std::move(_stack.top());
		_stack.pop();
	}

private:
	std::stack<T> _stack;
	std::mutex _mutex;
	std::condition_variable _condVar;
};


// 강의 영상 9분20초부터 보면 됨
template<typename T>
class LockFreeStack
{
	//struct Node;

	struct CountedNodePtr
	{
		int32 externalCount = 0;
		struct Node* ptr = nullptr;
	};

	struct Node
	{
		Node(const T& value) : data(make_shared<T>(value))
		{	}

		shared_ptr<T> data;
		atomic<int32> internalCount = 0;
		CountedNodePtr next;
	};

public:
	// [][][][][][][]
	// [head]
	void Push(const T& value)
	{
		CountedNodePtr node;//새로 삽입할 노드
		node.ptr = new Node(value);
		node.externalCount = 1;//외부 참조 카운트 1로 생성
		// [!]
		node.ptr->next = head;// head는 가장 top 노드
		/*head == node.ptr->next 일 경우(다른 스레드의 간섭을 받지 않은 상황), head = node , true 반환,   
		  head != node.ptr->next 일 경우(다른 스레드의 간섭을 받은 상황), node.ptr->next = head , false 반환(루프 재 진입하게되고 이번에 간섭받지 않았다면 통과)*/
		while (head.compare_exchange_weak(node.ptr->next, node) == false)
		{
		}
	}

	// [][][][][][][]
	// [head]
	std::shared_ptr<T> TryPop()
	{
		CountedNodePtr oldHead = head;//top 노드 꺼내오기
		while (true)
		{
			// 참조권 획득 (externalCount를 현 시점 기준 +1 한 애가 이김)
			IncreaseHeadCount(oldHead);
			// 최소한 externalCount >= 2 일테니 삭제X (oldHead에 대하여 안전하게 접근할 수 있음을 보장)
			Node* ptr = oldHead.ptr;//진짜 top에 있는 노드의 포인터를 꺼내옴

			// 데이터 없음
			if (ptr == nullptr)
				return std::shared_ptr<T>();

			// 소유권 획득 (ptr->next로 head를 바꿔치기 한 애가 이김)
			if (head.compare_exchange_strong(oldHead, ptr->next))//head와 oldHead가 같은(비트레벨에서 동일해야하므로 externalCount까지 동일) 경우 = 아직 다른 스레드에서 stack을 변경하지 않았다는 얘기
			{
				std::shared_ptr<T> res;
				res.swap(ptr->data);

				// external : 1 -> 2(나+1) -> 4(나+1 남+2)
				// internal : 1 -> 0
				const int32 countIncrease = oldHead.externalCount - 2;

				if (ptr->internalCount.fetch_add(countIncrease) == -countIncrease)
					delete ptr;

				return res;
			}
			else if (ptr->internalCount.fetch_sub(1) == 1)
			{
				// 참조권은 얻었으나, 소유권은 실패 -> 뒷수습은 내가 한다
				delete ptr;
			}
		}
	}

private:
	void IncreaseHeadCount(CountedNodePtr& oldCounter)
	{
		while (true)
		{
			CountedNodePtr newCounter = oldCounter;// old Counter는 이 함수가 호출 될 당시의 head였던 노드
			newCounter.externalCount++;
			/*head는 top 노드이므로 oldCounter와 다르다면, 다른 쓰레드가 중간에 노드를 pop또는 push한 경우이다.
			* 즉, head != oldCounter일 경우에는 oldCounter = head가 된다.(즉 oldCounter는 현재의 head를 가리킨다)
			* 하지만 head == oldCounter일 경우, head = newCounter가 되는데, 한번만에 만족했을 경우에는 newCounter와 oldCounter가 일치한다.
			* 한번만에 만족하지 않았을 경우에는, oldCounter가 현재의 head로 변한다.
			* 그럼 다시 최근의 헤드가 그대로 변화하지 않았는지를 확인하는 과정을 반복하는 것이다.
			*/
			if (head.compare_exchange_strong(oldCounter, newCounter))
			{//head == oldCounter를 만족하면, head = newCounter가 된다
				oldCounter.externalCount = newCounter.externalCount;
				break;
			}
		}
	}

private:
	std::atomic<CountedNodePtr> head; // 탑 노드
};

//간단한 Lock-free Stack
//template<typename T>
//class LockFreeStack
//{
//	struct Node
//	{
//		Node(const T& value) : data(make_shared<T>(value)), next(nullptr)
//		{
//
//		}
//
//		shared_ptr<T> data;
//		shared_ptr<Node> next;
//	};
//
//public:
//	void Push(const T& value)
//	{
//		shared_ptr<Node> node = make_shared<Node>(value);
//		node->next = std::atomic_load(&_head);
//		while (std::atomic_compare_exchange_weak(&_head, &node->next, node) == false)
//		{
//		}
//	}
//
//	shared_ptr<T> TryPop()
//	{
//		shared_ptr<Node> oldHead = std::atomic_load(&_head);
//
//		while (oldHead && std::atomic_compare_exchange_weak(&_head, &oldHead, oldHead->next) == false)
//		{
//
//		}
//
//		if (oldHead == nullptr)
//			return shared_ptr<T>();
//
//		return oldHead->data;
//	}
//
//private:
//	shared_ptr<Node> _head;
//};
