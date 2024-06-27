#pragma once
#include <mutex>
#include <memory>
#include <atomic>
#include "Types.h"


/* atomic의 compare_exchange_strong 함수의 동작
* 예) origin.compare_exchange_strong(expected, desired)
	- origin과 expected가 같으면 origin에 desired를 복사 대입
	- origin과 expected가 다르면 origin에 expected를 복사 대입
*/

template<typename T>
class LockFreeQueue
{
	struct NodeCounter
	{
		uint32 internalCount : 30; // 참조권 반환 관련
		uint32 externalCountRemaining : 2; // Push & Pop 다중 참조권 관련
	};

	struct CountedNodePtr;

	struct Node
	{
		Node()
		{
			NodeCounter newCount;
			newCount.internalCount = 0;
			newCount.externalCountRemaining = 2;

			nodeCounter.store(newCount);

			next.nodePtr = nullptr;
			next.externalCount = 0;
		}

		void ReleaseRef()
		{
			NodeCounter oldCounter = nodeCounter.load();// oldCounter는 변하지 않는다 따라서 nodeCounter.internalCount는 

			while (true)
			{
				NodeCounter newCounter = oldCounter;
				newCounter.internalCount--;

				/* 다른 Thread가 oldCounter와 nodeCounter를 건드리지 않았으면 nodeCounter는 internalCount가 1감소한다.
				만약 다른 함수에서 노드를 Push 또는 Pop하게 될 경우 FreeExternalCount함수에 의해
				externalCountRemaining가 바뀌게 되어 compare_exchange_strong는 실패한다.
				이를 통해 외부에서 head나 tail을 건드릴 경우에 대비할 수 있게 된다.	*/
				if (nodeCounter.compare_exchange_strong(oldCounter, newCounter))
				{
					if (newCounter.internalCount == 0 && newCounter.externalCountRemaining == 0)
						delete this;

					break;
				}
			}
		}

		std::atomic<T*> data;
		std::atomic<NodeCounter> nodeCounter;
		CountedNodePtr next;
	};

	struct CountedNodePtr
	{
		int32 externalCount; // 참조권
		Node* nodePtr = nullptr;
	};

public:
	LockFreeQueue()
	{
		CountedNodePtr node;
		node.nodePtr = new Node;
		node.externalCount = 1;

		head.store(node);
		tail.store(node);
	}

	LockFreeQueue(const LockFreeQueue&) = delete;
	LockFreeQueue& operator=(const LockFreeQueue&) = delete;

	/*	Push 과정
	*   기존 상태
		[node][node][nullptr]
		  ↑		        ↑
		[head]		  [tail]

	*	노드 삽입을 위해 더미 노드를 생성
		[node][node][nullptr][nullptr]
		  ↑		        ↑
		[head]		  [tail]

	*	새로운 노드를 기존의 tail이 가리키던 더미에 연결해주고 tail은 새로운 더미를 가리키도록 변경
		[node][node][new node][nullptr]
		  ↑				          ↑
		[head]				    [tail]
	
	※ head와 tail이 가리키는 노드는 CountedNodePtr.ptr를 의미
	*/
	void Push(const T& value)
	{
		std::unique_ptr<T> newData = std::make_unique<T>(value);

		CountedNodePtr dummy;
		dummy.nodePtr = new Node;
		dummy.externalCount = 1;

		CountedNodePtr tailCopy = tail.load(); // tail.nodePtr 은 nullptr

		while (true)
		{
			// tail에 대한 경합 (externalCount를 현시점 기준 +1 한 Thread가 이김)
			IncreaseExternalCount(tail, tailCopy);

			T* oldData = nullptr;
			/* tail에 대한 소유권 획득하기(data를 먼저를 교환한 Thread가 이김)
			   tailCopy.nodePtr->data가 nullptr였다면 oldData와 같기 때문에 newData로 변경됨 */
			if (tailCopy.nodePtr->data.compare_exchange_strong(oldData, newData.get()))
			{ // tail이 새로운 더미를 가리키게 변경하기
				tailCopy.nodePtr->next = dummy;
				tailCopy = tail.exchange(dummy); // tail이 원래 가리키고 있던 dummy를 tailCopy에 복사
				FreeExternalCount(tailCopy);
				newData.release(); // resource에 대한 unique_ptr의 소유권 포기(nullptr 가리키기)
				break;
			}

			// 소유권 경쟁 패배..
			tailCopy.nodePtr->ReleaseRef();
		}
	}

	std::shared_ptr<T> TryPop()
	{
		/*	[data][data][nullptr]
			  ↑				↑
			[head]		  [tail]
		*/

		CountedNodePtr oldHead = head.load();

		while (true)
		{
			// 참조권 획득 (externalCount를 현시점 기준 +1 한 애가 이김)
			IncreaseExternalCount(head, oldHead);

			Node* ptr = oldHead.nodePtr;
			if (ptr == tail.load().nodePtr)
			{
				ptr->ReleaseRef();
				return std::shared_ptr<T>();
			}

			// 소유권 획득 (head = nodePtr->next)
			if (head.compare_exchange_strong(oldHead, ptr->next))
			{
				T* res = ptr->data.load(); // exchange(nullptr); 로 하면 버그 있음!
				FreeExternalCount(oldHead);
				return std::shared_ptr<T>(res);
			}

			ptr->ReleaseRef();
		}
	}

private:
	/* _counter와 _counterCopy가 변경되기 전에 cas를 수행한 스레드가 먼저 탈출
		그 다음 스레드들은 새롭게 변경된 newCounter와 counter */
	static void IncreaseExternalCount(std::atomic<CountedNodePtr>& _counter,
		CountedNodePtr& _counterCopy)
	{
		while (true)
		{
			CountedNodePtr newCounter = _counterCopy;
			newCounter.externalCount++;


			if (_counter.compare_exchange_strong(_counterCopy, newCounter))
			{
				_counterCopy.externalCount = newCounter.externalCount;
				break;
			}
		}
	}

	static void FreeExternalCount(CountedNodePtr& oldNodePtr)
	{
		Node* ptr = oldNodePtr.nodePtr;
		const int32 countIncrease = oldNodePtr.externalCount - 2;

		NodeCounter oldCounter = ptr->nodeCounter.load();

		while (true)
		{
			NodeCounter newCounter = oldCounter;
			newCounter.externalCountRemaining--; // TODO
			newCounter.internalCount += countIncrease;

			if (ptr->nodeCounter.compare_exchange_strong(oldCounter, newCounter))
			{
				if (newCounter.internalCount == 0 && newCounter.externalCountRemaining == 0)
					delete ptr;

				break;
			}
		}
	}

private:
	/*	[data][data][nullptr]
		  ↑				↑
		[head]		  [tail]
	*/
	std::atomic<CountedNodePtr> head;
	std::atomic<CountedNodePtr> tail; // nullptr를 유지
};