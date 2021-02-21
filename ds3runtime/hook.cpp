#pragma once
#include "pch.h"
#include "hook.h"
#include "ds3runtime.h"

namespace ds3runtime {

Hook::Hook(uintptr_t original, uintptr_t replacement)
{
    this->original = new uintptr_t;
    *this->original = original;
    this->replacement = new uintptr_t;
    *this->replacement = replacement;
}

Hook::~Hook()
{
    delete original;
    delete replacement;
}

bool Hook::install()
{
    auto txn_status = DetourTransactionBegin();
    if (txn_status != NO_ERROR) throw std::runtime_error("Unable to create a Detours transaction");
    txn_status = DetourAttach((PVOID*)original, (PVOID)*replacement);
    if (txn_status != NO_ERROR) (void)DetourTransactionAbort();
    txn_status = DetourTransactionCommit();
    return txn_status == NO_ERROR;
}

bool Hook::uninstall()
{
    DetourTransactionBegin();
    DetourDetach((PVOID*)original, (PVOID)*replacement);
    DetourTransactionCommit();
    return true;
}

}