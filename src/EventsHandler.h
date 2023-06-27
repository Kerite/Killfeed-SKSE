#pragma once

class EventsHandler final :
	public dku::model::Singleton<EventsHandler>,
	public RE::BSTEventSink<RE::TESDeathEvent>,
	public RE::BSInputDeviceManager::Sink
{
public:
	void Register()
	{
		auto dispatcher = RE::ScriptEventSourceHolder::GetSingleton();
		dispatcher->GetEventSource<RE::TESDeathEvent>()->AddEventSink(this);
		INFO("Registered TESDeathEvent event sink");

		auto inputMgr = RE::BSInputDeviceManager::GetSingleton();
		inputMgr->AddEventSink(this);
	}

	void ToggleKillFeed();

	// Inherited via BSTEventSink
	virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESDeathEvent*, RE::BSTEventSource<RE::TESDeathEvent>*) override;

	// Inherited via BSTEventSink
	virtual RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;

private:
	bool m_killFeedEnabled{ true };
};