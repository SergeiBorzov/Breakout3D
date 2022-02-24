#ifndef FLY_COLLISION_DETECTION_H
#define FLY_COLLISION_DETECTION_H

#define FLY_MAX_CONTACTS 1

namespace Fly {
    class RigidBody;
    class Collider;

    struct Contact {
        Math::Vec3 point;
        Math::Vec3 normal;
        f32 penetration;
        RigidBody* bodies[2];
    };

    struct ContactData {
        Contact* contacts = nullptr;
        u32 contactsLeft = 0;
    };

    typedef  bool (*CollisionTestFunc)(const Collider& sphereA, const Collider& sphereB, ContactData& contactData);

    class CollisionDetection {
    public:
        static bool SphereSphere(const Collider& sphereA, const Collider& sphereB, ContactData& contactData);
        static bool SphereCylinderBrick(const Collider& sphere, const Collider& cylinderBrick, ContactData& contactdData);
    private:
    };
}
#endif /* FLY_COLLISION_DETECTION_H */