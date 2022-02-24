#include "scene.h"
#include "../script/script_manager.h"

namespace Fly {
    Entity* Scene::CreateEntity(const std::string& name) {
        Entity* entity = new Entity(*this, name);
        m_Entities.push_back(entity);
        return entity;
    }

    void Scene::DestroyEntity(Entity* entity) {
        auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);

        if (it != m_Entities.end() && entity) {
            std::iter_swap(it, m_Entities.end() - 1);
            m_Entities.pop_back();

            g_ScriptManager->DestroyEntity(entity);
        }
    }

    void Scene::ClearScene() {
        for (Entity* entity: m_Entities) {
            g_ScriptManager->DestroyEntity(entity);
        }
        m_Entities.clear();
    }

    Scene::~Scene() {
        for (Entity* entity: m_Entities) {
            delete entity;
        }
    }
}
