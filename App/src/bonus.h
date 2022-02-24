#ifndef BONUS_H
#define BONUS_H

#include "fly.h"

namespace Fly {
    class Bonus: public Fly::NativeScript {
    public:
        virtual void OnStart();
        virtual void OnUpdate(f32 deltaTime);
        virtual void OnTriggerEnter(Collider& other);

        virtual ~Bonus() {}            
    protected:
        Bonus(Fly::Entity& entity, const Math::Vec3& position, const Math::Vec3& velocity): 
            m_Position(position),
            m_Velocity(velocity),
            Fly::NativeScript(entity) 
        {}

        Math::Vec3 m_Position;
        Math::Vec3 m_Velocity;
        Fly::RenderComponent* m_RenderComponent = nullptr;
        Fly::RigidBody* m_RigidBody = nullptr;
        Fly::SphereCollider* m_Collider = nullptr;
        u32 m_Value = 10;
    };

    class ExtraPoints: public Bonus {
    public:
        ExtraPoints(Fly::Entity& entity, const Math::Vec3& position, const Math::Vec3& velocity): 
            Bonus(entity, position, velocity)
        {}

        virtual void OnStart();

        virtual ~ExtraPoints() {}
    };

    class ExtraLife: public Bonus {
    public:
        ExtraLife(Fly::Entity& entity, const Math::Vec3& position, const Math::Vec3& velocity): 
            Bonus(entity, position, velocity)
        {}

        virtual void OnStart();
        virtual void OnTriggerEnter(Collider& other);

        virtual ~ExtraLife() {}
    };

    class IncreasePaddle: public Bonus {
    public:
        IncreasePaddle(Fly::Entity& entity, const Math::Vec3& position, const Math::Vec3& velocity):
            Bonus(entity, position, velocity)
        {}

        virtual void OnStart();
        virtual void OnTriggerEnter(Collider& other);

        virtual ~IncreasePaddle() {}
    };

    class DecreasePaddle: public Bonus {
    public:
        DecreasePaddle(Fly::Entity& entity, const Math::Vec3& position, const Math::Vec3& velocity):
            Bonus(entity, position, velocity)
        {}

        virtual void OnStart();
        virtual void OnTriggerEnter(Collider& other);

        virtual ~DecreasePaddle() {}
    };

    class IncreaseBallSpeed: public Bonus {
    public:
        IncreaseBallSpeed(Fly::Entity& entity, const Math::Vec3& position, const Math::Vec3& velocity):
            Bonus(entity, position, velocity)
        {}

        virtual void OnStart();
        virtual void OnTriggerEnter(Collider& other);

        virtual ~IncreaseBallSpeed() {}
    };

    class DecreaseBallSpeed: public Bonus {
    public:
        DecreaseBallSpeed(Fly::Entity& entity, const Math::Vec3& position, const Math::Vec3& velocity):
            Bonus(entity, position, velocity)
        {}

        virtual void OnStart();
        virtual void OnTriggerEnter(Collider& other);

        virtual ~DecreaseBallSpeed() {}
    };

    class TemporaryPierce: public Bonus {
    public:
        TemporaryPierce(Fly::Entity& entity, const Math::Vec3& position, const Math::Vec3& velocity):
            Bonus(entity, position, velocity)
        {}
        
        virtual void OnStart();
        virtual void OnTriggerEnter(Collider& other);

        virtual ~TemporaryPierce() {}
    };

}
#endif 