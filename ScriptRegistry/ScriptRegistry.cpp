#include "ScriptRegistry.hpp"

ScriptRegistry::ScriptRegistry(std::vector<Language *>* languages)
{
	this->languages = languages;
}

void ScriptRegistry::SetLanguages(std::vector<Language *>* languages)
{
	this->languages = languages;
}

UUID::UUID ScriptRegistry::GenerateUID()
{
	UUID::UUID uuid = 0;
	while (scripts.find(uuid) != scripts.end())
	{
		uuid++;
	}
	return uuid++;
}

std::map<UUID::UUID, std::filesystem::path> &ScriptRegistry::GetScripts()
{
	return scripts;
}

std::map<UUID::UUID, std::string> ScriptRegistry::GetEntryNames()
{
	std::map<UUID::UUID, std::string> names;
	for (auto &script : scripts)
	{
		names[script.first] = script.second.string();
	}
	return names;
}

std::string ScriptRegistry::GetEntryName(UUID::UUID uuid)
{
	if (scripts.find(uuid) != scripts.end())
	{
		return scripts[uuid].string();
	}
	return "";
}

void ScriptRegistry::Unregister(UUID::UUID uuid)
{
	if (scripts.find(uuid) != scripts.end())
	{
		scripts.erase(uuid);
		script_constructors.erase(uuid);
	}
}

void ScriptRegistry::UnregisterAll()
{
	scripts.clear();
	script_constructors.clear();
}