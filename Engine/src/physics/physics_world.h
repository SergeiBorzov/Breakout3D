#ifndef FLY_PHYSICS_WORLD_H
#define FLY_PHYSICS_WORLD_H

#include <list>
#include <vector>
#include <unordered_map>
#include <utility>

#include "collider.h"
#include "collision_detection.h"
#include "rigid_body.h"

namespace Fly {

    struct PairHash
    {
        template <class T1, class T2>
        std::size_t operator() (const std::pair<T1, T2> &pair) const {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };

    class PhysicsWorld {
    public:
        i32 Init();
        void RegisterRigidBody(RigidBody* rigidBody);
        void DeregisterRigidBody(RigidBody* rigidBody);
        void RegisterCollider(Collider* collider);
        void DeregisterCollider(Collider* collider);

        void RunSimulation(f32 fixedDeltaTime);
        inline void SendEvents();

        ~PhysicsWorld();
    private:
        void BroadPhase();
        void NearPhase();
        void CollisionResolution();
        void SendCollisionEvent(Collider* collider, Collider* other);
        void SendCollisionStopEvent(Collider* collider, Collider* other);
        void SendTriggerEvent(Collider* collider, Collider* other);
        void SendTriggerStopEvent(Collider* collider, Collider* other);

        std::vector<Collider*> m_Colliders;
        std::list<RigidBody*> m_RigidBodyList;
        
        std::unordered_map<std::pair<u32, u32>, bool, PairHash> m_CollisionMap;
        CollisionTestFunc m_FunctionTable[4];
        ContactData m_ContactData;
        bool m_Inited = false;

        u32 m_NextKey = 0;
    };

    FLY_API extern PhysicsWorld* g_PhysicsWorld;
}

#endif /* End of FLY_PHYSICS_WORLD_H */