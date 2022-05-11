#pragma once
#include "Types.h"
#include "Allocator.h"
#include <array>
#include <vector>
#include <list> 
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
using namespace std;

template<typename Type, uint32 Size>
using Array = std::array<Type, Size>;

template<typename Type>
using Vector = std::vector<Type, STL_Allocator<Type>>;

template<typename Type>
using List = std::list<Type, STL_Allocator<Type>>;

template<typename Type>
using Deque = std::deque<Type, STL_Allocator<Type>>;

template<typename Type, typename Container = Deque<Type>>
using Queue = std::queue<Type, Container>;

template<typename Key, typename Type, typename Pred = less<Key>>
using Map = std::map<Key, Type, Pred, STL_Allocator<pair<const Key, Type>>>;

template<typename Key, typename Pred = less<Key>>
using Set = std::set<Key, Pred, STL_Allocator<Key>>;

template<typename Type, typename Container = Deque<Type>>
using Stack = std::stack<Type, Container>;

template<typename Type, typename Container = Vector<Type>, 
	typename Pred = less<typename Container::value_type>>
using PriorityQueue = std::priority_queue<Type, Container, Pred>;

using String = std::basic_string<wchar_t, char_traits<wchar_t>, STL_Allocator<wchar_t>>;

template<typename Key, typename Type, typename Hasher = hash<Key>,
	typename KeyEq = equal_to<Key>>
using HashMap = std::unordered_map<Key, Type, Hasher, KeyEq, STL_Allocator<pair<const Key, Type>>>;

template<typename Key, typename Hasher = hash<Key>, 
	typename KeyEq = equal_to<Key>>
using HashSet = std::unordered_set<Key, Hasher, KeyEq, STL_Allocator<Key>>;