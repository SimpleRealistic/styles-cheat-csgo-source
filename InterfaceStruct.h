#pragma once

#include "SDK.h"

class Interface
{
public:
	template< typename Function > inline Function VFunc(int Index)
	{
		PDWORD* VTablePointer = (PDWORD*)this;
		PDWORD VTableFunctionBase = *VTablePointer;
		DWORD dwAddress = VTableFunctionBase[Index];
		return (Function)(dwAddress);
	}
};