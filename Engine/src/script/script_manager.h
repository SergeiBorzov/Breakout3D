#ifndef FLY_SCRIPT_MANAGER_H
#define FLY_SCRIPT_MANAGER_H

#include <vector>

#include "../core/platform.h"
#include "../scene/native_script.h"

namespace Fly {
    class ScriptManager {
    public:
        void RegisterScript(NativeScript* script);
        void DeregisterScript(NativeScript* script);

        void Start();
        void Update(f32 deltaTime);
        void FixedUpdate(f32 fixedDeltaTime);
        void DestroyEntity(Entity* entity);
        void ReleaseMemory();
    private:
        std::vector<NativeScript*> m_StartScripts;
        std::vector<NativeScript*> m_UpdateScripts;
        std::vector<Entity*> m_EntitiesToRemove;
    };

    FLY_API extern ScriptManager* g_ScriptManager;
}

#endif /* End of FLY_SCRIPT_MANAGER_H */
