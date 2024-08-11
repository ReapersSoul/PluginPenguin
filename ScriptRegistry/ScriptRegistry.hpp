#pragma once
#include <Language/Language.hpp>
#include <filesystem>
#include <map>
#include <Script/Script.hpp>

namespace UUID{
    typedef unsigned int UUID;
}

class ScriptRegistry
{
protected:
    std::vector<Language *> *languages;
    std::map<UUID::UUID, std::filesystem::path> scripts;
    std::map<UUID::UUID, std::function<Script *()>> script_constructors;
    UUID::UUID GenerateUID();
    std::string typeID;

public:
    ScriptRegistry(std::vector<Language *> *languages);

    void SetLanguages(std::vector<Language *> *languages);

    template <typename T>
    T *Construct(UUID::UUID uuid)
    {
        static_assert(std::is_base_of<Script, T>::value, "T must be a subclass of Script");
        if (scripts.find(uuid) != scripts.end())
        {
            return dynamic_cast<T *>(script_constructors[uuid]());
        }
        return nullptr;
    }

    template <typename T>
    UUID::UUID RegisterAll(std::filesystem::path path, bool recursive)
    {
        static_assert(std::is_base_of<Script, T>::value, "T must be a subclass of Script");
        UUID::UUID count = 0;
        if (recursive)
        {
            if (!std::filesystem::exists(path))
            {
                printf("Path %s does not exist\n", path.string().c_str());
                std::filesystem::create_directory(path);
            }
            for (auto &p : std::filesystem::recursive_directory_iterator(path))
            {
                if (p.path().extension() == ".dll" || p.path().extension() == ".so")
                {
                    if (Register<T>(p.path()) != 0)
                    {
                        count++;
                    }
                }
            }
        }
        else
        {
            if (Register<T>(path) != 0)
            {
                count++;
            }
        }
        return count;
    }

    template <typename T>
    UUID::UUID Register(std::filesystem::path path)
    {
        static_assert(std::is_base_of<Script, T>::value, "T must be a subclass of Script");
        if (!std::filesystem::exists(path))
        {
            printf("Path %s does not exist\n", path.string().c_str());
            return 0;
        }

        for (auto &language : *languages)
        {
            bool extensionMatch = false;
            for (auto &ext : language->getExtensions())
            {
                if (path.extension() == ext)
                {
                    extensionMatch = true;
                    break;
                }
            }

            if (extensionMatch)
            {
                UUID::UUID uuid = GenerateUID();
                scripts[uuid] = path;
                script_constructors[uuid] = [this, path, language]() -> Script *
                {
                    return new T(language, path);
                };
                return uuid;
            }
        }

        // print supported languages and extensions
        printf("File %s does not have a supported extension\n", path.string().c_str());
        printf("Supported languages and extensions:\n");
        for (auto &language : *languages)
        {
            printf("%s: ", language->getName().c_str());
            for (auto &ext : language->getExtensions())
            {
                printf("%s ", ext.c_str());
            }
            printf("\n");
        }

        return 0;
    }

    std::map<UUID::UUID, std::filesystem::path> &GetScripts();

    std::map<UUID::UUID, std::string> GetEntryNames();

    std::string GetEntryName(UUID::UUID uuid);

    void Unregister(UUID::UUID uuid);

    void UnregisterAll();
};