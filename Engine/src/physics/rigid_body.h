#ifndef FLY_RIGID_BODY_H
#define FLY_RIGID_BODY_H

#include "../core/platform.h"
#include "../core/types.h"
#include "../scene/entity.h"
#include "../math/general.h"

namespace Fly {
    class Collider;

    class RigidBody: public Fly::Component {
        friend class PhysicsWorld;
        friend class Collider;
    public:
        FLY_API static const u32 s_ComponentID;

        RigidBody(Entity& entity);
        void Integrate(f32 fixedDeltaTime);

        inline f32 GetInvMass() const { return m_InvMass; }
        inline Math::Vec3 GetVelocity() const { return m_Velocity; }

        inline void SetMass(f32 mass) { f32 m = Math::Max(mass, 0.0f); m > 0.0f ? m_InvMass = 1/m : m_InvMass = 0.0f; }
        inline void SetVelocity(const Math::Vec3& velocity) { m_Velocity = velocity; }

        virtual ~RigidBody();

        bool enabled = true;
        f32 restitution = 1.0f;
    private:
        Math::Vec3 m_Velocity = Math::Vec3(0.0f);
        Collider* m_Collider;
        f32 m_InvMass = 1.0f;
    };
}
#endif /* End of FLY_RIGID_BODY_H */