#pragma once
class Settings : public dku::model::Singleton<Settings>
{
public:
	static inline int32_t KillFeedToggleKey{ 88 };
	static inline bool ShowFormID{ true };
	static inline bool OnlyUniqueNPC{ false };
	static inline bool PlaySoundFX{ true };
	static inline std::string KillFeedColor{ "FF0000" };
	static inline std::string KillFeedSoundFx{ "UISneakAttack" };

	void ReadSettings();

private:
	std::string_view m_defaultPath{ "Data/MCM/Config/KillFeed/settings.ini" };
	std::string_view m_mcmPath{ "Data/MCM/Settings/KillFeed.ini" };
};