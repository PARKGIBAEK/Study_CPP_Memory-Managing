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
	vector<int32>	discoveredOrder; // ��尡 �߰ߵ� ������ ����ϴ� �迭
	int32			discoveredCount = 0; // ��尡 �߰ߵ� ����
	vector<bool>	finished; // Dfs(i)�� ���� �Ǿ����� ����
	vector<int32>	parent;
	
};

