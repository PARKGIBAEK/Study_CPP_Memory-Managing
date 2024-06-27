#pragma once
#include "Types.h"
#include <memory>
//  사용 안함

/*============================================

	Lock-free Stack (Reference Count를 직접 구현하여 ABA 문제 해결하는 방식)

============================================*/


template<typename T>
class LockFreeStack
{
	struct Node;

	struct CountedNodePtr
	{
		int32 externalCount = 0;// Push 하면 1로 시작
		struct Node* ptr = nullptr;
	};

	struct Node
	{
		Node(const T& value) : data(make_shared<T>(value))
		{	}

		std::shared_ptr<T> data;
		std::atomic<int32> internalCount = 0;// 다른 Thread들이 더 이상 접근하지 않는지 확인하기 위한 용도 ( TryPop함수에서 건드림 )
		CountedNodePtr next;
	};

public:

	void Push(const T& value)
	{
		CountedNodePtr node;//새로 삽입할 노드 externalCount는 0으로 시작함
		node.ptr = new Node(value);
		node.externalCount = 1;//외부 참조 카운트 1로 생성

		node.ptr->next = head;// head는 가장 top 노드
		/*head == node.NodePtr->next 일 경우(다른 스레드의 간섭을 받지 않은 상황), head = node , true 반환,
		  head != node.NodePtr->next 일 경우(다른 스레드의 간섭을 받은 상황), node.NodePtr->next = head , false 반환(루프 재 진입하게되고 이번에 간섭받지 않았다면 통과)*/
		while (head.compare_exchange_weak(node.ptr->next, node) == false)
		{
		}
	}

	std::shared_ptr<T> TryPop()
	{
		CountedNodePtr oldHead = head;//head 노드 꺼내오기
		while (true)
		{
			// 참조권 획득 (externalCount를 현 시점 기준 +1 한 Thread가 이김)
			IncreaseHeadCount(oldHead);// IncreaseHeadCount함수는 externalCount를 1증가시킴

			// Push할 때 externalCount가 1로 변경되므로 externalCount는 최소한 2이상 일테니 삭제X  (안전하게 접근할 수 있는)
			Node* ptr = oldHead.ptr;

			if (ptr == nullptr)// 데이터 없을 시
				return std::shared_ptr<T>();

			// 소유권 획득 (head를 NodePtr->next로 변경한 Thread가 이김)
			if (head.compare_exchange_strong(oldHead, ptr->next))//처음부터 head와 oldHead가 같은 경우 : 아직 다른 스레드에서 stack에 손을 대지 않은 상황
			{
				std::shared_ptr<T> res;
				res.swap(ptr->data);

				// external(참조권 획득 시 1증가) : 1 -> 2(나+1) -> 4(나+1 남+2)
				// internal : 1 -> 0
				const int32 countIncrease = oldHead.externalCount - 2;

				if (ptr->internalCount.fetch_add(countIncrease) == -countIncrease)
					delete ptr;

				return res;
			}
			else if (ptr->internalCount.fetch_sub(1) == 1)//internalCount가 1이면 나만 남은 상태이므로 삭제
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
			CountedNodePtr newCounter = oldCounter;
			newCounter.externalCount++;
			//head는 top 노드이므로 oldCounter와 다르다면, 다른 쓰레드가 중간에 노드를 pop또는 push한 경우
			if (head.compare_exchange_strong(oldCounter, newCounter))
			{
				oldCounter.externalCount = newCounter.externalCount;
				break;
			}
		}
	}

private:
	std::atomic<CountedNodePtr> head; // 탑 노드를 가리킴
};




/*============================================
 
	Lock-free Stack (Chain-Pending List 방식)

============================================*/



template<typename T>
class LockFreeStack_ChainPending
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
	std::atomic<Node*> head;

	std::atomic<uint32> popCount = 0; // Pop을 실행중인 쓰레드 개수
	std::atomic<Node*> pendingList; // 삭제 되어야 할 노드들 (첫번째 노드)
};