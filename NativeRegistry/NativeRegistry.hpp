#pragma once
#include <boost/dll/shared_library.hpp>
#include <boost/function.hpp>
#include <filesystem>
#include <map>
#include <random>
#include <algorithm>

namespace UUID{
    typedef unsigned int UUID;
}

class NativeRegistry
{
protected:
    std::map<UUID::UUID, boost::dll::shared_library> libraries;
    UUID::UUID GenerateUID();
public:
    template <typename T>
    T *Construct(UUID::UUID uuid, void * userData)
    {
        if (libraries.find(uuid) != libraries.end())
        {
            std::function<T *(void*)> constructor = libraries[uuid].get<T *(void*)>("Create");
            return constructor(userData);
        }
        return nullptr;
    }

    template <typename T>
    std::vector<UUID::UUID> RegisterAll(std::filesystem::path path, bool recursive)
    {
        std::vector<UUID::UUID> uids;
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
                    UUID::UUID uuid = Register<T>(p.path());
                    if (uuid != UINT_MAX)
                    {
                        uids.push_back(uuid);
                    }
                }
            }
        }
        else
        {
            UUID::UUID uuid = Register<T>(path);
            if (uuid != UINT_MAX)
            {
                uids.push_back(uuid);
            }
        }
        return uids;
    }

    template <typename T>
    UUID::UUID Register(std::filesystem::path path)
    {
        boost::dll::shared_library library(path.string());
        if (library.has("Create") && library.has("Name"))
        {
            UUID::UUID uuid = GenerateUID();
            libraries[uuid] = library;
            // try to call the name function
            std::function<std::string()> name = library.get<std::string()>("Name");
            if (name)
            {
                printf("Registered %s\n", name().c_str());
            }

            return uuid;
        }
        else
        {
            library.unload();
            printf("Library %s does not have the required entry points\n", path.string().c_str());
        }
        return 0;
    }

    std::map<UUID::UUID, boost::dll::shared_library> &GetLibraries();

    std::map<UUID::UUID, std::string> GetEntryNames();

    std::string GetEntryName(UUID::UUID uuid);

    void Unregister(UUID::UUID uuid);

    void UnregisterAll();
};