#pragma once
#include "Types.h"
#include "STL_Allocator.h"
#include <array>
#include <vector>
#include <list> 
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>

template<typename Type, uint32 Size>
using Array = std::array<Type, Size>;

#define _CustomAllocator

#ifdef _CustomAllocator	
template<typename Type>
using Vector = std::vector<Type, STL_Allocator<Type>>;

template<typename Type>
using List = std::list<Type, STL_Allocator<Type>>;

template <typename Type>
using Deque = std::deque<Type, STL_Allocator<Type>>;

template<typename Type, typename Container = Deque<Type>>
using Queue = std::queue<Type, Container>;

template<typename Key, typename Type, typename Pred = std::less<Key>>
using Map = std::map<Key, Type, Pred, STL_Allocator<std::pair<const Key, Type>>>;

template<typename Key, typename Pred = std::less<Key>>
using Set = std::set<Key, Pred, STL_Allocator<Key>>;

template<typename Type, typename Container = Deque<Type>>
using Stack = std::stack<Type, Container>;

template<typename Type, typename Container = Vector<Type>,
	typename Pred = std::less<typename Container::value_type>>
	using PriorityQueue = std::priority_queue<Type, Container, Pred>;

using String = std::basic_string<wchar_t, std::char_traits<wchar_t>, STL_Allocator<wchar_t>>;

template<typename Key, typename Type, typename Hasher = std::hash<Key>,
	typename KeyEq = std::equal_to<Key>>
	using HashMap = std::unordered_map<Key, Type, Hasher, KeyEq, STL_Allocator<std::pair<const Key, Type>>>;

template<typename Key, typename Hasher = std::hash<Key>,
	typename KeyEq = std::equal_to<Key>>
	using HashSet = std::unordered_set<Key, Hasher, KeyEq, STL_Allocator<Key>>;
#else
template<typename Type>
using Vector = std::vector<Type, std::allocator<Type>>;

template<typename Type>
using List = std::list<Type, std::allocator<Type>>;

template <typename Type>
using Deque = std::deque<Type, std::allocator<Type>>;

template<typename Type, typename Container = Deque<Type>>
using Queue = std::queue<Type, Container>;

template<typename Key, typename Type, typename Pred = std::less<Key>>
using Map = std::map<Key, Type, Pred, std::allocator<std::pair<const Key, Type>>>;

template<typename Key, typename Pred = std::less<Key>>
using Set = std::set<Key, Pred, std::allocator<Key>>;

template<typename Type, typename Container = Deque<Type>>
using Stack = std::stack<Type, Container>;

template<typename Type, typename Container = Vector<Type>,
	typename Pred = std::less<typename Container::value_type> >
	using PriorityQueue = std::priority_queue<Type, Container, Pred>;

using String = std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>;

template<typename Key, typename Type, typename Hasher = std::hash<Key>,
	typename KeyEq = std::equal_to<Key> >
	using HashMap =
	std::unordered_map<Key, Type, Hasher, KeyEq, std::allocator<std::pair<const Key, Type>>>;

template<typename Key, typename Hasher = std::hash<Key>,
	typename KeyEq = std::equal_to<Key>>
	using HashSet = std::unordered_set<Key, Hasher, KeyEq, std::allocator<Key>>;
#endif