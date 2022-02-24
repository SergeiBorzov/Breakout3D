#include "../scene/native_script.h"
#include "physics_world.h"
#include "collider.h"


namespace Fly {
    const u32 Collider::s_ComponentID = ComponentID<Collider>::value();

    Collider::Collider(Entity& entity): Component(entity) {
        m_NativeScript = entity.GetComponent<NativeScript>();
        m_RigidBody = entity.GetComponent<RigidBody>();
        if (m_RigidBody) {
            m_RigidBody->m_Collider = this;
        }
        g_PhysicsWorld->RegisterCollider(this);
    }

    Collider::~Collider() {
        g_PhysicsWorld->DeregisterCollider(this);
    }
}