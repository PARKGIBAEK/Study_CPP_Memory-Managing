#pragma once
#include <mutex>
#include <memory>
#include <atomic>
#include "Types.h"


/* atomic�� compare_exchange_strong �Լ��� ����
* ��) origin.compare_exchange_strong(expected, desired)
	- origin�� expected�� ������ origin�� desired�� ���� ����
	- origin�� expected�� �ٸ��� origin�� expected�� ���� ����
*/

template<typename T>
class LockFreeQueue
{
	struct NodeCounter
	{
		uint32 internalCount : 30; // ������ ��ȯ ����
		uint32 externalCountRemaining : 2; // Push & Pop ���� ������ ����
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
			NodeCounter oldCounter = nodeCounter.load();// oldCounter�� ������ �ʴ´� ���� nodeCounter.internalCount�� 

			while (true)
			{
				NodeCounter newCounter = oldCounter;
				newCounter.internalCount--;

				/* �ٸ� Thread�� oldCounter�� nodeCounter�� �ǵ帮�� �ʾ����� nodeCounter�� internalCount�� 1�����Ѵ�.
				���� �ٸ� �Լ����� ��带 Push �Ǵ� Pop�ϰ� �� ��� FreeExternalCount�Լ��� ����
				externalCountRemaining�� �ٲ�� �Ǿ� compare_exchange_strong�� �����Ѵ�.
				�̸� ���� �ܺο��� head�� tail�� �ǵ帱 ��쿡 ����� �� �ְ� �ȴ�.	*/
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
		int32 externalCount; // ������
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

	/*	Push ����
	*   ���� ����
		[node][node][nullptr]
		  ��		        ��
		[head]		  [tail]

	*	��� ������ ���� ���� ��带 ����
		[node][node][nullptr][nullptr]
		  ��		        ��
		[head]		  [tail]

	*	���ο� ��带 ������ tail�� ����Ű�� ���̿� �������ְ� tail�� ���ο� ���̸� ����Ű���� ����
		[node][node][new node][nullptr]
		  ��				          ��
		[head]				    [tail]
	
	�� head�� tail�� ����Ű�� ���� CountedNodePtr.ptr�� �ǹ�
	*/
	void Push(const T& value)
	{
		std::unique_ptr<T> newData = std::make_unique<T>(value);

		CountedNodePtr dummy;
		dummy.nodePtr = new Node;
		dummy.externalCount = 1;

		CountedNodePtr tailCopy = tail.load(); // tail.nodePtr �� nullptr

		while (true)
		{
			// tail�� ���� ���� (externalCount�� ������ ���� +1 �� Thread�� �̱�)
			IncreaseExternalCount(tail, tailCopy);

			T* oldData = nullptr;
			/* tail�� ���� ������ ȹ���ϱ�(data�� ������ ��ȯ�� Thread�� �̱�)
			   tailCopy.nodePtr->data�� nullptr���ٸ� oldData�� ���� ������ newData�� ����� */
			if (tailCopy.nodePtr->data.compare_exchange_strong(oldData, newData.get()))
			{ // tail�� ���ο� ���̸� ����Ű�� �����ϱ�
				tailCopy.nodePtr->next = dummy;
				tailCopy = tail.exchange(dummy); // tail�� ���� ����Ű�� �ִ� dummy�� tailCopy�� ����
				FreeExternalCount(tailCopy);
				newData.release(); // resource�� ���� unique_ptr�� ������ ����(nullptr ����Ű��)
				break;
			}

			// ������ ���� �й�..
			tailCopy.nodePtr->ReleaseRef();
		}
	}

	std::shared_ptr<T> TryPop()
	{
		/*	[data][data][nullptr]
			  ��				��
			[head]		  [tail]
		*/

		CountedNodePtr oldHead = head.load();

		while (true)
		{
			// ������ ȹ�� (externalCount�� ������ ���� +1 �� �ְ� �̱�)
			IncreaseExternalCount(head, oldHead);

			Node* ptr = oldHead.nodePtr;
			if (ptr == tail.load().nodePtr)
			{
				ptr->ReleaseRef();
				return std::shared_ptr<T>();
			}

			// ������ ȹ�� (head = nodePtr->next)
			if (head.compare_exchange_strong(oldHead, ptr->next))
			{
				T* res = ptr->data.load(); // exchange(nullptr); �� �ϸ� ���� ����!
				FreeExternalCount(oldHead);
				return std::shared_ptr<T>(res);
			}

			ptr->ReleaseRef();
		}
	}

private:
	/* _counter�� _counterCopy�� ����Ǳ� ���� cas�� ������ �����尡 ���� Ż��
		�� ���� ��������� ���Ӱ� ����� newCounter�� counter */
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
		  ��				��
		[head]		  [tail]
	*/
	std::atomic<CountedNodePtr> head;
	std::atomic<CountedNodePtr> tail; // nullptr�� ����
};