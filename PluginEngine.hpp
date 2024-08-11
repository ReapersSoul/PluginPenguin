#pragma once
#include <climits>
#include <NativeRegistry/NativeRegistry.hpp>
#include <ScriptRegistry/ScriptRegistry.hpp>

class PluginEngine
{

	std::vector<Language *> languages;
	NativeRegistry nativeRegistry;
	ScriptRegistry scriptRegistry;

public:
	PluginEngine();

	std::vector<Language *> GetLanguages();

	std::vector<std::string> GetLanguagesNames();

	template <typename T>
	std::vector<UUID::UUID> RegisterScripts(std::filesystem::path path, bool recursive)
	{
		return scriptRegistry.RegisterAll<T>(path, recursive);
	}

	template <typename T>
	UUID::UUID RegisterScript(std::filesystem::path path)
	{
		return scriptRegistry.Register<T>(path);
	}

	template <typename T>
	T *ConstructScript(UUID::UUID uuid)
	{
		static_assert(std::is_base_of<Script, T>::value, "T must be derived from Script");
		return scriptRegistry.Construct<T>(uuid);
	}

	std::string GetScriptName(UUID::UUID uuid);

	template <typename T>
	std::vector<UUID::UUID> RegisterNatives(std::filesystem::path path, bool recursive=false)
	{
		std::vector<UUID::UUID> uids;
        if (recursive)
        {
            if (!std::filesystem::exists(path))
            {
                printf("Path %s does not exist\n", path.string().c_str());
                std::filesystem::create_directory(path);
				return uids;
            }
            for (auto &p : std::filesystem::recursive_directory_iterator(path))
            {
                if (p.path().extension() == ".dll" || p.path().extension() == ".so")
                {
					UUID::UUID uuid = RegisterNative<T>(p.path());
					if (uuid != UINT_MAX)
					{
						uids.push_back(uuid);
					}
                }
            }
        }
        else
        {
			UUID::UUID uuid = RegisterNative<T>(path);
			if (uuid != UINT_MAX)
			{
				uids.push_back(uuid);
			}
        }
        return uids;
	}

	template <typename T>
	UUID::UUID RegisterNative(std::filesystem::path path)
	{
		UUID::UUID id=nativeRegistry.Register<T>(path);
		//if T is a Language, add it to the list of languages
		if (std::is_base_of<Language, T>::value)
		{
			languages.push_back(nativeRegistry.Construct<Language>(id, nullptr));
			languages.back()->Initialize();
		}
		return id;
	}

	template <typename T>
	T *ConstructNative(UUID::UUID uuid, void *userData=nullptr)
	{
		return nativeRegistry.Construct<T>(uuid, userData);
	}

	std::string GetNativeName(UUID::UUID uuid);
};