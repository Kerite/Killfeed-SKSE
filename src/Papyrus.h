#pragma once

class Papyrus
{
public:
	static void Register();
	static bool RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm);

	static void ToggleKillFeed(RE::StaticFunctionTag*);

	static void OnConfigClose(RE::TESQuest*);
};