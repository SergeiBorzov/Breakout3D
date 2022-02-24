#include "collider.h"
#include "physics_world.h"

#include "../core/log.h"

namespace Fly {
    const u32 RigidBody::s_ComponentID = ComponentID<RigidBody>::value();

    RigidBody::RigidBody(Entity& entity): Component(entity) {
        m_Collider = entity.GetComponent<Collider>();
        if (m_Collider) {
            m_Collider->m_RigidBody = this;
        }
        g_PhysicsWorld->RegisterRigidBody(this);
    }

    void RigidBody::Integrate(f32 fixedDeltaTime) {
        bool stat = m_InvMass < Math::EPS;
        if (enabled && !stat) {
            Math::Vec3 pos = transform.GetPosition();
            if (Math::LengthSqr(fixedDeltaTime*m_Velocity) < Math::EPS) {
                return;
            }
            pos += fixedDeltaTime*m_Velocity;
            transform.SetPosition(pos);
        }
    }


    RigidBody::~RigidBody() {
        g_PhysicsWorld->DeregisterRigidBody(this);
    }

    
}