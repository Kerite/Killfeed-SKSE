#include "Settings.h"

// Copilot generated code
int ConvertRGBStringToInt(std::string rgb_string)
{
	int r = 0, g = 0, b = 0;
	// Extract the individual color values from the string
	size_t pos = 0;
	for (int i = 0; i < 3; i++) {
		size_t sep_pos = rgb_string.find(",", pos);
		std::string color_string = rgb_string.substr(pos, sep_pos - pos);
		pos = sep_pos + 1;
		switch (i) {
		case 0:
			r = std::stoi(color_string);
			break;
		case 1:
			g = std::stoi(color_string);
			break;
		case 2:
			b = std::stoi(color_string);
			break;
		}
	}
	// Combine the values using bitwise OR and shift operators
	int color = (r << 16) | (g << 8) | b;
	return color;
}

void ReadBoolean(CSimpleIniA& a_ini, std::string_view a_section, std::string_view a_key, bool& a_out)
{
	auto found = a_ini.GetValue(a_section.data(), a_key.data());
	if (found) {
		auto value = a_ini.GetBoolValue(a_section.data(), a_key.data());
		a_out = value;
	}
	INFO("Read config [{}]{}, value: {}", a_section, a_key, found)
}

void ReadInt32(CSimpleIniA& a_ini, std::string_view a_section, std::string_view a_key, int32_t& a_out)
{
	auto found = a_ini.GetValue(a_section.data(), a_key.data());
	if (found) {
		a_out = a_ini.GetLongValue(a_section.data(), a_key.data());
	}
	INFO("Read config [{}]{}, value: {}", a_section, a_key, found)
}

void ReadColor(CSimpleIniA& a_ini, std::string_view a_section, std::string_view a_key, int32_t& a_out)
{
	auto found = a_ini.GetValue(a_section.data(), a_key.data());
	if (found) {
		std::string colorStr(found);
		a_out = ConvertRGBStringToInt(colorStr);
	}
	INFO("Read config [{}]{}, value: {}", a_section, a_key, found)
}

void ReadString(CSimpleIniA& a_ini, std::string_view a_section, std::string_view a_key, std::string& a_out)
{
	auto found = a_ini.GetValue(a_section.data(), a_key.data());
	if (found) {
		a_out = std::string(found);
	}
	INFO("Read config [{}]{}, value: {}", a_section, a_key, found)
}

void LoadSettings(std::filesystem::path path)
{
	CSimpleIniA mcm;

	mcm.LoadFile(path.string().c_str());
	INFO("Reading Settings {}", path.string().c_str())
	mcm.SetUnicode();

	ReadBoolean(mcm, "Main", "bShowFormID", Settings::ShowFormID);
	ReadBoolean(mcm, "Main", "bOnlyUniqueNPC", Settings::OnlyUniqueNPC);
	ReadInt32(mcm, "Main", "iKillFeedToggleKey", Settings::KillFeedToggleKey);
	ReadString(mcm, "Main", "sKillFeedColor", Settings::KillFeedColor);

	ReadBoolean(mcm, "Sound", "bPlaySoundFX", Settings::PlaySoundFX);
	ReadString(mcm, "Sound", "sSoundFx", Settings::KillFeedSoundFx);

	ReadBoolean(mcm, "Debug", "bDebugMode", Settings::DebugMode);
}

void Settings::ReadSettings()
{
	LoadSettings(m_defaultPath);
	LoadSettings(m_mcmPath);

	dku::Logger::EnableDebug(DebugMode);
}