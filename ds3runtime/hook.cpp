#pragma once
#include "pch.h"
#include "hook.h"
#include "ds3runtime.h"

namespace ds3runtime {

Hook::Hook(uintptr_t originalFunc, uintptr_t replacementFunc, std::vector<std::string> dependencies) 
    : original(std::make_unique<uintptr_t>(originalFunc)),
      replacement(std::make_unique<uintptr_t>(replacementFunc)),
      dependencies(dependencies)
{
}

bool Hook::install()
{
    auto txn_status = DetourTransactionBegin();
    if (txn_status != NO_ERROR) throw std::runtime_error("Unable to create a Detours transaction");
    txn_status = DetourAttach(reinterpret_cast<PVOID*>(original.get()), reinterpret_cast<PVOID>(*replacement.get()));
    if (txn_status != NO_ERROR) DetourTransactionAbort();
    txn_status = DetourTransactionCommit();
    if (txn_status == NO_ERROR) installed = true;
    return txn_status == NO_ERROR;
}

bool Hook::uninstall()
{
    auto txn_status = DetourTransactionBegin();
    if (txn_status != NO_ERROR) throw std::runtime_error("Unable to create a Detours transaction");
    txn_status = DetourDetach(reinterpret_cast<PVOID*>(original.get()), reinterpret_cast<PVOID>(*replacement.get()));
    if (txn_status != NO_ERROR) DetourTransactionAbort();
    txn_status = DetourTransactionCommit();
    if (txn_status == NO_ERROR) installed = false;
    return txn_status == NO_ERROR;
}

std::vector<std::string> Hook::getDependencies()
{
    return dependencies;
}

bool Hook::isInstalled()
{
    return installed;
}

uintptr_t* Hook::getOriginal()
{
    return original.get();
}

}
