#include "../core/log.h"
#include "../scene/native_script.h"
#include "physics_world.h"

namespace Fly {
    PhysicsWorld* g_PhysicsWorld = nullptr;
    
    i32 PhysicsWorld::Init() {
        m_ContactData.contacts = new Contact[FLY_MAX_CONTACTS];
        m_ContactData.contactsLeft = FLY_MAX_CONTACTS;

        m_FunctionTable[0] = &CollisionDetection::SphereSphere;
        m_FunctionTable[1] = &CollisionDetection::SphereCylinderBrick;
        m_FunctionTable[2] = nullptr;
        m_FunctionTable[3] = nullptr;

        m_Inited = true;
        return 0;
    }
    void PhysicsWorld::RegisterRigidBody(RigidBody* rigidBody) {
        m_RigidBodyList.push_back(rigidBody);
    }
        
    void PhysicsWorld::DeregisterRigidBody(RigidBody* rigidBody) {
        m_RigidBodyList.remove(rigidBody);
    }

    void PhysicsWorld::RegisterCollider(Collider* collider) {
        collider->m_KeyToCollisionMap = m_NextKey++;
        m_Colliders.push_back(collider);
    }
        
    void PhysicsWorld::DeregisterCollider(Collider* collider) {
        auto it1 = std::find(m_Colliders.begin(), m_Colliders.end(), collider);

        if (it1 != m_Colliders.end()) {
            std::iter_swap(it1, m_Colliders.end() - 1);
            m_Colliders.pop_back();
            return;
        }
    }


    void PhysicsWorld::BroadPhase() {

    }


    void PhysicsWorld::SendCollisionEvent(Collider* collider, Collider* other) {
        if (collider->m_NativeScript) {
            auto p = std::make_pair(collider->m_KeyToCollisionMap, other->m_KeyToCollisionMap);
            auto it = m_CollisionMap.find(std::make_pair(collider->m_KeyToCollisionMap, other->m_KeyToCollisionMap));
            if (it == m_CollisionMap.end()) {
                collider->m_NativeScript->OnCollisionEnter(*other);
                m_CollisionMap.insert({p, true});
            }
            else {
                static int i = 0;
                collider->m_NativeScript->OnCollision(*other);
            }
        }
    }

    void PhysicsWorld::SendCollisionStopEvent(Collider* collider, Collider* other) {
        if (collider->m_NativeScript) {
            auto it = m_CollisionMap.find(std::make_pair(collider->m_KeyToCollisionMap, other->m_KeyToCollisionMap));
            if (it != m_CollisionMap.end()) {
                collider->m_NativeScript->OnCollisionExit(*other);
                m_CollisionMap.erase(it);
            }
        }
    }

    void PhysicsWorld::SendTriggerEvent(Collider* collider, Collider* other) {
        if (collider->m_NativeScript) {
            auto p = std::make_pair(collider->m_KeyToCollisionMap, other->m_KeyToCollisionMap);
            auto it = m_CollisionMap.find(std::make_pair(collider->m_KeyToCollisionMap, other->m_KeyToCollisionMap));
            if (it == m_CollisionMap.end()) {
                collider->m_NativeScript->OnTriggerEnter(*other);
                m_CollisionMap.insert({p, true});
            }
            else {
                collider->m_NativeScript->OnCollision(*other);
            }
        }
    }

    void PhysicsWorld::SendTriggerStopEvent(Collider* collider, Collider* other) {
        if (collider->m_NativeScript) {
            auto it = m_CollisionMap.find(std::make_pair(collider->m_KeyToCollisionMap, other->m_KeyToCollisionMap));
            if (it != m_CollisionMap.end()) {
                collider->m_NativeScript->OnTriggerExit(*other);
                m_CollisionMap.erase(it);
            }
        }
    }



    void PhysicsWorld::NearPhase() {
        // Clear contacts from previous frame
        m_ContactData.contactsLeft = FLY_MAX_CONTACTS;

        i32 count = (i32)m_Colliders.size();
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (!m_Colliders[i]->enabled || !m_Colliders[j]->enabled || 
                    !m_Colliders[i]->entity.IsActive() || !m_Colliders[j]->entity.IsActive())
                    continue;

                i32 t1 = (i32)m_Colliders[i]->GetType();
                i32 t2 = (i32)m_Colliders[j]->GetType();

                Collider* a;
                Collider* b;

                if (t2 > t1) {
                    i32 tmp = t1;
                    t1 = t2;
                    t2 = tmp;
                    a = m_Colliders[i];
                    b = m_Colliders[j];
                }
                else {
                    a = m_Colliders[j];
                    b = m_Colliders[i];
                }

                if (!m_FunctionTable[t1 + t2]) {
                    continue;
                }

                if ((*m_FunctionTable[t1 + t2])(*a, *b, m_ContactData)) {
                    
                    if (m_Colliders[i]->isTrigger || m_Colliders[j]->isTrigger) {
                        SendTriggerEvent(m_Colliders[i], m_Colliders[j]);
                        SendTriggerEvent(m_Colliders[j], m_Colliders[i]);
                    }
                    else {
                        SendCollisionEvent(m_Colliders[i], m_Colliders[j]);
                        SendCollisionEvent(m_Colliders[j], m_Colliders[i]);
                        m_ContactData.contactsLeft--;
                    }
                }
                else {
                    if (m_Colliders[i]->isTrigger || m_Colliders[j]->isTrigger) {
                        SendTriggerStopEvent(m_Colliders[i], m_Colliders[j]);
                        SendTriggerStopEvent(m_Colliders[j], m_Colliders[i]);
                    }
                    else {
                        SendCollisionStopEvent(m_Colliders[i], m_Colliders[j]);
                        SendCollisionStopEvent(m_Colliders[j], m_Colliders[i]);
                    }
                }

                if (m_ContactData.contactsLeft == 0) {
                    goto exit;
                }
            }
        }
        exit:;
    }

    void PhysicsWorld::CollisionResolution() {
        for (u32 i = 0; i < FLY_MAX_CONTACTS - m_ContactData.contactsLeft; i++) {
            RigidBody* body1 = m_ContactData.contacts[i].bodies[0];
            RigidBody* body2 = m_ContactData.contacts[i].bodies[1];

            Math::Vec3 v1 = body1->GetVelocity();
            Math::Vec3 v2 = body2->GetVelocity();
            f32 m1 = body1->GetInvMass();
            f32 m2 = body2->GetInvMass();

            //bool static1 = body1->GetInvMass() < Math::EPS;
            //bool static2 = body2->GetInvMass() < Math::EPS;
            Math::Vec3 n = m_ContactData.contacts[i].normal;
            Math::Vec3 pos1 = body1->transform.GetPosition();
            Math::Vec3 pos2 = body1->transform.GetPosition();
            f32 penetration = m_ContactData.contacts[i].penetration;


            // Temporary solution
            // body1 is always sphere
            // Just reflect it's velocity
            body1->transform.SetPosition(pos1 + n*penetration);
            Math::Vec3 dir = Math::Normalize(v1);
            f32 magnitude = Math::Length(v1);
            f32 dot = Math::Dot(n, Math::Normalize(v1));
            Math::Vec3 newDir = dir - 2.0f*dot*n;
            body1->SetVelocity(newDir*magnitude);
            
            /* 
            // Impulse based collision resolution
            f32 restitution = 0.5f*(body1->restitution + body2->restitution);
            f32 j = -(1.0f + restitution)*Math::Dot(v1 - v2, n)/(m1 + m2);

            if (!body1->isKinematic) {
                body1->SetVelocity(v1 + m1*j*n);
            }
            if (!body2->isKinematic) {
                body2->SetVelocity(v2 - m2*j*n);
            }
            */
        }
    }

    void PhysicsWorld::RunSimulation(f32 fixedDeltaTime) {
        // Broad phase
        NearPhase();
        CollisionResolution();
        for (RigidBody* rigidBody: m_RigidBodyList) {
            rigidBody->Integrate(fixedDeltaTime);
        }
    }

    PhysicsWorld::~PhysicsWorld() {
        if (m_Inited) {
            delete[] m_ContactData.contacts;
        }
    }
}