#include "EventsHandler.h"

using namespace RE;

BSEventNotifyControl EventsHandler::ProcessEvent(const TESDeathEvent* a_event, BSTEventSource<TESDeathEvent>*)
{
	if (a_event && a_event->actorDying) {
		auto& dying = a_event->actorDying;
		if (auto& killer = a_event->actorKiller) {
			DebugNotification(fmt::format("{} killed {}", killer->GetName(), dying->GetName()).c_str());
		} else {
			DebugNotification(fmt::format("{} died", dying->GetName()).c_str());
		}
	}
	return BSEventNotifyControl::kContinue;
}
