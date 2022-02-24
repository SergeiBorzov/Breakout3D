#ifndef FLY_RESOURCE_MANAGER_H
#define FLY_RESOURCE_MANAGER_H

#include <unordered_map>
#include <string>

#include "../core/platform.h"
#include "../app.h"
#include "resource.h"
#include "loader.h"

namespace Fly {
    class ResourceManager {
    public:
        template <typename T>
        T* GetResource(const char* name);
        template <typename T, typename... Args>
        T* GetResource(const char* name, Args&&... args);

        template <typename T>
        void AddResource(const char* name, T* resource);

        ~ResourceManager();
    private:
        std::unordered_map<std::string, Resource*> m_ResourceCache;
    };

    template <typename T>
    void ResourceManager::AddResource(const char* name, T* resource) {
        auto it = m_ResourceCache.find(name);

        if (it != m_ResourceCache.end()) {
            FLY_WARN("Resource name collision, %s", name);
            return;
        }

        m_ResourceCache.insert({name, (Resource*)resource});
    }

    template <typename T>
    T* ResourceManager::GetResource(const char* name) {
        auto it = m_ResourceCache.find(name);

        if (it != m_ResourceCache.end()) {
            return (T*)it->second;
        }
        
        T* result = nullptr;

        std::string abs_path = (App::GetPath() / std::filesystem::path(name)).u8string();
        Loader::LoadResource(abs_path.c_str(), &result);
        if (result) {
            m_ResourceCache.insert({name, (Resource*)result});
        }
        return result;
    }

    template <typename T, typename... Args>
    T* ResourceManager::GetResource(const char* name, Args&&... args) {
        auto it = m_ResourceCache.find(name);

        if (it != m_ResourceCache.end()) {
            return (T*)it->second;
        }
        
        T* result = nullptr;

        std::string abs_path = (App::GetPath() / std::filesystem::path(name)).u8string();
        Loader::LoadResource(abs_path.c_str(), &result, std::forward<Args>(args)...);
        if (result) {
            m_ResourceCache.insert({std::string(name) + result->GetName(), (Resource*)result});
        }
        return result;
    }

    FLY_API extern ResourceManager* g_ResourceManager;  
}

#endif /* End of FLY_RESOURCE_MANAGER_H */
