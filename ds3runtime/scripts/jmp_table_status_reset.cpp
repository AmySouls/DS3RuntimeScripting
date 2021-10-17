#pragma once
#include "pch.h"
#include "jmp_table_status_reset.h"
#include "ds3runtime/hooks/jmp_table_status.h"

namespace ds3runtime {

bool JmpTableStatusReset::onAttach()
{
	return true;
}

void JmpTableStatusReset::execute()
{
	reinterpret_cast<JmpTableStatus*>(ds3runtime_global->accessHook("jmp_table_status"))->tickJmpTableStatus();
}

bool JmpTableStatusReset::onDetach()
{
	return true;
}

}