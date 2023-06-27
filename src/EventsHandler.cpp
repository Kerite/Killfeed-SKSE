#include "EventsHandler.h"

#include "Settings.h"

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
	if (!a_event || !m_killFeedEnabled || !a_event->actorDying) {
		return BSEventNotifyControl::kContinue;
	}

	auto& dying = a_event->actorDying;

	if (Settings::OnlyUniqueNPC) {
		// Check the dying npc is unique
		RE::Actor* actor = dying->As<Actor>();
		RE::TESNPC* dyingNpc = actor ? actor->GetActorBase() : nullptr;
		if (dyingNpc && !dyingNpc->IsUnique()) {
			return BSEventNotifyControl::kContinue;
		}
	}
	auto dyingFormID = dying->GetFormID();
	auto dyingLocation = dying->GetCurrentLocation();
	auto locationName = dyingLocation ? dyingLocation->GetFullName() : "???";

	auto& killer = a_event->actorKiller;

	std::string text;
	if (killer) {
		auto killerFormID = killer->GetFormID();
		DEBUG("{} [FormID: 0x{:X}] killed by {} [FormID: 0x{:X}] at {}", dying->GetName(), dyingFormID, killer->GetName(), killerFormID, locationName);
		if (Settings::ShowFormID) {
			text = fmt::vformat(Translate(L"$KillMessageFormID"), fmt::make_format_args(killer->GetName(), killerFormID, dying->GetName(), dyingFormID, locationName));
		} else {
			text = fmt::vformat(Translate(L"$KillMessage"), fmt::make_format_args(killer->GetName(), dying->GetName(), locationName));
		}
	} else {
		DEBUG("{} [FormID: 0x{:X}] died at {}", dying->GetName(), dyingFormID, locationName);
		if (Settings::ShowFormID) {
			text = fmt::vformat(Translate(L"$KillMessageFormID"), fmt::make_format_args(dying->GetName(), dyingFormID, locationName));
		} else {
			text = fmt::vformat(Translate(L"$KillMessage"), fmt::make_format_args(dying->GetName(), locationName));
		}
		text = fmt::vformat(Translate(L"$DeathMessageFormID"), fmt::make_format_args(dying->GetName(), dyingFormID, locationName));
	}

	DebugNotification(fmt::format("<font color='#{}'>{}</font>", Settings::KillFeedColor, text).c_str(), Settings::PlaySoundFX ? Settings::KillFeedSoundFx.c_str() : 0);
	return BSEventNotifyControl::kContinue;
}

BSEventNotifyControl EventsHandler::ProcessEvent(InputEvent* const* a_event, BSTEventSource<InputEvent*>* a_eventSource)
{
	if (!a_event) {
		return BSEventNotifyControl::kContinue;
	}
	auto controlMap = ControlMap::GetSingleton();
	if (!controlMap || controlMap->textEntryCount) {
		return BSEventNotifyControl::kContinue;
	}

	for (auto inputEvent = *a_event; inputEvent; inputEvent = inputEvent->next) {
		if (const auto button = inputEvent->AsButtonEvent()) {
			if (button->IsDown()) {
				auto key = button->idCode;
				if (key == Settings::KillFeedToggleKey) {
					ToggleKillFeed();
				}
			}
		}
	}

	return BSEventNotifyControl::kContinue;
}

void EventsHandler::ToggleKillFeed()
{
	m_killFeedEnabled = !m_killFeedEnabled;
	if (m_killFeedEnabled) {
		DebugNotification("$KillFeed_MessageEnabled");
	} else {
		DebugNotification("$KillFeed_MessageDisabled");
	}
}