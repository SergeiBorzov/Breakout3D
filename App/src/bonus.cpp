#include "scoring_system.h"
#include "game_manager.h"
#include "bonus.h"

using namespace Fly;

void Bonus::OnStart() {
    m_Collider = entity.AddComponent<SphereCollider>(0.75f);
    m_Collider->isTrigger = true;
    m_RigidBody = entity.AddComponent<RigidBody>();
    m_RigidBody->SetVelocity(m_Velocity);
    transform.SetPosition(m_Position);
}

void Bonus::OnTriggerEnter(Collider& other) {
    if (other.entity.name == "Paddle") {
        ScoringSystem::GetInstance()->AddPoints(m_Value);
        scene.DestroyEntity(&entity);
    }
}

void Bonus::OnUpdate(f32 deltaTime) {
    transform.Rotate(0.0f, deltaTime*150.0f, 0.0f);

    f32 R = 20.0f + 0.5f;
    if (Math::LengthSqr(transform.GetPosition()) > R*R) {
        scene.DestroyEntity(&entity);
    }
}

void ExtraPoints::OnStart() {
    Mesh* mesh = g_ResourceManager->GetResource<Mesh>("Assets/Meshes/coin.obj");
    Material* ballMaterial = g_ResourceManager->GetResource<Material>("ExtraPointsMaterial");

    m_RenderComponent = entity.AddComponent<RenderComponent>();
    m_RenderComponent->materials.push_back(ballMaterial);
    m_RenderComponent->mesh = mesh;

    m_Value = 500;
    Bonus::OnStart();
}


void ExtraLife::OnStart() {
    Mesh* mesh = g_ResourceManager->GetResource<Mesh>("Assets/Meshes/heart.obj");
    Material* ballMaterial = g_ResourceManager->GetResource<Material>("BuffBonusMaterial");
    m_RenderComponent = entity.AddComponent<RenderComponent>();
    m_RenderComponent->materials.push_back(ballMaterial);
    m_RenderComponent->mesh = mesh;

    Bonus::OnStart();
}

void ExtraLife::OnTriggerEnter(Collider& other) {
    if (other.entity.name == "Paddle")
        GameManager::GetInstance()->AddLife(1);
    Bonus::OnTriggerEnter(other);
}

void IncreasePaddle::OnStart() {
    Mesh* mesh = g_ResourceManager->GetResource<Mesh>("Assets/Meshes/increasePaddle.obj");
    Material* ballMaterial = g_ResourceManager->GetResource<Material>("BuffBonusMaterial");
    m_RenderComponent = entity.AddComponent<RenderComponent>();
    m_RenderComponent->materials.push_back(ballMaterial);
    m_RenderComponent->mesh = mesh;

    Bonus::OnStart();
}

void IncreasePaddle::OnTriggerEnter(Collider& other) {
    if (other.entity.name == "Paddle")
        GameManager::GetInstance()->IncreasePaddles();
    Bonus::OnTriggerEnter(other);
}

void DecreasePaddle::OnStart() {
    Mesh* mesh = g_ResourceManager->GetResource<Mesh>("Assets/Meshes/decreasePaddle.obj");
    Material* ballMaterial = g_ResourceManager->GetResource<Material>("DebuffBonusMaterial");
    m_RenderComponent = entity.AddComponent<RenderComponent>();
    m_RenderComponent->materials.push_back(ballMaterial);
    m_RenderComponent->mesh = mesh;

    Bonus::OnStart();

    m_Value = 100;
}

void DecreasePaddle::OnTriggerEnter(Collider& other) {
    if (other.entity.name == "Paddle")
        GameManager::GetInstance()->DecreasePaddles();
    Bonus::OnTriggerEnter(other);
}

void IncreaseBallSpeed::OnStart() {
    Mesh* mesh = g_ResourceManager->GetResource<Mesh>("Assets/Meshes/increaseSpeed.obj");
    Material* ballMaterial = g_ResourceManager->GetResource<Material>("DebuffBonusMaterial");
    m_RenderComponent = entity.AddComponent<RenderComponent>();
    m_RenderComponent->materials.push_back(ballMaterial);
    m_RenderComponent->mesh = mesh;

    Bonus::OnStart();

    m_Value = 100;
}

void IncreaseBallSpeed::OnTriggerEnter(Collider& other) {
    if (other.entity.name == "Paddle")
        GameManager::GetInstance()->IncreaseBallSpeed();
    Bonus::OnTriggerEnter(other);
}

void DecreaseBallSpeed::OnStart() {
    Mesh* mesh = g_ResourceManager->GetResource<Mesh>("Assets/Meshes/decreaseSpeed.obj");
    Material* ballMaterial = g_ResourceManager->GetResource<Material>("BuffBonusMaterial");
    m_RenderComponent = entity.AddComponent<RenderComponent>();
    m_RenderComponent->materials.push_back(ballMaterial);
    m_RenderComponent->mesh = mesh;

    Bonus::OnStart();
}

void DecreaseBallSpeed::OnTriggerEnter(Collider& other) {
    if (other.entity.name == "Paddle")
        GameManager::GetInstance()->DecreaseBallSpeed();
    Bonus::OnTriggerEnter(other);
}

void TemporaryPierce::OnStart() {
    Mesh* mesh = g_ResourceManager->GetResource<Mesh>("Assets/Meshes/pierce.obj");
    Material* ballMaterial = g_ResourceManager->GetResource<Material>("PierceBonusMaterial");
    m_RenderComponent = entity.AddComponent<RenderComponent>();
    m_RenderComponent->materials.push_back(ballMaterial);
    m_RenderComponent->mesh = mesh;

    Bonus::OnStart();
}

void TemporaryPierce::OnTriggerEnter(Collider& other) {
    if (other.entity.name == "Paddle")
        GameManager::GetInstance()->TemporaryPierce();
    Bonus::OnTriggerEnter(other);
}