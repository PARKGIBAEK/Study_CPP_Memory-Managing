#include "DeadLockProfiler.h"

#include "../Core/CoreMacro.h"
#include "../Core/CoreTLS.h"


namespace ServerCore
{


void DeadLockProfiler::PushLock(const char* _name)
{
	LockGuard guard(mtxLock);

	// ���̵� ã�ų� �߱��Ѵ�.
	int32 lockId = 0;

	auto findIt = nameToId.find(_name);
	if (findIt == nameToId.end())
	{
		lockId = static_cast<int32>(nameToId.size());
		nameToId[_name] = lockId;
		idToName[lockId] = _name;
	}
	else
	{
		lockId = findIt->second;
	}

	// ���� �����忡�� ��� �ִ� ���� �־��ٸ�
	if (tls_LockStack.empty() == false)
	{
		// ��� �ִ� �� �߿��� ������ ���� ���� ID ��ȸ
		const int32 prevId = tls_LockStack.top();
		// ������ �߰ߵ��� ���� ���̽���� ����� ���� �ٽ� Ȯ���Ѵ�.
		if (lockId != prevId)// ���� �������� ���� �ٷ� ������ ���� ���� �ƴѰ��
		{
			std::set<int32>& history = lockHistory[prevId];
			if (history.find(lockId) == history.end())
			{
				history.insert(lockId);
				CheckCycle();
			}
		}
	}

	tls_LockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char* _name)
{
	LockGuard guard(mtxLock);

	if (tls_LockStack.empty())
		CRASH("MULTIPLE_UNLOCK");

	int32 lockId = nameToId[_name];
	if (tls_LockStack.top() != lockId)
		CRASH("INVALID_UNLOCK");

	tls_LockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	const int32 lockCount = static_cast<int32>(nameToId.size());
	discoveredOrder = std::vector<int32>(lockCount, -1);
	discoveredCount = 0;
	finished = std::vector<bool>(lockCount, false);
	parent = std::vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; lockId++)
		DFS(lockId);

	// ������ �������� �����Ѵ�.
	discoveredOrder.clear();
	finished.clear();
	parent.clear();
}

void DeadLockProfiler::DFS(int32 _here)
{
	if (discoveredOrder[_here] != -1)
		return;

	discoveredOrder[_here] = discoveredCount++;

	// ��� ������ ������ ��ȸ�ϱ�
	auto findIt = lockHistory.find(_here);
	if (findIt == lockHistory.end())
	{
		finished[_here] = true;
		return;
	}

	std::set<int32>& nextSet = findIt->second;
	for (int32 there : nextSet)
	{
		// ���� �湮�� ���� ���� �����̶�� �湮�ϱ�
		if (discoveredOrder[there] == -1)
		{
			parent[there] = _here;
			DFS(there);
			continue;
		}

		// here�� there���� ���� �߰ߵǾ��ٸ�, there�� here�� �ļ��̴�. (������ ����)
		if (discoveredOrder[_here] < discoveredOrder[there])
			continue;

		// �������� �ƴϰ�, Dfs(there)�� ���� �������� �ʾҴٸ�, there�� here�� �����̴�. (������ ����)
		if (finished[there] == false)
		{
			printf("%s -> %s\n", idToName[_here], idToName[there]);

			int32 now = _here;
			while (true)
			{
				printf("%s -> %s\n", idToName[parent[now]], idToName[now]);
				now = parent[now];
				if (now == there)
					break;
			}

			CRASH("DEADLOCK_DETECTED");
		}
	}

	finished[_here] = true;
}
}