#pragma once
#include <mutex>


template<typename T>
class LockFreeQueue {

	struct Node;

	struct CountedNodePtr
	{
		int32 externalCount;
		Node* node = nullptr;
	};

	struct NodeCounter
	{
		uint32 internalCount : 30;
		uint32 externalCountRemaining : 2;
	};

	struct Node
	{
		Node()
		{
			NodeCounter newCount;
			newCount.internalCount = 0;
			newCount.externalCountRemaining = 2;
			nodeCounter.store(newCount);

			next.ptr = nullptr;
			next.externalCount = 0;
		}

		void ReleaseRef()
		{
			NodeCounter oldCounter = nodeCounter.load();

			while (true)
			{
				NodeCounter newCounter = oldCounter;
				newCounter.internalCount--;

				//끼어들 수 있음
				if (nodeCounter.compare_exchange_strong(oldCounter, newCounter))
				{
					if (newCunter.internalCouont == 0 && newCounter.externalCountRemaining == 0)
						delete this;
					break;
				}
			}
		}

		std::atomic<T*> data;
		std::atomic<NodeCounter> nodeCounter;
		CountedNodePtr next;
	};

public:
	LockFreeQueue()
	{
		CountedNodePtr node;
		node.ptr = new Node;
		node.externalCount = 1;

		head.store(node);
		tail.store(node);
	}

	LockFreeQueue(const LockFreeQueue&) = delete;
	LockFreeQueue& operator=(const LockFreeQueue&) = delete;


	void Push(const T& value)
	{
		//삽입할 데이터 생성
		std::unique_ptr<T> newData = std::make_unique<T>(value);

		CountedNodePtr dummy;
		dummy.ptr = new Node;
		dummy.externalCount = 1;

		CountedNodePtr oldTail = tail.load(); // tail은 더미노드를 가리키고 있을 것이므로 내용은 nullptr이다

		while (true) 
		{
			// 참조권 획득 ( externalCount를 현시점 기준 +1 한 Thread가 이김 )
			IncreaseExternalCount(tail, oldTail);

			//소유권 획득 ( CAS를 통해 데이터 교환을 먼저한 Thread가 이김 )
			T* oldData = nullptr;
			if (oldTail.ptr->data.compare_exchange_strong(oldData, newData.get()))
			{//기존의 tail이 가리키던 노
				oldTail.ptr->next = dummy;
				oldTail = tail.exchange(dummy);
				FreeExternalCount(oldTail);
				newData.release(); // 데이터에 대한 unique_ptr의 소유권 포기
				break;
			}
		}
		// 소유권 경쟁 패배
		oldTail.ptr->ReleaseRef();
	}

	std::shared_ptr<T> TryPop() 
	{

		CountedNodePtr oldHead = head.load();

		while (true)
		{
			// 참조권 획득 (externalCount를 현 시점 기준 +1 한 Thread가 이김 )
			IncreaseExternalCount()

				Node* ptr = oldHead.ptr;
			if (ptr == tail.load().ptr)
			{
				ptr->ReleaseRef();
				return std::shared_ptr<T>();
			}
			//소유권 획득 ( head 를 ptr->next로 먼저 변경한 Thread가 승리
			if (head.compare_exchange_strong(oldHead, ptr->next))
			{
				T* res = ptr->data.exchange(nullptr);
				FreeExternalCount(oldHead);
				return std::shared_ptr<T>(res);
			}
		}

		return res;
	}

private:
	static void IncreaseExternalCount(std::atomic<CountedNodePtr>& counter, CountedNodePtr& oldCounter) 
	{
		while (true) 
		{
			CountedNodePtr newCounter = oldCounter;
			newCounter.externalCount++;

			if (counter.compare_exchange_strong(oldCounter, newCounter)) 
			{
				oldCounter.externalCount = newCounter.externalCount;
				break;
			}
		}
	}

	static void FreeExternalCount(CountedNodePtr& oldNodePtr) 
	{
		Node* ptr = oldNodePtr.ptr;
		const int32 countIncrease = oldNodePtr.externalCount - 2;

		NodeCounter oldCounter = ptr->count.load();

		while (true) 
		{
			NodeCounter newCounter = oldCounter;
			newCounter.externalCountRemaining--;
			newCounter.internalCount += countIncrease;

			if (ptr->count.compare_exchange_strong(oldCounter, newCounter)) 
			{
				if (newCounter.internalCount == 0 && newCounter.externalCounterRemaining)
					delete ptr;

				break;
			}
		}
	}
private:
	// head == tail 이면 empty 상태
	std::atomic<CountedNodePtr> head = nullptr;
	std::atomic<CountedNodePtr> tail = nullptr;
};

/* 사용 안함 */


/*
template<typename T>
class LockQueue
{
public:
	LockQueue() { }

	LockQueue(const LockQueue&) = delete;
	LockQueue& operator=(const LockQueue&) = delete;

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_queue.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_queue.empty())
			return false;

		value = std::move(_queue.front());
		_queue.pop();
		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] { return _queue.empty() == false; });
		value = std::move(_queue.front());
		_queue.pop();
	}

private:
	queue<T> _queue;
	mutex _mutex;
	condition_variable _condVar;
};
*/