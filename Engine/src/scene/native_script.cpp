#include "../script/script_manager.h"
#include "native_script.h"

namespace Fly {
    const u32 NativeScript::s_ComponentID = ComponentID<NativeScript>::value();

    NativeScript::NativeScript(Fly::Entity& entity): Component(entity) {
        g_ScriptManager->RegisterScript(this);
    }

    NativeScript::~NativeScript() {
        g_ScriptManager->DeregisterScript(this);
    }
}
