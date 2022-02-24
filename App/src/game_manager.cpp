#include "game.h"
#include "free_camera.h"
#include "tower.h"
#include "paddles.h"
#include "ball.h"
#include "corner_light.h"
#include "main_light.h"
#include "game_manager.h"

using namespace Fly;

GameManager* GameManager::instance = nullptr;

void GameManager::IncreasePaddles() {
    m_Paddles->IncreasePaddleSize();
}

void GameManager::DecreasePaddles() {
    m_Paddles->DecreasePaddleSize();
}

void GameManager::IncreaseBallSpeed() {
    m_Ball->IncreaseSpeed(3.0f);
}

void GameManager::DecreaseBallSpeed() {
    m_Ball->DecreaseSpeed(3.0f);
}

void GameManager::TemporaryPierce() {
    m_Ball->StartTemporaryPierce();
}

void GameManager::CreateScene() {
    // Camera
    i32 width = g_Window->GetWidth();
    i32 height = g_Window->GetHeight();
    
    Entity* perspectiveCamera = scene.CreateEntity("Camera");
    m_Cameras[0] = perspectiveCamera->AddComponent<CameraComponent>();
    m_Cameras[0]->SetProjection(Math::Perspective(60.0f, (f32)(width)/height, 0.1f, 100.0f));
    m_Cameras[0]->transform.SetPosition({0.0f, 20.0f, 16.0f});
    m_Cameras[0]->transform.SetRotation(Math::Quat::Euler(-60.0f, 0.0f, 0.0f));

    Entity* topCamera = scene.CreateEntity("Camera");
    //topCamera->AddComponent<FreeCamera>();
    m_Cameras[1] = topCamera->AddComponent<CameraComponent>();
    m_Cameras[1]->SetProjection(Math::Perspective(60.0f, (f32)(width)/height, 0.1f, 100.0f));
    m_Cameras[1]->transform.SetPosition({0.0f, 30.0f, 0.0f});
    m_Cameras[1]->transform.SetRotation(Math::Quat::Euler(-89.0f, 0.0f, 0.0f));

    CameraComponent::s_MainCamera = m_Cameras[m_CurrentCameraIndex];
    // Paddles
    Entity* paddles = scene.CreateEntity("Paddles");
    m_Paddles = paddles->AddComponent<Paddles>();

    // Ball
    Entity* ball = scene.CreateEntity("Ball");
    m_Ball = ball->AddComponent<Ball>(this);

    m_MainPaddle = m_Paddles->GetMainPaddle();
    m_Ball->transform.SetParent(&m_MainPaddle->transform);
    m_Ball->transform.SetLocalPosition(0.0f, 0.0f, -1.0f);

    // Tower
    Entity* towerEntity = scene.CreateEntity("Tower");
    m_Tower = towerEntity->AddComponent<Tower>();

    //Entity* testBrick = scene.CreateEntity("TestBrick");
    //RenderComponent* rc = testBrick->AddComponent<RenderComponent>();
    //RigidBody* rb = testBrick->AddComponent<RigidBody>();
    //rb->SetMass(0.0f);
    //testBrick->AddComponent<CylinderBrickCollider>(5.0f, 4.0f, 1.0f, 15.0f);
    //testBrick->transform.SetPosition(0.0f, 0.0f, 1.0f);
    //Mesh* brickMesh = g_ResourceManager->GetResource<Mesh>("Brick");
    //Material* brickMaterial = g_ResourceManager->GetResource<Material>("BrickMaterial");
    //rc->mesh = brickMesh;
    //rc->materials.push_back(brickMaterial);
    



    // Floor
    Entity* floor = scene.CreateEntity("Floor");
    RenderComponent* floor_rc = 
        floor->AddComponent<RenderComponent>(
            g_ResourceManager->GetResource<Mesh>("Box"), 
            g_ResourceManager->GetResource<Material>("FloorMaterial")
        );
    floor->transform.SetPosition(0.0f, -1.0f, 0.0f);
    floor->transform.SetScale(40.0f, 0.1f, 40.0f);
    
    // Main Lights
    Entity* mainSpotLight = scene.CreateEntity("Spotlight");
    m_SpotLight = mainSpotLight->AddComponent<MainLight>(
                      Math::Vec3(0.0f, 15.0f, 0.0f),
                      Math::Vec3(1600.0f)
                  );

    Entity* magentaPointLight = scene.CreateEntity("RedPointLight");
    m_CornerLights[0] = magentaPointLight->AddComponent<CornerLight>(
                            Math::Vec3(-15.0f, 5.0f, -15.0f), 
                            Math::Vec3(400/Math::Sqrt(2), 0.0f, 400/Math::Sqrt(2))
                        );

    Entity* greenPointLight = scene.CreateEntity("GreenPointLight");
    m_CornerLights[1] = greenPointLight->AddComponent<CornerLight>(
                            Math::Vec3(-15.0f, 5.0f, 15.0f), 
                            Math::Vec3(0.0f, 400.0f, 0.0f)
                        );

    Entity* bluePointLight = scene.CreateEntity("BluePointLight");
    m_CornerLights[2] = bluePointLight->AddComponent<CornerLight>(
                            Math::Vec3(15.0f, 5.0f, -15.0f), 
                            Math::Vec3(0.0f, 0.0f, 400.0f)
                        );


    Entity* yellowPointLight = scene.CreateEntity("YellowPointLight");
    m_CornerLights[3] = yellowPointLight->AddComponent<CornerLight>(
                    Math::Vec3(15.0f, 5.0f, 15.0f),
                    Math::Vec3(400/Math::Sqrt(2), 400/Math::Sqrt(2), 0.0f) 
                );

    // Life Label
    Font* font = g_ResourceManager->GetResource<Font>("Assets/Fonts/Alphakind.ttf48", 48.0f);
    Font* largeFont = g_ResourceManager->GetResource<Font>("Assets/Fonts/Alphakind.ttf170", 170.0f);

    Entity* lifeLabel = scene.CreateEntity("LifeLabel");
    m_LifeLabel = lifeLabel->AddComponent<UI::Label>();
    m_LifeLabel->SetFont(font);
    m_LifeLabel->hasBackground = false;
    m_LifeLabel->units = UI::Units::Percents;
    m_LifeLabel->horizontalAlignment = UI::Alignment::Left;
    m_LifeLabel->size = Math::Vec2(0.2f, 0.05f);
    m_LifeLabel->text = "Lives: " + std::to_string(m_LivesLeft);
    m_LifeLabel->textColor = Color(255, 255, 255, 255);
    m_LifeLabel->position = Math::Vec3(0.15f, 0.05f, 0.0f);
    m_LifeLabel->roundness = 0.0f;

    Entity* gameOverLabel = scene.CreateEntity("GameOver label");
    m_GameOverLabel = gameOverLabel->AddComponent<UI::Label>();
    m_GameOverLabel->SetFont(largeFont);
    m_GameOverLabel->hasBackground = false;
    m_GameOverLabel->units = UI::Units::Percents;
    m_GameOverLabel->horizontalAlignment = UI::Alignment::Center;
    m_GameOverLabel->size = Math::Vec2(0.8f, 0.4f);
    m_GameOverLabel->text = "";
    m_GameOverLabel->textColor = Color(255, 255, 255, 255);
    m_GameOverLabel->position = Math::Vec3(0.5f, 0.5f, 0.0f);
    m_LifeLabel->roundness = 0.0f;
}

void GameManager::OnStart() {
    instance = this;
    CreateScene();
}

void GameManager::OnUpdate(f32 deltaTime) {
    if (Input::GetKey(Key::Space) && !m_BallReleased) {
        m_Ball->transform.SetParent(nullptr);
        m_Ball->Release(8.0f*m_MainPaddle->transform.GetForward() + m_MainPaddle->transform.GetRight());
        m_BallReleased = true;
    }

    if (Input::GetKeyDown(Key::C)) {
        m_CurrentCameraIndex = (m_CurrentCameraIndex + 1) % 2;
        CameraComponent::s_MainCamera = m_Cameras[m_CurrentCameraIndex];
    }

    Math::Vec4 vs = CameraComponent::s_MainCamera->GetView()*Math::Vec4(m_Ball->transform.GetPosition(), 1.0f);
    Math::Vec4 cs = CameraComponent::s_MainCamera->GetProjection()*vs;
    Math::Vec3 ndc = Math::Vec3(cs)/cs.w;

    i32 x = (i32)((ndc.x + 1.0f)*g_Window->GetWidth()*0.5f);
    i32 y = (i32)((ndc.y + 1.0f)*g_Window->GetHeight()*0.5f);

    if (Math::Length(m_Ball->transform.GetPosition()) < 3.7f) {
        m_Tower->SetBrickTransparency(true);
    }
    else {
        m_Tower->SetBrickTransparency(false);
    }

    f32 R = 20.0f + 0.5f;
    if (Math::LengthSqr(m_Ball->transform.GetPosition()) > R*R) {
        ResetBall();
    }

    m_LifeLabel->text = "Lives: " + std::to_string(m_LivesLeft);
}

void GameManager::ResetBall() {
    m_LivesLeft--;
    if (m_LivesLeft > 0) {
        for (int i = 0; i < 4; i++) {
            m_CornerLights[i]->StartBlinking();
        }
        m_SpotLight->StartBlinking();

        m_Ball->transform.SetParent(&m_MainPaddle->transform);
        m_Ball->transform.SetLocalPosition(0.0f, 0.0f, -1.0f);
        m_Ball->Withhold();
        m_BallReleased = false;
    }
    else {
        m_Paddles->SetActive(false);
        m_Ball->transform.SetParent(&m_MainPaddle->transform);
        m_Ball->transform.SetLocalPosition(0.0f, 0.0f, -1.0f);
        m_Ball->Withhold();

        for (int i = 0; i < 4; i++) {
            m_CornerLights[i]->SetIntensity(Math::Vec3(0.0f));
        }
        m_SpotLight->SetIntensity(Math::Vec3(100.0f, 0.0f, 0.0f));
        m_GameOverLabel->text = "Game Over!";
        // Enable blur
        g_Renderer->SetBlur(true);
        // Show game over label
        m_Game->GameOverState();
    }
}


