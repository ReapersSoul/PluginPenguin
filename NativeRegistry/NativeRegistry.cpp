#include "NativeRegistry.hpp"


UUID::UUID NativeRegistry::GenerateUID()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<UUID::UUID> dis(0, 0 - 1);
	UUID::UUID uuid = dis(gen);
	while (libraries.find(uuid) != libraries.end())
	{
		uuid = dis(gen);
	}
	return uuid;
}

std::map<UUID::UUID, boost::dll::shared_library> &NativeRegistry::GetLibraries()
{
	return libraries;
}


std::map<UUID::UUID, std::string> NativeRegistry::GetEntryNames()
{
	std::map<UUID::UUID, std::string> names;
	for (auto &lib : libraries)
	{
		std::function<std::string()>name = lib.second.get<std::string()>("Name");
		if (name)
		{
			try{
				names[lib.first] = name();
			}
			catch(std::exception e){
				printf("Error getting name from library %d: %s\n",lib.first,e.what());
			}
		}
	}
	return names;
}

std::string NativeRegistry::GetEntryName(UUID::UUID uuid)
{
	if (libraries.find(uuid) != libraries.end())
	{
		std::function<std::string()> name = libraries[uuid].get<std::string()>("Name");
		if (name)
		{
			return name();
		}
	}
	return "";
}

void NativeRegistry::Unregister(UUID::UUID uuid)
{
	if (libraries.find(uuid) != libraries.end())
	{
		libraries.erase(uuid);
	}
}


void NativeRegistry::UnregisterAll()
{
	libraries.clear();
}