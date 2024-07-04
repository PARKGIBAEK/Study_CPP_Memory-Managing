#pragma once
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include "../Core/Types.h"
#include "../Core/CoreMacro.h"

namespace ServerCore
{


class DeadLockProfiler
{
public:
	void PushLock(const char* _name);
	void PopLock(const char* _name);
	void CheckCycle();

private:
	void DFS(int32 _here);

private:
	std::unordered_map<const char*, int32>	nameToId;
	std::unordered_map<int32, const char*>	idToName;
	std::map<int32, std::set<int32>>				lockHistory;

	std::mutex mtxLock{};

private:
	std::vector<int32>	discoveredOrder; // 노드가 발견된 순서를 기록하는 배열
	int32			discoveredCount = 0; // 노드가 발견된 순서
	std::vector<bool>	finished; // Dfs(i)가 종료 되었는지 여부
	std::vector<int32>	parent;
	
};
}