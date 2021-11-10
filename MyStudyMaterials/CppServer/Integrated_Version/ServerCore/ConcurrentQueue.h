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

				//����� �� ����
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
		//������ ������ ����
		std::unique_ptr<T> newData = std::make_unique<T>(value);

		CountedNodePtr dummy;
		dummy.ptr = new Node;
		dummy.externalCount = 1;

		CountedNodePtr oldTail = tail.load(); // tail�� ���̳�带 ����Ű�� ���� ���̹Ƿ� ������ nullptr�̴�

		while (true) 
		{
			// ������ ȹ�� ( externalCount�� ������ ���� +1 �� Thread�� �̱� )
			IncreaseExternalCount(tail, oldTail);

			//������ ȹ�� ( CAS�� ���� ������ ��ȯ�� ������ Thread�� �̱� )
			T* oldData = nullptr;
			if (oldTail.ptr->data.compare_exchange_strong(oldData, newData.get()))
			{//������ tail�� ����Ű�� ��
				oldTail.ptr->next = dummy;
				oldTail = tail.exchange(dummy);
				FreeExternalCount(oldTail);
				newData.release(); // �����Ϳ� ���� unique_ptr�� ������ ����
				break;
			}
		}
		// ������ ���� �й�
		oldTail.ptr->ReleaseRef();
	}

	std::shared_ptr<T> TryPop() 
	{

		CountedNodePtr oldHead = head.load();

		while (true)
		{
			// ������ ȹ�� (externalCount�� �� ���� ���� +1 �� Thread�� �̱� )
			IncreaseExternalCount()

				Node* ptr = oldHead.ptr;
			if (ptr == tail.load().ptr)
			{
				ptr->ReleaseRef();
				return std::shared_ptr<T>();
			}
			//������ ȹ�� ( head �� ptr->next�� ���� ������ Thread�� �¸�
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
	// head == tail �̸� empty ����
	std::atomic<CountedNodePtr> head = nullptr;
	std::atomic<CountedNodePtr> tail = nullptr;
};

/* ��� ���� */


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