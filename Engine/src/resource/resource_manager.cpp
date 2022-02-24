#include "resource_manager.h"

namespace Fly {
    ResourceManager* g_ResourceManager = nullptr;
    ResourceManager::~ResourceManager() {
        for (auto& pair: m_ResourceCache) {
            delete pair.second;
        }
    }
}
