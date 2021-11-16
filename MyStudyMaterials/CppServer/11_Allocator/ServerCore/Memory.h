#pragma once
#include "Allocator.h"

template<typename Type, typename... Args>
Type* xNew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xAlloc(sizeof(Type)));
	new(memory)Type(forward<Args>(args)...); // placement new
	return memory;
}

template<typename Type>
void xDelete(Type* obj)
{
	obj->~Type(); // 소멸자 직접 호출
	xRelease(obj);
}