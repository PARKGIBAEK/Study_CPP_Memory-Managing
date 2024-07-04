#pragma once
#include "../Memory/Container.h"
#include "../Core/CoreMacro.h"
#include "../Thread/Lock.h"

namespace ServerCore
{

struct false_t
{	static constexpr bool value = false; };

struct true_t
{	static constexpr bool value = true; };

template<typename T>
struct is_shared_ptr :false_t {};

template<typename T>
struct is_shared_ptr<std::shared_ptr<T>> : true_t {};

template<typename T>
class LockQueue
{
	// shared_ptr만 사용할 예정
	static_assert(is_shared_ptr<T>::value, "T must be shared_ptr");
public:
	void Push(T item)
	{
		WRITE_LOCK;
		_items.push(item);
	}

	T Pop()
	{
		WRITE_LOCK;
		if (_items.empty())
			return T();

		T ret = _items.front();
		_items.pop();
		return ret;
	}

	void PopAll(OUT Vector<T>& items)
	{
		WRITE_LOCK;
		while (T item = Pop())
			items.push_back(item);
	}

	void Clear()
	{
		WRITE_LOCK;
		_items = Queue<T>();
	}

private:
	USE_LOCK;
	Queue<T> _items;
};
}