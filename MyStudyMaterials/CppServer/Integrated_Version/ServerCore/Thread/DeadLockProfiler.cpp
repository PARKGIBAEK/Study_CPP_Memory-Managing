#include "DeadLockProfiler.h"

#include "../Core/CoreMacro.h"
#include "../Core/CoreTLS.h"


namespace ServerCore
{


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

	// 현재 쓰레드에서 잡고 있는 락이 있었다면
	if (tls_LockStack.empty() == false)
	{
		// 잡고 있는 락 중에서 직전에 잡은 락의 ID 조회
		const int32 prevId = tls_LockStack.top();
		// 기존에 발견되지 않은 케이스라면 데드락 여부 다시 확인한다.
		if (lockId != prevId)// 지금 잡으려는 락이 바로 직전에 잡은 락이 아닌경우
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

	// 모든 인접한 정점을 순회하기
	auto findIt = lockHistory.find(_here);
	if (findIt == lockHistory.end())
	{
		finished[_here] = true;
		return;
	}

	std::set<int32>& nextSet = findIt->second;
	for (int32 there : nextSet)
	{
		// 아직 방문한 적이 없는 정점이라면 방문하기
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
}