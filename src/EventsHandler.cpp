#include "EventsHandler.h"

using namespace RE;

std::string WideStringToString(const std::wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.length(), nullptr, 0, nullptr, nullptr);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.length(), &strTo[0], size_needed, nullptr, nullptr);
	return strTo;
}

std::string Translate(const wchar_t* key)
{
	auto loader = BSScaleformManager::GetSingleton()->loader;
	auto translator = loader->GetState<BSScaleformTranslator>(RE::GFxState::StateType::kTranslator);

	GFxTranslator::TranslateInfo info;
	GFxWStringBuffer buffer;
	info.key = key;
	info.result = &buffer;

	translator->Translate(&info);

	auto result = WideStringToString(buffer.c_str());
	return result;
}

BSEventNotifyControl EventsHandler::ProcessEvent(const TESDeathEvent* a_event, BSTEventSource<TESDeathEvent>*)
{
	if (a_event && a_event->actorDying) {
		auto& dying = a_event->actorDying;
		auto dyingFormID = dying->GetFormID();
		auto dyingLocation = dying->GetCurrentLocation();
		auto locationName = dyingLocation ? dyingLocation->GetFullName() : "??";

		auto& killer = a_event->actorKiller;

		std::string text;
		if (killer) {
			auto killerFormID = killer->GetFormID();
			text = fmt::vformat(Translate(L"$KillMessageFormID"), fmt::make_format_args(killer->GetName(), killerFormID, dying->GetName(), dyingFormID, locationName));
		} else {
			text = fmt::vformat(Translate(L"$DeathMessageFormID"), fmt::make_format_args(dying->GetName(), dyingFormID, locationName));
		}
		DebugNotification(fmt::format("<font color='#FF0000'>{}</font>", text).c_str());
	}
	return BSEventNotifyControl::kContinue;
}
