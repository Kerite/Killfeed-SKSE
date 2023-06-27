#include "Papyrus.h"

#include "EventsHandler.h"
#include "Settings.h"

void Papyrus::Register()
{
	SKSE::GetPapyrusInterface()->Register(Papyrus::RegisterFunctions);
}

bool Papyrus::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm)
{
	a_vm->RegisterFunction("ToggleKillFeed", "KillFeed_Native", ToggleKillFeed);
	a_vm->RegisterFunction("OnConfigClose", "KillFeed_MCM", OnConfigClose);

	return true;
}

void Papyrus::ToggleKillFeed(RE::StaticFunctionTag*)
{
	EventsHandler::GetSingleton()->ToggleKillFeed();
}

void Papyrus::OnConfigClose(RE::TESQuest*)
{
	Settings::GetSingleton()->ReadSettings();
}