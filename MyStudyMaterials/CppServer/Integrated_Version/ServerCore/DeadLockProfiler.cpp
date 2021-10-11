#include "pch.h"
#include "DeadLockProfiler.h"


/*--------------------
	DeadLockProfiler
---------------------*/

void DeadLockProfiler::PushLock(const char* _name)
{
	LockGuard guard(mtxLock);

	// 아이디를 찾거나 발급한다.
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

	// 잡고 있는 락이 있었다면
	if (LLockStack.empty() == false)
	{
		// 기존에 발견되지 않은 케이스라면 데드락 여부 다시 확인한다.
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

	// 연산이 끝났으면 정리한다.
	discoveredOrder.clear();
	finished.clear();
	parent.clear();
}

void DeadLockProfiler::DFS(int32 _here)
{
	if (discoveredOrder[_here] != -1)
		return;

	discoveredOrder[_here] = discoveredCount++;

	// 모든 인접한 정점을 순회한다.
	auto findIt = lockHistory.find(_here);
	if (findIt == lockHistory.end())
	{
		finished[_here] = true;
		return;
	}

	set<int32>& nextSet = findIt->second;
	for (int32 there : nextSet)
	{
		// 아직 방문한 적이 없다면 방문한다.
		if (discoveredOrder[there] == -1)
		{
			parent[there] = _here;
			DFS(there);
			continue;
		}

		// here가 there보다 먼저 발견되었다면, there는 here의 후손이다. (순방향 간선)
		if (discoveredOrder[_here] < discoveredOrder[there])
			continue;

		// 순방향이 아니고, Dfs(there)가 아직 종료하지 않았다면, there는 here의 선조이다. (역방향 간선)
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
