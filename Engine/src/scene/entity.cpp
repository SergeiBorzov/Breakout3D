#include "scene.h"
#include "native_script.h"
#include "../script/script_manager.h"

namespace Fly {
    Entity::Entity(Scene& s, const std::string& name = "Entity"):
        scene(s),
        transform(Transform(*this)),
        name(name)
    {}

    void Entity::SetActive(bool value) {
        if (value && !active) {
            NativeScript* script = this->GetComponent<Fly::NativeScript>();
            if (script) {
                active = value;
                g_ScriptManager->RegisterScript(script);
            }
        }

        if (!value && active) {
            NativeScript* script = this->GetComponent<Fly::NativeScript>();
            if (script) {
                active = value;
                g_ScriptManager->DeregisterScript(script);
            }
        }

        active = value;
    }

    Entity::~Entity() {
        for (int i = 0; i < FLY_MAX_COMPONENTS; i++) {
            if (m_Components[i]) {
                delete m_Components[i];
            }
        }
    }
}
