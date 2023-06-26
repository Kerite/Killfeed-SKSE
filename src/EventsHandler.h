#pragma once

class EventsHandler final : public dku::model::Singleton<EventsHandler>, public RE::BSTEventSink<RE::TESDeathEvent> {
public:
	void Register()
	{
		auto dispatcher = RE::ScriptEventSourceHolder::GetSingleton();
		dispatcher->GetEventSource<RE::TESDeathEvent>()->AddEventSink(this);
	}

	// Inherited via BSTEventSink
	virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESDeathEvent*, RE::BSTEventSource<RE::TESDeathEvent>*) override;
};