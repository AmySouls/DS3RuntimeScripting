/*
	* DS3RuntimeScripting
	* Contributers: Amir
*/

#pragma once
#include "pch.h"

namespace ds3runtime::packet {

struct PacketField {
	std::string typeName;
	std::string name;
	uintptr_t offset;
	size_t size;
};

enum class id : int32_t {
	Status = 1,
	PlayerStruct = 8,
	Equipment = 12,
	MessageMapList = 14,
	Hit = 20,
	WorldFlag = 26,
	Effect = 36,
	Event = 49,
};

class Packet
{
public:
	Packet(int32_t id, char* buffer, uint32_t length);

	char* getData();

	uint32_t getLength();

	std::vector<char> getDataCopy();

	void copyToBuffer(char* buffer, uint32_t maxLength);

	virtual const int32_t getId()
	{
		return id;
	}

	std::string getStringField(std::string fieldName);

	std::wstring getWideStringField(std::string fieldName);

	template<class FieldType>
	FieldType getField(std::string fieldName)
	{
		for (auto field : *getFields()) {
			if (fieldName != field.name) continue;
			else if (sizeof(FieldType) != field.size) return 0;
			else if (field.offset + field.size > buffer.size()) return 0;
			return *(FieldType*)(&buffer[0] + field.offset);
		}

		return 0;
	}

	void setStringField(std::string fieldName, std::string value);

	void setWideStringField(std::string fieldName, std::wstring value);

	template<class FieldType>
	void setField(std::string fieldName, FieldType value)
	{
		for (auto field : *getFields()) {
			if (fieldName != field.name) continue;
			else if (sizeof(FieldType) != field.size) return;
			else if (field.offset + field.size > buffer.size()) return;
			*(FieldType*)(&buffer[0] + field.offset) = value;
			return;
		}
	}
protected:
	Packet();

	Packet(char* buffer, uint32_t length);

	Packet(std::vector<char> buffer);
private:
	int32_t id;
	std::vector<char> buffer;
	
	virtual const std::vector<PacketField>* getFields()
	{
		return nullptr;
	}
};

class Status : public Packet
{
public:
	Status() : Packet()
	{
	}

	Status(char* buffer, uint32_t length) : Packet(buffer, length)
	{
	}

	Status(std::vector<char> buffer) : Packet(buffer)
	{
	}

	const int32_t getId()
	{
		return 1;
	}
private:
	static std::vector<PacketField> fields;

	const std::vector<PacketField>* getFields();
};

class PlayerStruct : public Packet
{
public:
	PlayerStruct() : Packet()
	{
	}

	PlayerStruct(char* buffer, uint32_t length) : Packet(buffer, length)
	{
	}

	PlayerStruct(std::vector<char> buffer) : Packet(buffer)
	{
	}

	const int32_t getId()
	{
		return 8;
	}
private:
	static std::vector<PacketField> fields;

	const std::vector<PacketField>* getFields();
};

class Equipment : public Packet
{
public:
	Equipment() : Packet()
	{
	}

	Equipment(char* buffer, uint32_t length) : Packet(buffer, length)
	{
	}

	Equipment(std::vector<char> buffer) : Packet(buffer)
	{
	}

	const int32_t getId()
	{
		return 12;
	}
private:
	static std::vector<PacketField> fields;

	const std::vector<PacketField>* getFields();
};

class MessageMapList : public Packet
{
public:
	MessageMapList() : Packet()
	{
	}

	MessageMapList(char* buffer, uint32_t length) : Packet(buffer, length)
	{
	}

	MessageMapList(std::vector<char> buffer) : Packet(buffer)
	{
	}

	const int32_t getId()
	{
		return 14;
	}
private:
	static std::vector<PacketField> fields;

	const std::vector<PacketField>* getFields();
};

class Hit : public Packet
{
public:
	Hit() : Packet()
	{
	}

	Hit(char* buffer, uint32_t length) : Packet(buffer, length)
	{
	}

	Hit(std::vector<char> buffer) : Packet(buffer)
	{
	}

	const int32_t getId()
	{
		return 20;
	}
private:
	static std::vector<PacketField> fields;

	const std::vector<PacketField>* getFields();
};

class Effect : public Packet
{
public:
	Effect() : Packet()
	{
	}

	Effect(char* buffer, uint32_t length) : Packet(buffer, length)
	{
	}

	Effect(std::vector<char> buffer) : Packet(buffer)
	{
	}

	const int32_t getId()
	{
		return 36;
	}
private:
	static std::vector<PacketField> fields;

	const std::vector<PacketField>* getFields();
};

}