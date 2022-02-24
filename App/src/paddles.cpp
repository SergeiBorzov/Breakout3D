
#include "paddles.h"
#include "ball.h"

using namespace Fly;

Paddles::Paddles(Fly::Entity& entity): Fly::NativeScript(entity) {
    for (i32 i = 0; i < 9; i++) {
        std::string name = "Paddle" + std::to_string(i);
        m_PaddleMeshes[i] = g_ResourceManager->GetResource<Mesh>(name.c_str());
    }
    Mesh* sphereMesh = g_ResourceManager->GetResource<Mesh>("Sphere");
    Material* paddleMaterial = g_ResourceManager->GetResource<Material>("PaddleMaterial");

    f32 radius = 0.5f*(m_DefaultPaddleBigRadius + m_DefaultPaddleSmallRadius);
    f32 deltaAngle = 2.0f*Math::PI/m_PaddleCount; 


    for (i32 i = 0; i < m_PaddleCount; i++) {
        m_Paddles[i] = scene.CreateEntity("Paddle");
        
        m_RenderComponents[i] = m_Paddles[i]->AddComponent<RenderComponent>();
        m_RenderComponents[i]->materials.push_back(paddleMaterial);
        m_RenderComponents[i]->mesh = m_PaddleMeshes[m_PaddleSize];
        m_PaddleColliders[i] = 
            m_Paddles[i]->AddComponent<CylinderBrickCollider>(
                m_DefaultPaddleBigRadius, m_DefaultPaddleSmallRadius, 
                m_DefaultPaddleHeight, m_DefaultPaddleAngle
            );
        RigidBody* paddleRB = m_Paddles[i]->AddComponent<RigidBody>();
        paddleRB->SetMass(0.0f);
        
        m_Paddles[i]->transform.SetParent(&transform);
        m_Paddles[i]->transform.SetPosition(radius*Math::Sin(i*deltaAngle), 0.0f, radius*Math::Cos(i*deltaAngle));
        m_Paddles[i]->transform.Rotate(0.0f, Math::Degrees(i*deltaAngle), 0.0f);
    }
}

void Paddles::IncreasePaddleSize() {
    if (m_PaddleSize < 8) {
        m_PaddleSize++;
        for (int i = 0; i < m_PaddleCount; i++) {
            m_RenderComponents[i]->mesh = m_PaddleMeshes[m_PaddleSize];
            m_PaddleColliders[i]->angle += Math::Radians(5.0f);
        }
    }
}

void Paddles::DecreasePaddleSize() {
    if (m_PaddleSize > 0) {
        m_PaddleSize--;
        for (int i = 0; i < m_PaddleCount; i++) {
            m_RenderComponents[i]->mesh = m_PaddleMeshes[m_PaddleSize];
            m_PaddleColliders[i]->angle -= Math::Radians(5.0f);
        }
    }
}

void Paddles::OnFixedUpdate(f32 fixedDeltaTime) {
    if (!active) {
        return;
    }
    
    if (Input::GetKey(Key::LeftArrow)) {
        transform.Rotate(0.0f, -m_Speed*fixedDeltaTime, 0.0f);
        for (int i = 1; i < 3; i++) {
            RigidBody* rb = m_Paddles[i]->GetComponent<RigidBody>();
            Math::Vec3 pos = m_Paddles[i]->transform.GetPosition();
        }
    }
    if (Input::GetKey(Key::RightArrow)) {
        transform.Rotate(0.0f, m_Speed*fixedDeltaTime, 0.0f);
        for (int i = 1; i < 3; i++) {
            RigidBody* rb = m_Paddles[i]->GetComponent<RigidBody>();
            Math::Vec3 pos = m_Paddles[i]->transform.GetPosition();
        }
    }
}

void Paddles::OnUpdate(f32 deltaTime) {

}