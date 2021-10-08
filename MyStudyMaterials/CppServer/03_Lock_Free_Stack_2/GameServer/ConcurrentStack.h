#pragma once

#include <mutex>

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
	stack<T> _stack;
	mutex _mutex;
	condition_variable _condVar;
};

template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : data(value), next(nullptr)
		{

		}

		T data;
		Node* next;
	};

public:
	
	// 1) 새 노드를 만들고
	// 2) 새 노드의 next = head
	// 3) head = 새 노드
	void Push(const T& value)
	{
		Node* node = new Node(value);//새로운 노드 생성
		node->next = head;//새로운 노드에 헤드 연결

		/*중간에 다른 쓰레드가 끼어들어 head가 변경되었을 수 있으므로 확인하기
		* head == node->next 의 경우 (아직 head가 변경되지 않음)head가 새로 생성된 node를 가리키도록 변경하고 종료.
		* head != node->next 의 경우 (head가 변경 된것이기 때문에) node->next가 가리키는 대상을 새로운 head로 바꿔주고 다시 루프를 돈다.
		*/
		while (head.compare_exchange_weak(node->next, node) == false)
		{
		}
	}
	
	// 1) head 읽기
	// 2) head->next 읽기
	// 3) head = head->next
	// 4) data 추출해서 반환
	// 5) 추출한 노드를 삭제
	bool TryPop(T& value)
	{
		++popCount;

		Node* oldHead = head;
		/* head가 nullptr이 아닌것을 확인하고,
		* 
		*/

		while (oldHead && head.compare_exchange_weak(oldHead, oldHead->next) == false)
		{
		}

		if (oldHead == nullptr)
		{
			--popCount;
			return false;
		}

		value = oldHead->data;
		TryDelete(oldHead);
		return true;
	}
	
	// 1) 데이터 분리
	// 2) Count 체크
	// 3) 나 혼자면 삭제
	void TryDelete(Node* oldHead)
	{
		// 나 외에 누가 있는가?
		if (popCount == 1)
		{
			// 나 혼자네?

			// 이왕 혼자인거, 삭제 예약된 다른 데이터들도 삭제해보자
			Node* node = pendingList.exchange(nullptr);//기존의 pendingList가 가리키던 주소를 반환하고, pendingList는 nullptr을 가리키도록 변경

			if (--popCount == 0)
			{
				// 끼어든 애가 없음 -> 삭제 진행
				// 이제와서 끼어들어도, 어차피 삭제해야할 데이터(pendingList)는 분리해둔 상태~!
				DeleteNodes(node);
			}
			else if (node)
			{
				// 누가 끼어들었으니 다시 갖다 놓자
				ChainPendingNodeList(node);
			}

			// 내 데이터만 삭제
			delete oldHead;
		}
		else
		{
			// 누가 있네? 그럼 지금 삭제하지 않고, 삭제 예약만
			ChainPendingNode(oldHead);
			--popCount;
		}
	}

	void ChainPendingNodeList(Node* first, Node* last)
	{
		last->next = pendingList;

		while (pendingList.compare_exchange_weak(last->next, first) == false)
		{
		}
	}

	void ChainPendingNodeList(Node* node)
	{
		Node* last = node;
		while (last->next)
			last = last->next;

		ChainPendingNodeList(node, last);
	}

	void ChainPendingNode(Node* node)
	{
		ChainPendingNodeList(node, node);
	}

	static void DeleteNodes(Node* node)
	{
		while (node)
		{
			Node* next = node->next;
			delete node;
			node = next;
		}
	}

private:
	atomic<Node*> head;

	atomic<uint32> popCount = 0; // Pop을 실행중인 쓰레드 개수
	atomic<Node*> pendingList; // 삭제 되어야 할 노드들 (첫번째 노드)
};