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
	obj->~Type(); // �Ҹ��� ���� ȣ��
	xRelease(obj);
}