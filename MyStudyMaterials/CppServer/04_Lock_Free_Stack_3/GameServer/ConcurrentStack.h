#pragma once

#include <mutex>

//������ Stack ( thread-safe )
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


// ���� ���� 9��20�ʺ��� ���� ��
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
		CountedNodePtr node;//���� ������ ���
		node.ptr = new Node(value);
		node.externalCount = 1;//�ܺ� ���� ī��Ʈ 1�� ����
		// [!]
		node.ptr->next = head;// head�� ���� top ���
		/*head == node.ptr->next �� ���(�ٸ� �������� ������ ���� ���� ��Ȳ), head = node , true ��ȯ,   
		  head != node.ptr->next �� ���(�ٸ� �������� ������ ���� ��Ȳ), node.ptr->next = head , false ��ȯ(���� �� �����ϰԵǰ� �̹��� �������� �ʾҴٸ� ���)*/
		while (head.compare_exchange_weak(node.ptr->next, node) == false)
		{
		}
	}

	// [][][][][][][]
	// [head]
	std::shared_ptr<T> TryPop()
	{
		CountedNodePtr oldHead = head;//top ��� ��������
		while (true)
		{
			// ������ ȹ�� (externalCount�� �� ���� ���� +1 �� �ְ� �̱�)
			IncreaseHeadCount(oldHead);
			// �ּ��� externalCount >= 2 ���״� ����X (oldHead�� ���Ͽ� �����ϰ� ������ �� ������ ����)
			Node* ptr = oldHead.ptr;//��¥ top�� �ִ� ����� �����͸� ������

			// ������ ����
			if (ptr == nullptr)
				return std::shared_ptr<T>();

			// ������ ȹ�� (ptr->next�� head�� �ٲ�ġ�� �� �ְ� �̱�)
			if (head.compare_exchange_strong(oldHead, ptr->next))//head�� oldHead�� ����(��Ʈ�������� �����ؾ��ϹǷ� externalCount���� ����) ��� = ���� �ٸ� �����忡�� stack�� �������� �ʾҴٴ� ���
			{
				std::shared_ptr<T> res;
				res.swap(ptr->data);

				// external : 1 -> 2(��+1) -> 4(��+1 ��+2)
				// internal : 1 -> 0
				const int32 countIncrease = oldHead.externalCount - 2;

				if (ptr->internalCount.fetch_add(countIncrease) == -countIncrease)
					delete ptr;

				return res;
			}
			else if (ptr->internalCount.fetch_sub(1) == 1)
			{
				// �������� �������, �������� ���� -> �޼����� ���� �Ѵ�
				delete ptr;
			}
		}
	}

private:
	void IncreaseHeadCount(CountedNodePtr& oldCounter)
	{
		while (true)
		{
			CountedNodePtr newCounter = oldCounter;// old Counter�� �� �Լ��� ȣ�� �� ����� head���� ���
			newCounter.externalCount++;
			/*head�� top ����̹Ƿ� oldCounter�� �ٸ��ٸ�, �ٸ� �����尡 �߰��� ��带 pop�Ǵ� push�� ����̴�.
			* ��, head != oldCounter�� ��쿡�� oldCounter = head�� �ȴ�.(�� oldCounter�� ������ head�� ����Ų��)
			* ������ head == oldCounter�� ���, head = newCounter�� �Ǵµ�, �ѹ����� �������� ��쿡�� newCounter�� oldCounter�� ��ġ�Ѵ�.
			* �ѹ����� �������� �ʾ��� ��쿡��, oldCounter�� ������ head�� ���Ѵ�.
			* �׷� �ٽ� �ֱ��� ��尡 �״�� ��ȭ���� �ʾҴ����� Ȯ���ϴ� ������ �ݺ��ϴ� ���̴�.
			*/
			if (head.compare_exchange_strong(oldCounter, newCounter))
			{//head == oldCounter�� �����ϸ�, head = newCounter�� �ȴ�
				oldCounter.externalCount = newCounter.externalCount;
				break;
			}
		}
	}

private:
	std::atomic<CountedNodePtr> head; // ž ���
};

//������ Lock-free Stack
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
