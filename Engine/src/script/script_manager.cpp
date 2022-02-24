#include <algorithm>

#include "script_manager.h"

namespace Fly {
    ScriptManager* g_ScriptManager = nullptr;
    
    void ScriptManager::RegisterScript(NativeScript* script) {
        if (script->entity.IsActive()) {
            m_StartScripts.push_back(script);
        }
    }

    void ScriptManager::DeregisterScript(NativeScript* script) {
        auto it1 = std::find(m_UpdateScripts.begin(), m_UpdateScripts.end(), script);

        if (it1 != m_UpdateScripts.end()) {
            std::iter_swap(it1, m_UpdateScripts.end() - 1);
            m_UpdateScripts.pop_back();
            return;
        }

        auto it2 = std::find(m_StartScripts.begin(), m_StartScripts.end(), script);
        if (it2 != m_StartScripts.end()) {
            std::iter_swap(it2, m_StartScripts.end() - 1);
            m_StartScripts.pop_back();
        }
    }

    void ScriptManager::Start() {
        for (int i = 0; i < m_StartScripts.size(); i++) {
            m_StartScripts[i]->OnStart();
        }

        m_UpdateScripts.insert(std::end(m_UpdateScripts),
                               std::begin(m_StartScripts), std::end(m_StartScripts));
        m_StartScripts.clear();
    }

    void ScriptManager::Update(f32 deltaTime) {
        for (int i = 0; i < m_UpdateScripts.size(); i++) {
            m_UpdateScripts[i]->OnUpdate(deltaTime);
        }
    }

    void ScriptManager::FixedUpdate(f32 fixedDeltaTime) {
        for (int i = 0; i < m_UpdateScripts.size(); i++) {
            m_UpdateScripts[i]->OnFixedUpdate(fixedDeltaTime);
        }
    }

    void ScriptManager::DestroyEntity(Entity* entity) {
        m_EntitiesToRemove.push_back(entity);
    }

    void ScriptManager::ReleaseMemory() {
        for (Entity* entity: m_EntitiesToRemove) {
            delete entity;
        }
        m_EntitiesToRemove.clear();
    }
}
