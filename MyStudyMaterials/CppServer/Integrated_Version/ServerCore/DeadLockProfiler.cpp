#include "pch.h"
#include "DeadLockProfiler.h"


/*--------------------
	DeadLockProfiler
---------------------*/

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

	// ��� �ִ� ���� �־��ٸ�
	if (LLockStack.empty() == false)
	{
		// ������ �߰ߵ��� ���� ���̽���� ����� ���� �ٽ� Ȯ���Ѵ�.
		const int32 prevId = LLockStack.top();
		if (lockId != prevId)
		{
			set<int32>& history = lockHistory[prevId];
			if (history.find(lockId) == history.end())
			{
				history.insert(lockId);
				CheckCycle();
			}
		}
	}

	LLockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char* _name)
{
	LockGuard guard(mtxLock);

	if (LLockStack.empty())
		CRASH("MULTIPLE_UNLOCK");

	int32 lockId = nameToId[_name];
	if (LLockStack.top() != lockId)
		CRASH("INVALID_UNLOCK");

	LLockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	const int32 lockCount = static_cast<int32>(nameToId.size());
	discoveredOrder = vector<int32>(lockCount, -1);
	discoveredCount = 0;
	finished = vector<bool>(lockCount, false);
	parent = vector<int32>(lockCount, -1);

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

	// ��� ������ ������ ��ȸ�Ѵ�.
	auto findIt = lockHistory.find(_here);
	if (findIt == lockHistory.end())
	{
		finished[_here] = true;
		return;
	}

	set<int32>& nextSet = findIt->second;
	for (int32 there : nextSet)
	{
		// ���� �湮�� ���� ���ٸ� �湮�Ѵ�.
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
