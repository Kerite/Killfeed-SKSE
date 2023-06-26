#include "EventsHandler.h"

void MessageListener(SKSE::MessagingInterface::Message* a_msg) noexcept
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		EventsHandler::GetSingleton()->Register();
		break;
	}
}

DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	DKUtil::Logger::Init(Plugin::NAME, REL::Module::get().version().string());

	//REL::Module::reset();
	SKSE::Init(a_skse);

	INFO("{} v{} loaded", Plugin::NAME, Plugin::Version);

	// do stuff
	SKSE::GetMessagingInterface()->RegisterListener(MessageListener);

	return true;
}
