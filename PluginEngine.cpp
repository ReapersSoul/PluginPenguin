#include "PluginEngine.hpp"

PluginEngine::PluginEngine(): nativeRegistry(), scriptRegistry(&languages)
{
	
}

std::vector<Language *> PluginEngine::GetLanguages()
{
	return languages;
}

std::vector<std::string> PluginEngine::GetLanguagesNames()
{
	std::vector<std::string> names;
	for (Language * language : languages)
	{
		names.push_back(language->getName());
	}
	return names;
}

std::string PluginEngine::GetScriptName(UUID::UUID uuid)
{
	return scriptRegistry.GetEntryName(uuid);
}

std::string PluginEngine::GetNativeName(UUID::UUID uuid)
{
	return nativeRegistry.GetEntryName(uuid);
}