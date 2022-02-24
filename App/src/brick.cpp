
#include "tower.h"
#include "scoring_system.h"
#include "game_manager.h"
#include "bonus.h"
#include "brick.h"

using namespace Fly;

Brick::Brick(Fly::Entity& entity, Tower* tower, i32 index): 
    Fly::NativeScript(entity), m_Tower(tower), m_Index(index)     
{
    m_DissolveMaterial = new PBRMaterial();
}

Brick::~Brick() {
    if (m_DissolveMaterial) {
        delete m_DissolveMaterial;
        m_DissolveMaterial = nullptr;
    }
}

void Brick::SetTransparency(bool value) {
    m_IsTransparent = value;
}

void Brick::Eliminate() {
    ScoringSystem::GetInstance()->AddPoints(m_Value);
    m_IsDissolving = true;
    m_Collider->enabled = false;
}

void Brick::OnStart() {
    Mesh* brickMesh = g_ResourceManager->GetResource<Mesh>("Brick");
    Material* brickMaterial = g_ResourceManager->GetResource<Material>("BrickMaterial");
    PBRMaterial* pbrBrickMaterial = (PBRMaterial*)brickMaterial;

    m_DissolveMaterial->albedo = Math::Vec3(0.16470588235f, 0.81568627451f, 0.76078431372f);
    m_DissolveMaterial->albedoMap = pbrBrickMaterial->albedoMap;
    m_DissolveMaterial->normalMap = pbrBrickMaterial->normalMap;
    m_DissolveMaterial->roughnessMap = pbrBrickMaterial->roughnessMap;
    m_DissolveMaterial->metalnessMap = pbrBrickMaterial->metalnessMap;
    m_DissolveMaterial->tiling = Math::Vec2(0.125f);
    m_DissolveMaterial->threshold = 0.0f;

    m_RenderComponent = entity.AddComponent<RenderComponent>();
    m_RenderComponent->materials.push_back(m_DissolveMaterial);
    m_RenderComponent->mesh = brickMesh;
    m_RenderComponent->SetTransparency(true);
    m_RenderComponent->opaqueness = 1.0f;
    
    m_Collider = entity.AddComponent<CylinderBrickCollider>(5.0f, 4.0f, 1.0f, 15.0f);
    m_Collider->enabled = false;
    m_RigidBody = entity.AddComponent<RigidBody>();
    m_RigidBody->SetMass(0.0f);
}

void Brick::OnCollisionEnter(Collider& other) {
    if (other.entity.name == "Ball") {
        ScoringSystem::GetInstance()->AddPoints(m_Value);
        m_IsDissolving = true;
        m_Collider->enabled = false;
    }
}

void Brick::OnTriggerEnter(Collider& other) {
    if (other.entity.name == "Ball") {
        ScoringSystem::GetInstance()->AddPoints(m_Value);
        m_IsDissolving = true;
        m_Collider->enabled = false;
    }
}

void Brick::StartReplacement(i32 index, f32 targetScale) {
    i32 layer = index % 24;
    m_Index = index;
    m_IsReplacing = true;

    f32 radius = 4.5f;

    m_TargetPosition = transform.GetPosition() - transform.GetForward()*0.75f - transform.GetUp();
    m_TargetScale = targetScale;
    m_TotalDist = Math::Length(transform.GetForward()*0.75f + transform.GetUp());
}

void Brick::StartAppearing() {
    m_IsAppearing = true;
}

void Brick::OnUpdate(f32 deltaTime) {
    f32 radius = 4.5f;

    if (m_IsAppearing) {
        m_DissolveMaterial->threshold += deltaTime*0.75f;
        if (m_DissolveMaterial->threshold > 1.0f) {
            m_DissolveMaterial->threshold = 1.0f;


            m_Collider->enabled = true;
            m_IsAppearing = false;
        }
    }
    else if (m_IsDissolving) {
        m_DissolveMaterial->threshold -= deltaTime*2.0f;
        if (m_DissolveMaterial->threshold < 0.0f) {
            m_DissolveMaterial->threshold = 0.0f;
            m_Tower->ReleaseBrick(m_Index);
            scene.DestroyEntity(&entity);
            m_IsDissolving = false;
        }
    }

    if (m_IsTransparent) {
        if (m_RenderComponent->opaqueness > 0.35f) {
            m_RenderComponent->opaqueness -= deltaTime;
        }
        else {
            m_RenderComponent->opaqueness = 0.35f;
        }
    }
    else {
        if (m_RenderComponent->opaqueness < 1.0f) {
            m_RenderComponent->opaqueness += deltaTime;
        }
        else {
            m_RenderComponent->opaqueness = 1.0f;
        }
    }

    if (m_IsReplacing) {
        Math::Vec3 pos = transform.GetPosition();
        f32 dist = Math::Length(pos - m_TargetPosition);
        if (dist < 0.02f) {
            m_IsReplacing = false;
            transform.SetPosition(m_TargetPosition);
            transform.SetScale(m_TargetScale, 1.0f, m_TargetScale);
            m_Tower->Replace(m_Index);
            return;
        }
        Math::Vec3 velocity = Math::Normalize(m_TargetPosition - pos)*4.0f;
        transform.SetPosition(pos + velocity*deltaTime);

        Math::Vec3 scale = transform.GetScale();
        f32 t = (m_TotalDist - dist)/m_TotalDist;
        transform.SetScale(Math::Lerp(scale, Math::Vec3(m_TargetScale, 1.0f, m_TargetScale), t));
    }
}

BonusBrick::BonusBrick(Fly::Entity& entity, Tower* tower, i32 index): 
    Brick(entity, tower, index) 
{
    m_Value = 50;
}

void BonusBrick::SpawnRandomBonus() {
    Entity* bonus = scene.CreateEntity("Bonus");
    Math::Vec3 dir = Math::Normalize(transform.GetPosition());

    i32 value = Math::RandomInt(0, 99);
    if (value < 3) {
        bonus->AddComponent<ExtraLife>(transform.GetPosition(), dir*5.0f);
    }
    else if (value >= 3 && value < 23) {
        bonus->AddComponent<ExtraPoints>(transform.GetPosition(), dir*5.0f);
    }
    else if (value >=23 && value < 39) {
        bonus->AddComponent<IncreaseBallSpeed>(transform.GetPosition(), dir*5.0f);
    }
    else if (value >=39 && value < 55) {
        bonus->AddComponent<DecreaseBallSpeed>(transform.GetPosition(), dir*5.0f);
    }
    else if (value >=55 && value < 71) {
        bonus->AddComponent<IncreasePaddle>(transform.GetPosition(), dir*5.0f);
    }
    else if (value >=71 && value < 87) {
        bonus->AddComponent<DecreasePaddle>(transform.GetPosition(), dir*5.0f);
    }
    else {
        bonus->AddComponent<TemporaryPierce>(transform.GetPosition(), dir*5.0f);
    }
}

void BonusBrick::Eliminate() {
    ScoringSystem::GetInstance()->AddPoints(m_Value);
    SpawnRandomBonus();
    m_IsDissolving = true;
    m_Collider->enabled = false;
}

void BonusBrick::OnCollisionEnter(Collider& other) {
    if (other.entity.name == "Ball") {
        ScoringSystem::GetInstance()->AddPoints(m_Value);
        SpawnRandomBonus();
        m_IsDissolving = true;
        m_Collider->enabled = false;
    }
}

void BonusBrick::OnTriggerEnter(Collider& other) {
    if (other.entity.name == "Ball") {
        ScoringSystem::GetInstance()->AddPoints(m_Value);
        SpawnRandomBonus();
        m_IsDissolving = true;
        m_Collider->enabled = false;
    }
}

void BonusBrick::OnStart() {
    Mesh* brickMesh = g_ResourceManager->GetResource<Mesh>("Brick");
    Material* brickMaterial = g_ResourceManager->GetResource<Material>("BrickMaterial");
    PBRMaterial* pbrBrickMaterial = (PBRMaterial*)brickMaterial;

    m_DissolveMaterial->albedo = Math::Vec3(0.91372549019f, 0.86862745098f, 0.21568627451f);
    m_DissolveMaterial->albedoMap = pbrBrickMaterial->albedoMap;
    m_DissolveMaterial->normalMap = pbrBrickMaterial->normalMap;
    m_DissolveMaterial->roughnessMap = pbrBrickMaterial->roughnessMap;
    m_DissolveMaterial->metalnessMap = pbrBrickMaterial->metalnessMap;
    m_DissolveMaterial->tiling = Math::Vec2(0.125f);
    m_DissolveMaterial->threshold = 0.0f;

    m_RenderComponent = entity.AddComponent<RenderComponent>();
    m_RenderComponent->materials.push_back(m_DissolveMaterial);
    m_RenderComponent->mesh = brickMesh;
    m_RenderComponent->SetTransparency(true);
    m_RenderComponent->opaqueness = 1.0f;
    
    m_Collider = entity.AddComponent<CylinderBrickCollider>(5.0f, 4.0f, 1.0f, 15.0f);
    m_Collider->enabled = false;
    m_RigidBody = entity.AddComponent<RigidBody>();
    m_RigidBody->SetMass(0.0f);
}

ToughBrick::ToughBrick(Fly::Entity& entity, Tower* tower, i32 index): 
    Brick(entity, tower, index) 
{
    m_Value = 200;
}

void ToughBrick::OnCollisionEnter(Collider& other) {
    if (other.entity.name == "Ball") {
        m_Lives--;
        if (m_Lives == 0) {
            ScoringSystem::GetInstance()->AddPoints(m_Value);
            m_IsDissolving = true;
            m_Collider->enabled = false;
            return;
        }
        m_Animate = true;
        m_NewThreshold -= 0.35f;
    }
}

void ToughBrick::OnTriggerEnter(Collider& other) {
    if (other.entity.name == "Ball") {
        m_Lives = 0;
        ScoringSystem::GetInstance()->AddPoints(m_Value);
        m_IsDissolving = true;
        m_Collider->enabled = false;
    }
}

void ToughBrick::OnStart() {
    Mesh* brickMesh = g_ResourceManager->GetResource<Mesh>("Brick");
    Material* brickMaterial = g_ResourceManager->GetResource<Material>("BrickMaterial");
    PBRMaterial* pbrBrickMaterial = (PBRMaterial*)brickMaterial;

    m_DissolveMaterial->albedo = Math::Vec3(0.95686274509f, 0.3529411764f, 0.38039215686f);
    m_DissolveMaterial->albedoMap = pbrBrickMaterial->albedoMap;
    m_DissolveMaterial->normalMap = pbrBrickMaterial->normalMap;
    m_DissolveMaterial->roughnessMap = pbrBrickMaterial->roughnessMap;
    m_DissolveMaterial->metalnessMap = pbrBrickMaterial->metalnessMap;
    m_DissolveMaterial->tiling = Math::Vec2(0.125f);
    m_DissolveMaterial->threshold = 0.0f;

    m_RenderComponent = entity.AddComponent<RenderComponent>();
    m_RenderComponent->materials.push_back(m_DissolveMaterial);
    m_RenderComponent->mesh = brickMesh;
    m_RenderComponent->SetTransparency(true);
    m_RenderComponent->opaqueness = 1.0f;
    
    m_Collider = entity.AddComponent<CylinderBrickCollider>(5.0f, 4.0f, 1.0f, 15.0f);
    m_Collider->enabled = false;
    m_RigidBody = entity.AddComponent<RigidBody>();
    m_RigidBody->SetMass(0.0f);
}

void ToughBrick::OnUpdate(f32 deltaTime) {
    Brick::OnUpdate(deltaTime);
 
    if (m_Animate) {
        m_DissolveMaterial->threshold -= deltaTime*0.5f;
        if (m_DissolveMaterial->threshold < m_NewThreshold) {
            m_DissolveMaterial->threshold = m_NewThreshold;
            m_Animate = false;
        }   
    }
}

