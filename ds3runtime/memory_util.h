#pragma once
#include "pch.h"

namespace ds3runtime {

template<class ResultType, class PointerType>
ResultType* accessMultilevelPointer(PointerType baseAddress)
{
	return (intptr_t)baseAddress < 65536 || IsBadReadPtr((void*)((uintptr_t)baseAddress), sizeof(ResultType)) ? NULL : (ResultType*)baseAddress;
}

template<class ResultType, class PointerType, class OffsetType, class...OffsetTypes>
ResultType* accessMultilevelPointer(PointerType baseAddress, OffsetType offset, OffsetTypes... offsets)
{
	return (intptr_t)baseAddress < 65536 || IsBadReadPtr((void*)((uintptr_t)baseAddress), sizeof(void*)) ? NULL : accessMultilevelPointer<ResultType>(*(char**)((uintptr_t)baseAddress) + offset, offsets...);
}

}