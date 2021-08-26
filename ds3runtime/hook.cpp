#pragma once
#include "pch.h"
#include "hook.h"
#include "ds3runtime.h"

namespace ds3runtime {

Hook::Hook(uintptr_t originalFunc, uintptr_t replacementFunc) 
    : original(std::make_unique<uintptr_t>(originalFunc)),
      replacement(std::make_unique<uintptr_t>(replacementFunc))
{
}

bool Hook::install()
{
    auto txn_status = DetourTransactionBegin();
    if (txn_status != NO_ERROR) throw std::runtime_error("Unable to create a Detours transaction");
    txn_status = DetourAttach(reinterpret_cast<PVOID*>(original.get()), reinterpret_cast<PVOID>(*replacement.get()));
    if (txn_status != NO_ERROR) DetourTransactionAbort();
    txn_status = DetourTransactionCommit();
    return txn_status == NO_ERROR;
}

bool Hook::uninstall()
{
    auto txn_status = DetourTransactionBegin();
    if (txn_status != NO_ERROR) throw std::runtime_error("Unable to create a Detours transaction");
    txn_status = DetourDetach(reinterpret_cast<PVOID*>(original.get()), reinterpret_cast<PVOID>(*replacement.get()));
    if (txn_status != NO_ERROR) DetourTransactionAbort();
    txn_status = DetourTransactionCommit();
    return txn_status == NO_ERROR;
}

uintptr_t* Hook::getOriginal()
{
    return original.get();
}

}
