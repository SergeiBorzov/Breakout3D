#include "free_camera.h"
#include "game_manager.h"
#include "scoring_system.h"
#include "pause_menu.h"
#include "splash_screen.h"
#include "main_menu.h"
#include "game.h"

using namespace Fly;

void Game::LoadSplashScreen() {
    g_Window->ShowCursor(false);
    g_ResourceManager->GetResource<Font>("Assets/Fonts/arial.ttf", 120.0f);
    Entity* splashScreen = m_Scene.CreateEntity("SplashScreen");
    splashScreen->AddComponent<SplashScreen>(this);

    Entity* camera = m_Scene.CreateEntity("Camera");
    CameraComponent* cameraComponent = camera->AddComponent<CameraComponent>();
    i32 width = g_Window->GetWidth();
    i32 height = g_Window->GetHeight();
    CameraComponent::s_MainCamera = cameraComponent;
}

void Game::LoadMainResources() {
        // Fonts 
        g_ResourceManager->GetResource<Font>("Assets/Fonts/Alphakind.ttf", 48.0f);
        g_ResourceManager->GetResource<Font>("Assets/Fonts/Alphakind.ttf", 170.0f);

        // Meshes
        g_ResourceManager->GetResource<Mesh>("Assets/Meshes/increasePaddle.obj");
        g_ResourceManager->GetResource<Mesh>("Assets/Meshes/decreasePaddle.obj");
        g_ResourceManager->GetResource<Mesh>("Assets/Meshes/increaseSpeed.obj");
        g_ResourceManager->GetResource<Mesh>("Assets/Meshes/decreaseSpeed.obj");
        g_ResourceManager->GetResource<Mesh>("Assets/Meshes/heart.obj");
        g_ResourceManager->GetResource<Mesh>("Assets/Meshes/coin.obj");
        g_ResourceManager->GetResource<Mesh>("Assets/Meshes/pierce.obj");

        g_ResourceManager->AddResource("Paddle0", Mesh::CreateCylinderBrick(15.0, 14.0, 1.0, 20.0f, 20));
        g_ResourceManager->AddResource("Paddle1", Mesh::CreateCylinderBrick(15.0, 14.0, 1.0, 25.0f, 20));
        g_ResourceManager->AddResource("Paddle2", Mesh::CreateCylinderBrick(15.0, 14.0, 1.0, 30.0f, 20));
        g_ResourceManager->AddResource("Paddle3", Mesh::CreateCylinderBrick(15.0, 14.0, 1.0, 35.0f, 20));
        g_ResourceManager->AddResource("Paddle4", Mesh::CreateCylinderBrick(15.0, 14.0, 1.0, 40.0f, 20));
        g_ResourceManager->AddResource("Paddle5", Mesh::CreateCylinderBrick(15.0, 14.0, 1.0, 45.0f, 20));
        g_ResourceManager->AddResource("Paddle6", Mesh::CreateCylinderBrick(15.0, 14.0, 1.0, 50.0f, 20));
        g_ResourceManager->AddResource("Paddle7", Mesh::CreateCylinderBrick(15.0, 14.0, 1.0, 55.0f, 20));
        g_ResourceManager->AddResource("Paddle8", Mesh::CreateCylinderBrick(15.0, 14.0, 1.0, 60.0f, 20));
        g_ResourceManager->AddResource("Brick", Mesh::CreateCylinderBrick(5.0f, 4.0f, 1.0f, 15.0f, 20));
        g_ResourceManager->AddResource("Sphere", Mesh::CreateSphere(0.5f, 100, 30));
        g_ResourceManager->AddResource("Box", Mesh::CreateBox(1.0f, 1.0f, 1.0f));

        // Textures
        g_ResourceManager->AddResource("BlackTexture", Texture::CreateBlackTexture());
        Texture* noiseTex = g_ResourceManager->GetResource<Texture>("Assets/Textures/Noise/noise.png");

        Texture* paddleTex = g_ResourceManager->GetResource<Texture>("Assets/Textures/Paddle/Albedo.png");
        Texture* paddleNormalTex = g_ResourceManager->GetResource<Texture>("Assets/Textures/Paddle/Normal.png");
        Texture* paddleRoughnessTex = g_ResourceManager->GetResource<Texture>("Assets/Textures/Paddle/Roughness.png");
        Texture* paddleMetalnessTex = g_ResourceManager->GetResource<Texture>("BlackTexture");

        Texture* brickTex = g_ResourceManager->GetResource<Texture>("Assets/Textures/Brick/Albedo.png");
        Texture* brickNormalTex = g_ResourceManager->GetResource<Texture>("Assets/Textures/Brick/Normal.png");
        Texture* brickRoughnessTex = g_ResourceManager->GetResource<Texture>("Assets/Textures/Brick/Roughness.png");
        Texture* brickMetalnessTex = g_ResourceManager->GetResource<Texture>("BlackTexture");

        Texture* floorTex = g_ResourceManager->GetResource<Texture>("Assets/Textures/Floor/Albedo.png");
        Texture* floorNormalTex = g_ResourceManager->GetResource<Texture>("Assets/Textures/Floor/Normal.png");
        Texture* floorRoughnessTex = g_ResourceManager->GetResource<Texture>("Assets/Textures/Floor/Roughness.png");
        Texture* floorMetalnessTex = g_ResourceManager->GetResource<Texture>("BlackTexture");

        Texture* ballTex = g_ResourceManager->GetResource<Texture>("Assets/Textures/Ball/Albedo.png");
        Texture* ballNormalTex = g_ResourceManager->GetResource<Texture>("Assets/Textures/Ball/Normal.png");
        Texture* ballRoughnessTex = g_ResourceManager->GetResource<Texture>("Assets/Textures/Ball/Roughness.png");
        Texture* ballMetalnessTex = g_ResourceManager->GetResource<Texture>("BlackTexture");


        // Materials
        PBRMaterial* paddleMaterial = new PBRMaterial();
        paddleMaterial->albedoMap = paddleTex;
        paddleMaterial->normalMap = paddleNormalTex;
        paddleMaterial->roughnessMap = paddleRoughnessTex;
        paddleMaterial->metalnessMap = paddleMetalnessTex;
        paddleMaterial->tiling = Math::Vec2(0.5f, 0.125f);
        g_ResourceManager->AddResource<Material>("PaddleMaterial", paddleMaterial);

        PBRMaterial* floorMaterial = new PBRMaterial();
        floorMaterial->albedoMap = floorTex;
        floorMaterial->normalMap = floorNormalTex;
        floorMaterial->roughnessMap = floorRoughnessTex;
        floorMaterial->metalnessMap = floorMetalnessTex;
        floorMaterial->tiling = Math::Vec2(2.0f, 2.0f);
        g_ResourceManager->AddResource<Material>("FloorMaterial", floorMaterial);

        PBRMaterial* brickMaterial = new PBRMaterial();
        //brickMaterial->albedo = Math::Vec3(1.0f, 0.2882f, 0.80784f);
        //brickMaterial->albedo = Math::Vec3(0.929f, 0.333f, 0.231f);
        brickMaterial->albedoMap = brickTex;
        brickMaterial->normalMap = brickNormalTex;
        brickMaterial->roughnessMap = brickRoughnessTex;
        brickMaterial->metalnessMap = brickMetalnessTex;
        brickMaterial->tiling = Math::Vec2(0.125f);
        g_ResourceManager->AddResource<Material>("BrickMaterial", brickMaterial);

        PBRMaterial* bonusBrickMaterial = new PBRMaterial();
        //bonusBrickMaterial->albedo = Math::Vec3(0.74f, 0.2882f, 0.80784f);
        bonusBrickMaterial->albedo = Math::Vec3(0.929f, 0.333f, 0.231f);
        bonusBrickMaterial->albedoMap = brickTex;
        bonusBrickMaterial->normalMap = brickNormalTex;
        bonusBrickMaterial->roughnessMap = brickRoughnessTex;
        bonusBrickMaterial->metalnessMap = brickMetalnessTex;
        bonusBrickMaterial->tiling = Math::Vec2(0.125f);
        g_ResourceManager->AddResource<Material>("BonusBrickMaterial", bonusBrickMaterial);

        PBRMaterial* ballMaterial = new PBRMaterial();
        ballMaterial->albedo = Math::Vec3(1.0f, 0.0f, 0.0f);
        ballMaterial->albedoMap = ballTex;
        ballMaterial->normalMap = ballNormalTex;
        ballMaterial->roughnessMap = ballRoughnessTex;
        ballMaterial->metalnessMap = ballMetalnessTex;
        ballMaterial->tiling = Math::Vec2(1.0f);
        g_ResourceManager->AddResource<Material>("BallMaterial", ballMaterial);

        PBRMaterial* extraPointsMaterial = new PBRMaterial();
        extraPointsMaterial->albedo = Math::Vec3(1.0f, 1.0f, 0.0f);
        extraPointsMaterial->albedoMap = ballTex;
        extraPointsMaterial->normalMap = ballNormalTex;
        extraPointsMaterial->roughnessMap = ballRoughnessTex;
        extraPointsMaterial->metalnessMap = ballMetalnessTex;
        extraPointsMaterial->tiling = Math::Vec2(1.0f);
        g_ResourceManager->AddResource<Material>("ExtraPointsMaterial", extraPointsMaterial);

        PBRMaterial* buffBonusMaterial = new PBRMaterial();
        buffBonusMaterial->albedo = Math::Vec3(0.0f, 1.0f, 0.0f);
        buffBonusMaterial->albedoMap = ballTex;
        buffBonusMaterial->normalMap = ballNormalTex;
        buffBonusMaterial->roughnessMap = ballRoughnessTex;
        buffBonusMaterial->metalnessMap = ballMetalnessTex;
        buffBonusMaterial->tiling = Math::Vec2(1.0f);
        g_ResourceManager->AddResource<Material>("BuffBonusMaterial", buffBonusMaterial);

        PBRMaterial* debuffBonusMaterial = new PBRMaterial();
        debuffBonusMaterial->albedo = Math::Vec3(1.0f, 0.0f, 0.0f);
        debuffBonusMaterial->albedoMap = ballTex;
        debuffBonusMaterial->normalMap = ballNormalTex;
        debuffBonusMaterial->roughnessMap = ballRoughnessTex;
        debuffBonusMaterial->metalnessMap = ballMetalnessTex;
        debuffBonusMaterial->tiling = Math::Vec2(1.0f);
        g_ResourceManager->AddResource<Material>("DebuffBonusMaterial", debuffBonusMaterial);

        PBRMaterial* pierceBonusMaterial = new PBRMaterial();
        pierceBonusMaterial->albedo = Math::Vec3(0.0f, 0.0f, 1.0f);
        pierceBonusMaterial->albedoMap = ballTex;
        pierceBonusMaterial->normalMap = ballNormalTex;
        pierceBonusMaterial->roughnessMap = ballRoughnessTex;
        pierceBonusMaterial->metalnessMap = ballMetalnessTex;
        pierceBonusMaterial->tiling = Math::Vec2(1.0f);
        g_ResourceManager->AddResource<Material>("PierceBonusMaterial", pierceBonusMaterial);
}

void Game::NewGame() {
    m_Scene.ClearScene();
    m_CurrentState = State::Active;
    g_Window->ShowCursor(false);
    g_Renderer->SetBlur(false);

    // Game manager
    Entity* gameManager = m_Scene.CreateEntity("GameManager");
    gameManager->AddComponent<GameManager>(this);

    // Scoring system
    Entity* scoringSystem = m_Scene.CreateEntity("ScoringSystem");
    scoringSystem->AddComponent<ScoringSystem>();

    // Pause menu
    Entity* pauseMenu = m_Scene.CreateEntity("PauseMenu");
    m_PauseMenu = pauseMenu->AddComponent<PauseMenu>(this);
}

void Game::Start() {
    LoadSplashScreen();
}

void Game::ResetMainMenu() {
    m_CurrentState = State::MainMenuState;
    m_Scene.ClearScene();
    g_Renderer->SetBlur(true);
    Entity* mainMenu = m_Scene.CreateEntity("MainMenu");
    mainMenu->AddComponent<MainMenu>(this);
    g_Window->ShowCursor(true);
    m_TimeMultiplier = 1.0f;
    m_Pause = false;
}

void Game::LoadMainMenu() {
    LoadMainResources();
    ResetMainMenu();
}

void Game::GameOverState() {
    m_CurrentState = State::GameOver;
}

void Game::ResumeGame() {
    m_TimeMultiplier = m_Pause ? 1.0f : 0.0f;
    m_Pause = false;
    m_PauseMenu->Hide();
    m_CurrentState = State::Active;
    g_Window->ShowCursor(false);
    g_Renderer->SetBlur(false);
}

void Game::CloseGame() {
    g_Window->Close();
}

void Game::PauseState() {
    if (Input::GetKeyDown(Key::Escape) && !m_PauseNotReleased) {
        ResumeGame();
        m_PauseNotReleased = true;
    }

    if (Input::GetKeyUp(Key::Escape)) {
        m_PauseNotReleased = false;
    }
}



void Game::ActiveState() {
    if (Input::GetKeyDown(Key::Escape) && !m_PauseNotReleased) {
        m_TimeMultiplier = m_Pause ? 1.0f : 0.0f;
        m_Pause = true;
        m_PauseNotReleased = true;
        m_PauseMenu->Show();
        m_CurrentState = State::Pause;
        g_Renderer->SetBlur(true);
        g_Window->ShowCursor(true);
    }

    if (Input::GetKeyUp(Key::Escape)) {
        m_PauseNotReleased = false;
    }
}

void Game::Update() {
    switch (m_CurrentState) {
        case State::Active: {
            ActiveState();
            break;
        }
        case State::Pause: {
            PauseState();
            break;
        }
        case State::GameOver: {
            if (Input::GetKeyDown(Key::Enter)) {
                NewGame();
            }
        }
    }
}