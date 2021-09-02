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
	void execute();

	bool onAttach();

	bool onDetach();

	std::string getName()
	{
		return "param_patcher";
	}

	bool doesIdExistInParam(const std::wstring& param, const int32_t& id);

	template<class FieldType>
	FieldType read(const std::wstring& param, const int32_t& id, const uintptr_t& offset)
	{
		return *accessMultilevelPointer<FieldType>(paramIdTables[param][id] + offset);
	}

	bool readBinary(const std::wstring& param, const int32_t& id, const uintptr_t& offset, const uint8_t& binaryOffset);

	template<class FieldType>
	FieldType readOriginal(const std::wstring& param, const int32_t& id, const uintptr_t& offset)
	{
		std::vector<ParamPatchInfo>* patchList = &patchMap[param][id][offset];
		if (patchList->size() == 0) return *accessMultilevelPointer<FieldType>(paramIdTables[param][id] + offset);
		else return (FieldType)patchList->at(0).value;
	}

	bool readBinaryOriginal(const std::wstring &param, const int32_t& id, const uintptr_t& offset, const uint8_t& binaryOffset);

	bool isValidParam(const std::wstring& param, const int32_t& id);

	void patch(const std::wstring& param, const int32_t& id, const uintptr_t& offset, const ParamPatchInfo& patch);
	
	void patchBinary(const std::wstring& param, const int32_t& id, const uintptr_t& offset, const uint8_t& binaryOffset, const ParamPatchInfo& patch);

	/**
	* Restores the specified param field to it's value before the specified patch id.
	* 
	* @param param The param type of the patch to restore.
	* @param id The id of the param of the patchId to restore.
	* @param offset The offset of the param field of the patch to restore.
	* @param patchId The patch id to revert the changes of.
	*/
	void restore(const std::wstring& param, const int32_t& id, const uintptr_t& offset, const std::string& patchId);

	void restoreBinary(const std::wstring& param, const int32_t& id, const uintptr_t& offset, const uint8_t& binaryOffset, const std::string& patchId);

	/**
	* Restores all params patched under the specified patch id to their unpatched values.
	* 
	* @param patchId The patch id to revert the changes of.
	*/
	void restore(const std::string& patchId);

	/**
	* Gets a list of fields belonging to the specified param type.
	* 
	* @param paramName The name of the param type to get the layout of.
	* @return List of fields within this param type.
	*/
	std::vector<ParamField> getParamLayout(const std::wstring& paramName);

	/**
	* Gets a list of all ids in this param
	* 
	* @param paramName The name of the param type to get the ids of.
	* @return List of ids in a param.
	*/
	std::vector<int32_t> getIdsInParam(const std::wstring& paramName);
private:
	std::unordered_map<std::wstring, uintptr_t> paramOffsetTable;
	std::unordered_map<std::wstring, std::unordered_map<int32_t, uintptr_t>> paramIdTables;
	std::unordered_map<std::wstring, std::unordered_map<int32_t, std::unordered_map<uintptr_t, std::vector<ParamPatchInfo>>>> patchMap;
	std::unordered_map<std::wstring, std::unordered_map<int32_t, std::unordered_map<uintptr_t, std::unordered_map<uint8_t, std::vector<ParamPatchInfo>>>>> binaryPatchMap;
	std::unordered_map<std::wstring, std::vector<ParamField>> paramLayoutTable;
	static std::unordered_map<std::wstring, std::wstring> paramLayoutFileTable;

	/**
	* Helper function for creating an id-address table that maps out the ids of the specified param.
	* 
	* @param param Pointer Pointer to the base of a param catagory(Eg. SpEffectParam).
	* @return The complete map of ids to addesses of the specifed param.
	*/
	std::unordered_map<int32_t, uintptr_t> createParamIdTable(const uintptr_t& paramPointer);

	/**
	* Helper function for finding the address of a param by it's id.
	* 
	* @param id The param id to find the address of.
	* @return The address of the specified param.
	*/
	uintptr_t getIdAddress(const std::unordered_map<int32_t, uintptr_t>& idTable, const int32_t& id);

	/**
	* Helper function for finding the id of a param by it's address.
	* 
	* @param addess The address of the param to find the id of.
	* @return The id of the specified param.
	*/
	int32_t getIdFromAddress(const std::unordered_map<int32_t, uintptr_t>& idTable, const uintptr_t& address);
};

class ParamHandler
{
public:
	ParamHandler(const std::string& patchId, const std::wstring& param, const int32_t& id);

	bool isValidParam() const;

	template<class FieldType>
	FieldType read(const uintptr_t& offset) const
	{
		return (dynamic_cast<ParamPatcher*>(ds3runtime_global->accessScript("param_patcher")))
			->read<FieldType>(param, id, offset);
	}

	template<class FieldType>
	FieldType read(const std::string& fieldName) const
	{
		for (ParamField field : (dynamic_cast<ParamPatcher*>(ds3runtime_global->accessScript("param_patcher")))
				->getParamLayout(param)) {
			if (field.fieldName != fieldName) continue;
			return (dynamic_cast<ParamPatcher*>(ds3runtime_global->accessScript("param_patcher")))
				->read<FieldType>(param, id, field.bitOffset / 8u);
		}
	}

	bool readBinary(const uintptr_t& offset, const uint8_t& binaryOffset) const;

	template<class FieldType>
	FieldType readOriginal(const uintptr_t& offset) const
	{
		return dynamic_cast<ParamPatcher*>(ds3runtime_global->accessScript("param_patcher"))
			->readOriginal<FieldType>(param, id, offset);
	}
	
	bool readBinaryOriginal(const uintptr_t& offset, const uint8_t& binaryOffset) const;

	template<class FieldType>
	void patch(const uintptr_t& offset, const FieldType& value)
	{
		ParamPatchInfo patch = {};
		patch.patchId = patchId;
		memcpy(&patch.value, &value, sizeof(value));
		patch.size = sizeof(value);
		(dynamic_cast<ParamPatcher*>(ds3runtime_global->accessScript("param_patcher")))
			->patch(param, id, offset, patch);
	}

	template<class FieldType>
	void patch(const std::string& fieldName, const FieldType& value)
	{
		for (ParamField field : (dynamic_cast<ParamPatcher*>(ds3runtime_global->accessScript("param_patcher")))
				->getParamLayout(param)) {
			if (field.fieldName != fieldName) continue;
			patch<FieldType>(field.bitOffset / 8u, value);
			break;
		}
	}

	void patchBinary(const uintptr_t& offset, const int8_t& binaryOffset, const bool& value);

	void patchBinary(const std::string& fieldName, const bool& value);
private:
	std::string patchId;
	std::wstring param;
	int32_t id;
};

}
