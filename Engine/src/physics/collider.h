#ifndef FLY_COLLIDER_H
#define FLY_COLLIDER_H

#include "../scene/entity.h"

namespace Fly {
    class NativeScript;
    class RigidBody;

    class Collider: public Component {
        friend class PhysicsWorld;
        friend class CollisionDetection;
        friend class RigidBody;
    public:
        FLY_API static const u32 s_ComponentID;

        virtual ~Collider();

        enum class Type {
            Sphere = 0,
            CylinderBrick = 1,
        };
        Type GetType() const { return m_Type; }

        bool isTrigger = false;
        bool enabled = true;
    protected:
        Collider(Entity& entity);
        RigidBody* m_RigidBody;
        NativeScript* m_NativeScript;

        Type m_Type = Type::Sphere; 
    private:
        u32 m_KeyToCollisionMap = 0;
    };

    class SphereCollider: public Collider {
    public:
        SphereCollider(Entity& entity, f32 radius): Collider(entity), radius(radius) {}
        f32 radius = 1.0f;
    };

    class CylinderBrickCollider: public Collider {
    public:
        // angle is in degrees
        CylinderBrickCollider(Entity& entity, f32 R, f32 r, f32 h, f32 angle): 
            Collider(entity),
            largeCylinderRadius(R),
            smallCylinderRadius(r),
            height(h),
            angle(Math::Radians(angle))
        {
            m_Type = Type::CylinderBrick;
        }

        f32 largeCylinderRadius = 2.0f;
        f32 smallCylinderRadius = 1.0f;
        f32 height = 1.0f;
        f32 angle = Math::Radians(30.0f);
    };
}

#endif /* End of FLY_COLLIDER_H */