#include "game_manager.h"
#include "ball.h"

using namespace Fly;

void Ball::IncreaseSpeed(f32 amount) {
    m_MagnitudeLastFrame += amount;
    m_MagnitudeLastFrame = Math::Min(m_MagnitudeLastFrame, 15.0f);

    Math::Vec3 velocity = m_RigidBody->GetVelocity();
    m_RigidBody->SetVelocity(Math::Normalize(velocity)*m_MagnitudeLastFrame);
}

void Ball::DecreaseSpeed(f32 amount) {
    m_MagnitudeLastFrame -= amount;
    m_MagnitudeLastFrame = Math::Max(m_MagnitudeLastFrame, 8.0f);
    Math::Vec3 velocity = m_RigidBody->GetVelocity();
    m_RigidBody->SetVelocity(Math::Normalize(velocity)*m_MagnitudeLastFrame);
}

void Ball::StartTemporaryPierce() {
    PBRMaterial* pbrMat = (PBRMaterial*)m_RenderComponent->materials[0];
    pbrMat->albedo = Math::Vec3(3.0f, 0.0f, 1.0f);
    m_IsPiercing = true;
    m_PierceTimer = 0.0f;
    m_Collider->isTrigger = true;
}

void Ball::OnStart() {
    Mesh* ballMesh = g_ResourceManager->GetResource<Mesh>("Sphere");
    Material* ballMaterial = g_ResourceManager->GetResource<Material>("BallMaterial");
    PBRMaterial* pbrMat = (PBRMaterial*)ballMaterial;
    pbrMat->albedo = Math::Vec3(1.0f, 0.0f, 0.0f);

    m_RenderComponent = entity.AddComponent<RenderComponent>();
    m_RenderComponent->materials.push_back(ballMaterial);
    m_RenderComponent->mesh = ballMesh;

    m_Collider = entity.AddComponent<SphereCollider>(0.5f);
    m_RigidBody = entity.AddComponent<RigidBody>();
    Withhold();
}

void Ball::Withhold() {
    m_RigidBody->enabled = false;
    m_RigidBody->SetVelocity(Math::Vec3(0.0f));
}

void Ball::Release(const Math::Vec3& velocity) {
    m_RigidBody->enabled = true;
    m_RigidBody->SetVelocity(velocity);
    m_MagnitudeLastFrame = Math::Length(velocity);
}

void Ball::OnCollisionEnter(Collider& other) {
    m_MagnitudeLastFrame += 0.25f;
}

void Ball::OnTriggerEnter(Collider& other) {
    if (other.entity.name == "Paddle") {
        Math::Vec3 v = m_RigidBody->GetVelocity();
        Math::Vec3 n = Math::Normalize(-transform.GetPosition());

        f32 magnitude = Math::Length(v);
        Math::Vec3 dir = Math::Normalize(v);
        f32 dot = Math::Dot(n, Math::Normalize(v));
        Math::Vec3 newDir = dir - 2.0f*dot*n;

        m_RigidBody->SetVelocity(newDir*magnitude);
    }
}

void Ball::OnUpdate(f32 deltaTime) {
    if (m_IsPiercing) {
        m_PierceTimer += deltaTime;
        if (m_PierceTimer > 4.0f) {
            m_IsBlinking = true;
        }
        else {
            m_IsBlinking = false;
        }

        if (m_PierceTimer > 5.0f) {
            PBRMaterial* pbrMat = (PBRMaterial*)m_RenderComponent->materials[0];
            pbrMat->albedo = Math::Vec3(1.0f, 0.0f, 0.0f);

            m_PierceTimer = 0.0f;
            m_BlinkTimer = 0.0f;
            m_IsPiercing = false;
            m_IsBlinking = false;
            m_IsRedColor = true;
            m_Collider->isTrigger = false;
        }
    }

    if (m_IsBlinking) {
        m_BlinkTimer += deltaTime;
        if (m_BlinkTimer > m_BlinkTime) {
            m_BlinkTimer = 0.0f;
            NextColor();
        }
    }
}

void Ball::NextColor() {
    PBRMaterial* pbrMat = (PBRMaterial*)m_RenderComponent->materials[0];

    if (m_IsRedColor) {
        pbrMat->albedo = Math::Vec3(3.0f, 0.0f, 1.0f);
        m_IsRedColor = false;
    }
    else {
        pbrMat->albedo = Math::Vec3(1.0f, 0.0f, 0.0f);
        m_IsRedColor = true;
    }
    
}

void Ball::OnFixedUpdate(f32 fixedDeltaTime) {
    Math::Vec3 velocity = m_RigidBody->GetVelocity();
    f32 magnitude = Math::Length(velocity);
    f32 diff = Math::Clamp(magnitude - m_MagnitudeLastFrame, 0.0f, m_MaxSpeedDiff);

    f32 newMagnitude = Math::Min(m_MagnitudeLastFrame + diff, 15.0f);
    m_RigidBody->SetVelocity(Math::Normalize(velocity)*newMagnitude);
}