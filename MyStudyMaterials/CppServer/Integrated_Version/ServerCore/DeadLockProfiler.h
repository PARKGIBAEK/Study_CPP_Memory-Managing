#pragma once
#include <stack>
#include <map>
#include <vector>
//#include "ConcurrentStack.h"

/*--------------------
	DeadLockProfiler
---------------------*/

class DeadLockProfiler
{
public:

	void PushLock(const char* _name);
	void PopLock(const char* _name);
	void CheckCycle();

private:
	void DFS(int32 _here);

private:
	unordered_map<const char*, int32>	nameToId;
	unordered_map<int32, const char*>	idToName;
	map<int32, set<int32>>				lockHistory;

	std::mutex mtxLock{};

private:
	vector<int32>	discoveredOrder; // 노드가 발견된 순서를 기록하는 배열
	int32			discoveredCount = 0; // 노드가 발견된 순서
	vector<bool>	finished; // Dfs(i)가 종료 되었는지 여부
	vector<int32>	parent;
	
};

