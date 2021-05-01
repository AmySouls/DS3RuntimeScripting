/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"
#include "ds3runtime/script_module.h"
#include "ds3runtime/ds3runtime.h"

namespace ds3runtime {

struct ParamPatchInfo
{
	std::string patchId;
	uint64_t value;
	size_t size;
};

struct ParamField
{
	std::string fieldName;
	std::string fieldType;
	uint64_t defaultValue;
	uint64_t bitOffset;
};

class ParamPatcher : public ScriptModule
{
public:
	ParamPatcher()
	{
		paramOffsetTable = std::unordered_map<std::wstring, uintptr_t>();
		paramIdTables = std::unordered_map<std::wstring, std::unordered_map<int32_t, uintptr_t>>();
		patchMap = std::unordered_map<std::wstring, std::unordered_map<int32_t, std::unordered_map<uintptr_t, std::vector<ParamPatchInfo>>>>();
		binaryPatchMap = std::unordered_map<std::wstring, std::unordered_map<int32_t, std::unordered_map<uintptr_t, std::unordered_map<uint8_t, std::vector<ParamPatchInfo>>>>>();
	}

	void execute();

	void onAttach();

	void onDetach();

	std::string getName()
	{
		return "param_patcher";
	}

	template<class FieldType>
	FieldType read(std::wstring param, int32_t id, uintptr_t offset)
	{
		return *accessMultilevelPointer<FieldType>(paramIdTables[param][id] + offset);
	}

	bool readBinary(std::wstring param, int32_t id, uintptr_t offset, uint8_t binaryOffset);

	template<class FieldType>
	FieldType readOriginal(std::wstring param, int32_t id, uintptr_t offset)
	{
		std::vector<ParamPatchInfo>* patchList = &patchMap[param][id][offset];
		if (patchList->size() == 0) return *accessMultilevelPointer<FieldType>(paramIdTables[param][id] + offset);
		else return (FieldType)patchList->at(0).value;
	}

	bool readBinaryOriginal(std::wstring param, int32_t id, uintptr_t offset, uint8_t binaryOffset);

	void patch(std::wstring param, int32_t id, uintptr_t offset, ParamPatchInfo patch);
	
	void patchBinary(std::wstring param, int32_t id, uintptr_t offset, uint8_t binaryOffset, ParamPatchInfo patch);

	void restore(std::wstring param, int32_t id, uintptr_t offset, std::string patchId);

	void restoreBinary(std::wstring param, int32_t id, uintptr_t offset, uint8_t binaryOffset, std::string patchId);

	void restore(std::string patchId);

	std::vector<ParamField> getParamLayout(std::wstring paramName);
private:
	std::unordered_map<std::wstring, uintptr_t> paramOffsetTable;
	std::unordered_map<std::wstring, std::unordered_map<int32_t, uintptr_t>> paramIdTables;
	std::unordered_map<std::wstring, std::unordered_map<int32_t, std::unordered_map<uintptr_t, std::vector<ParamPatchInfo>>>> patchMap;
	std::unordered_map<std::wstring, std::unordered_map<int32_t, std::unordered_map<uintptr_t, std::unordered_map<uint8_t, std::vector<ParamPatchInfo>>>>> binaryPatchMap;
	std::unordered_map<std::wstring, std::vector<ParamField>> paramLayoutTable;
	static std::unordered_map<std::wstring, std::wstring> paramLayoutFileTable;

	std::unordered_map<int32_t, uintptr_t> createParamIdTable(uintptr_t paramPointer);

	uintptr_t getIdAddress(const std::unordered_map<int32_t, uintptr_t>& idTable, int int32_t);

	int32_t getIdFromAddress(const std::unordered_map<int32_t, uintptr_t>& idTable, uintptr_t address);
};

class ParamHandler
{
public:
	ParamHandler(std::string patchId, std::wstring param, int32_t id);

	template<class FieldType>
	FieldType read(uintptr_t offset)
	{
		return (FieldType)((ParamPatcher*)ds3runtime_global->accessScript("param_patcher").get())->read(param, id, offset);
	}

	bool readBinary(uintptr_t offset, uint8_t binaryOffset);

	template<class FieldType>
	FieldType readOriginal(uintptr_t offset)
	{
		return (FieldType)((ParamPatcher*)ds3runtime_global->accessScript("param_patcher").get())->readOriginal(param, id, offset);
	}

	bool readBinaryOriginal(uintptr_t offset, uint8_t binaryOffset);

	template<class FieldType>
	void patch(uintptr_t offset, FieldType value)
	{
		ParamPatchInfo patch = {};
		patch.patchId = patchId;
		memcpy(&patch.value, &value, sizeof(value));
		patch.size = sizeof(value);
		((ParamPatcher*)ds3runtime_global->accessScript("param_patcher").get())->patch(param, id, offset, patch);
	}

	template<class FieldType>
	void patch(std::string fieldName, FieldType value)
	{
		for (ParamField field : ((ParamPatcher*)ds3runtime_global->accessScript("param_patcher").get())->getParamLayout(param)) {
			if (field.fieldName != fieldName) continue;
			patch(field.bitOffset / 8, value);
			break;
		}
	}

	void patchBinary(uintptr_t offset, int8_t binaryOffset, bool value);

	void patchBinary(std::string fieldName, bool value);
private:
	std::string patchId;
	std::wstring param;
	int32_t id;
};

}