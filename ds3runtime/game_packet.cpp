#pragma once
#include "pch.h"
#include "game_packet.h"

namespace ds3runtime::packet {

Packet::Packet()
{
	id = -1;
}

Packet::Packet(int32_t id, char* buffer, uint32_t length)
{
	this->id = id;
	this->buffer = std::vector<char>(buffer, buffer + length);
}

Packet::Packet(char* buffer, uint32_t length)
{
	this->buffer = std::vector<char>(buffer, buffer + length);
}

Packet::Packet(std::vector<char> buffer)
{
	this->buffer = buffer;
}

char* Packet::getData()
{
	return &buffer[0];
}

uint32_t Packet::getLength()
{
	return buffer.size();
}

std::vector<char> Packet::getDataCopy()
{
	return buffer;
}

void Packet::copyToBuffer(char* buffer, uint32_t maxLength)
{
	memcpy(buffer, &this->buffer[0], std::min((uint32_t)this->buffer.size(), maxLength));
}

std::string Packet::getStringField(std::string fieldName)
{
	for (auto field : *getFields()) {
		if (fieldName != field.name) continue;
		else if (field.typeName != "char[]") return "";
		else if (field.offset + field.size > buffer.size()) return "";
		return std::string(&buffer[0] + field.offset, field.size);
	}

	return "";
}

std::wstring Packet::getWideStringField(std::string fieldName)
{
	for (auto field : *getFields()) {
		if (fieldName != field.name) continue;
		else if (field.typeName != "wchar_t[]") return L"";
		else if (field.offset + field.size > buffer.size()) return L"";
		return std::wstring((wchar_t*)(&buffer[0] + field.offset), field.size);
	}

	return L"";
}

void Packet::setStringField(std::string fieldName, std::string value)
{
	for (auto field : *getFields()) {
		if (fieldName != field.name) continue;
		else if (field.typeName != "char[]") return;
		else if (field.offset + field.size > buffer.size() || field.offset + field.size > value.size()) return;
		memcpy(&buffer[0] + field.offset, value.c_str(), value.size());
		return;
	}
}

void Packet::setWideStringField(std::string fieldName, std::wstring value)
{
	for (auto field : *getFields()) {
		if (fieldName != field.name) continue;
		else if (field.typeName != "wchar_t[]") return;
		//else if (field.offset + field.size > buffer.size() || field.offset + field.size > value.size() * 2) return;
		memcpy(&buffer[0] + field.offset, value.c_str(), value.size() * 2);
		return;
	}
}

std::vector<PacketField> Status::fields = {
	{ "float", "position_x", 0x10, 4 },
	{ "float", "position_y", 0x14, 4 },
	{ "float", "position_z", 0x18, 4 },
	{ "int16_t", "health_points", 0x28, 2 },
	{ "int16_t", "max_health_points", 0x2A, 2 },
};

const std::vector<PacketField>* Status::getFields()
{
	return &fields;
}

std::vector<PacketField> PlayerStruct::fields = {
	{ "int32_t", "player_number", 0, 4 },
	{ "int32_t", "unknown_1", 4, 4 },
	{ "int32_t", "unknown_2", 8, 4 },
	{ "int32_t", "multi_play_count", 0xC, 4 },
	{ "int32_t", "vigor", 0x2C, 4 },
	{ "int32_t", "attunement", 0x30, 4 },
	{ "int32_t", "endurance", 0x34, 4 },
	{ "int32_t", "strength", 0x38, 4 },
	{ "int32_t", "dexterity", 0x3C, 4 },
	{ "int32_t", "intelligence", 0x40, 4 },
	{ "int32_t", "faith", 0x44, 4 },
	{ "int32_t", "luck", 0x48, 4 },
	{ "int32_t", "vitality", 0x50, 4 },
	{ "int32_t", "soul_level", 0x54, 4 },
	{ "int8_t", "summon_type", 0x58, 1 },
	{ "wchar_t[]", "name", 0x5C, 32 },
	{ "wchar_t[]", "steam_id", 0x7D, 32 },
	{ "char[]", "face", 0xA0, 4 },
	{ "char[]", "face_data", 0xA4, 256 },
};

const std::vector<PacketField>* PlayerStruct::getFields()
{
	return &fields;
}

std::vector<PacketField> Equipment::fields = {
	{ "int32_t", "player_number", 0, 4 },
	{ "uint8_t", "gender", 4, 1 },
	{ "uint8_t", "voice", 6, 1 },
	{ "uint8_t", "covenant", 7, 1 },
	{ "int32_t", "left_weapon_1", 0xC, 4 },
	{ "int32_t", "right_weapon1", 0x10, 4 },
	{ "int32_t", "left_weapon_2", 0x14, 4 },
	{ "int32_t", "right_weapon_2", 0x18, 4 },
	{ "int32_t", "left_weapon_3", 0x1C, 4 },
	{ "int32_t", "right_weapon_3", 0x20, 4 },
	{ "int32_t", "arrow_1", 0x24, 4 },
	{ "int32_t", "bolt_1", 0x28, 4 },
	{ "int32_t", "arrow_2", 0x2C, 4 },
	{ "int32_t", "bolt_2", 0x30, 4 },
	{ "int32_t", "unused_arrow_3", 0x34, 4 },
	{ "int32_t", "unused_bolt_3", 0x38, 4 },
	{ "int32_t", "head", 0x3C, 4 },
	{ "int32_t", "chest", 0x40, 4 },
	{ "int32_t", "hands", 0x44, 4 },
	{ "int32_t", "legs", 0x48, 4 },
	{ "int32_t", "unused_hair", 0x4C, 4 },
	{ "int32_t", "ring_1", 0x50, 4 },
	{ "int32_t", "ring_2", 0x54, 4 },
	{ "int32_t", "ring_3", 0x58, 4 },
	{ "int32_t", "ring_4", 0x5C, 4 },
	{ "int32_t", "covenant_item", 0x60, 4 },
	/*
	{ "uint8_t", "left_weapon_1_ammo", 0x8C, 1 },
	{ "uint8_t", "right_weapon_1_ammo", 0x8D, 1 },
	{ "uint8_t", "left_weapon_2_ammo", 0x8E, 1 },
	{ "uint8_t", "right_weapon_2_ammo", 0x8F, 1 },
	{ "uint8_t", "left_weapon_3_ammo", 0x90, 1 },
	{ "uint8_t", "right_weapon_3_ammo", 0x91, 1 },
	*/
	{ "float", "head_proportion", 0x70, 4 },
	{ "float", "upper_body_proportion", 0x74, 4 },
	{ "float", "lower_body_proportion", 0x78, 4 },
	{ "float", "right_hand_proportion", 0x7C, 4 },
	{ "float", "right_leg_proportion", 0x80, 4 },
	{ "float", "left_hand_proportion", 0x84, 4 },
	{ "float", "left_leg_proportion", 0x88, 4 },
};

const std::vector<PacketField>* Equipment::getFields()
{
	return &fields;
}

std::vector<PacketField> MessageMapList::fields = {
	{ "int32_t", "message_id", 0, 4 },
	{ "int32_t", "event_id", 4, 4 },
	{ "int32_t", "param_1", 8, 4 },
	{ "int32_t", "param_2", 0xC, 4 },
	{ "int32_t", "param_3", 0x10, 4 },
};

const std::vector<PacketField>* MessageMapList::getFields()
{
	return &fields;
}

std::vector<PacketField> Hit::fields = {
	{ "float", "physical_damage", 0, 4 },
	{ "float", "magic_damage", 4, 4 },
	{ "float", "fire_damage", 8, 4 },
	{ "float", "lightning_damage", 0xC, 4 },
	{ "float", "dark_damage", 0x10, 4 },
	{ "uint16_t", "unknown_percent_1", 0x30, 2 },
	{ "uint16_t", "unknown_percent_2", 0x32, 2 },
	{ "uint16_t", "unknown_percent_3", 0x34, 2 },
	{ "uint16_t", "unknown_percent_4", 0x36, 2 },
	{ "uint16_t", "unknown_percent_5", 0x38, 2 },
	{ "float", "guard_damage", 0x3C, 4 },
	{ "int32_t", "atk_param_id", 0x44, 4 },
	{ "int32_t", "headshot_flag", 0x48, 4 },
	{ "int32_t", "unknown_flag", 0x74, 4 },
	{ "float", "position_x", 0x90, 4 },
	{ "float", "position_y", 0x94, 4 },
	{ "float", "position_z", 0x98, 4 },
};

const std::vector<PacketField>* Hit::getFields()
{
	return &fields;
}

std::vector<PacketField> Effect::fields = {
	{ "int32_t", "forward_id", 0, 4 },
	{ "int32_t", "unknown", 4, 4 },
	{ "int32_t", "effect_param_id", 8, 4 },
};

const std::vector<PacketField>* Effect::getFields()
{
	return &fields;
}

};