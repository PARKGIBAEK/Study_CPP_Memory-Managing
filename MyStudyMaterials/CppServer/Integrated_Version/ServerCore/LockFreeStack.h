#pragma once
#include "Types.h"
#include <memory>
//  ��� ����

/*============================================

	Lock-free Stack (Reference Count�� ���� �����Ͽ� ABA ���� �ذ��ϴ� ���)

============================================*/


template<typename T>
class LockFreeStack
{
	struct Node;

	struct CountedNodePtr
	{
		int32 externalCount = 0;// Push �ϸ� 1�� ����
		struct Node* ptr = nullptr;
	};

	struct Node
	{
		Node(const T& value) : data(make_shared<T>(value))
		{	}

		std::shared_ptr<T> data;
		std::atomic<int32> internalCount = 0;// �ٸ� Thread���� �� �̻� �������� �ʴ��� Ȯ���ϱ� ���� �뵵 ( TryPop�Լ����� �ǵ帲 )
		CountedNodePtr next;
	};

public:

	void Push(const T& value)
	{
		CountedNodePtr node;//���� ������ ��� externalCount�� 0���� ������
		node.ptr = new Node(value);
		node.externalCount = 1;//�ܺ� ���� ī��Ʈ 1�� ����

		node.ptr->next = head;// head�� ���� top ���
		/*head == node.NodePtr->next �� ���(�ٸ� �������� ������ ���� ���� ��Ȳ), head = node , true ��ȯ,
		  head != node.NodePtr->next �� ���(�ٸ� �������� ������ ���� ��Ȳ), node.NodePtr->next = head , false ��ȯ(���� �� �����ϰԵǰ� �̹��� �������� �ʾҴٸ� ���)*/
		while (head.compare_exchange_weak(node.ptr->next, node) == false)
		{
		}
	}

	std::shared_ptr<T> TryPop()
	{
		CountedNodePtr oldHead = head;//head ��� ��������
		while (true)
		{
			// ������ ȹ�� (externalCount�� �� ���� ���� +1 �� Thread�� �̱�)
			IncreaseHeadCount(oldHead);// IncreaseHeadCount�Լ��� externalCount�� 1������Ŵ

			// Push�� �� externalCount�� 1�� ����ǹǷ� externalCount�� �ּ��� 2�̻� ���״� ����X  (�����ϰ� ������ �� �ִ�)
			Node* ptr = oldHead.ptr;

			if (ptr == nullptr)// ������ ���� ��
				return std::shared_ptr<T>();

			// ������ ȹ�� (head�� NodePtr->next�� ������ Thread�� �̱�)
			if (head.compare_exchange_strong(oldHead, ptr->next))//ó������ head�� oldHead�� ���� ��� : ���� �ٸ� �����忡�� stack�� ���� ���� ���� ��Ȳ
			{
				std::shared_ptr<T> res;
				res.swap(ptr->data);

				// external(������ ȹ�� �� 1����) : 1 -> 2(��+1) -> 4(��+1 ��+2)
				// internal : 1 -> 0
				const int32 countIncrease = oldHead.externalCount - 2;

				if (ptr->internalCount.fetch_add(countIncrease) == -countIncrease)
					delete ptr;

				return res;
			}
			else if (ptr->internalCount.fetch_sub(1) == 1)//internalCount�� 1�̸� ���� ���� �����̹Ƿ� ����
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
			CountedNodePtr newCounter = oldCounter;
			newCounter.externalCount++;
			//head�� top ����̹Ƿ� oldCounter�� �ٸ��ٸ�, �ٸ� �����尡 �߰��� ��带 pop�Ǵ� push�� ���
			if (head.compare_exchange_strong(oldCounter, newCounter))
			{
				oldCounter.externalCount = newCounter.externalCount;
				break;
			}
		}
	}

private:
	std::atomic<CountedNodePtr> head; // ž ��带 ����Ŵ
};




/*============================================
 
	Lock-free Stack (Chain-Pending List ���)

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

	// 1) �� ��带 �����
	// 2) �� ����� next = head
	// 3) head = �� ���
	void Push(const T& value)
	{
		Node* node = new Node(value);//���ο� ��� ����
		node->next = head;//���ο� ��忡 ��� ����

		/*�߰��� �ٸ� �����尡 ������ head�� ����Ǿ��� �� �����Ƿ� Ȯ���ϱ�
		* head == node->next �� ��� (���� head�� ������� ����)head�� ���� ������ node�� ����Ű���� �����ϰ� ����.
		* head != node->next �� ��� (head�� ���� �Ȱ��̱� ������) node->next�� ����Ű�� ����� ���ο� head�� �ٲ��ְ� �ٽ� ������ ����.
		*/
		while (head.compare_exchange_weak(node->next, node) == false)
		{
		}
	}

	// 1) head �б�
	// 2) head->next �б�
	// 3) head = head->next
	// 4) data �����ؼ� ��ȯ
	// 5) ������ ��带 ����
	bool TryPop(T& value)
	{
		++popCount;

		Node* oldHead = head;
		/* head�� nullptr�� �ƴѰ��� Ȯ���ϰ�,
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

	// 1) ������ �и�
	// 2) Count üũ
	// 3) �� ȥ�ڸ� ����
	void TryDelete(Node* oldHead)
	{
		// �� �ܿ� ���� �ִ°�?
		if (popCount == 1)
		{
			// �� ȥ�ڳ�?

			// �̿� ȥ���ΰ�, ���� ����� �ٸ� �����͵鵵 �����غ���
			Node* node = pendingList.exchange(nullptr);//������ pendingList�� ����Ű�� �ּҸ� ��ȯ�ϰ�, pendingList�� nullptr�� ����Ű���� ����

			if (--popCount == 0)
			{
				// ����� �ְ� ���� -> ���� ����
				// �����ͼ� �����, ������ �����ؾ��� ������(pendingList)�� �и��ص� ����~!
				DeleteNodes(node);
			}
			else if (node)
			{
				// ���� ���������� �ٽ� ���� ����
				ChainPendingNodeList(node);
			}

			// �� �����͸� ����
			delete oldHead;
		}
		else
		{
			// ���� �ֳ�? �׷� ���� �������� �ʰ�, ���� ���ุ
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

	std::atomic<uint32> popCount = 0; // Pop�� �������� ������ ����
	std::atomic<Node*> pendingList; // ���� �Ǿ�� �� ���� (ù��° ���)
};