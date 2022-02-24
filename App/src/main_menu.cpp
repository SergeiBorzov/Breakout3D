#include "game.h"
#include "main_light.h"
#include "corner_light.h"
#include "tower.h"
#include "main_menu.h"

using namespace Fly;

i32 MainMenu::s_CurrentResolution = 7;
bool MainMenu::s_FullScreen = true;
i32 MainMenu::s_PrevResolution = s_CurrentResolution;
bool MainMenu::s_PrevFullScreen = s_FullScreen;


MainMenu::MainMenu(Entity& entity, Game* game): NativeScript(entity), m_Game(game) {}


static void OnMouseHoverStart(UI::Button* button) {
    button->textColor = Color(255, 0, 0, 255);
}

static void OnMouseHoverEnd(UI::Button* button) {
    button->textColor = Color(255, 255, 255, 255);
}

static void OnNewGameButton(Game* game) {
    game->NewGame();
}

static void OnExitButton(Game* game) {
    game->CloseGame();
}

static void OnSettingsButton(MainMenu* menu) {
    menu->ShowSettings();
}

static void OnBackToMenuButton(MainMenu* menu) {
    menu->DiscardChanges();
    menu->HideSettings();
}

static void OnApplyButton(MainMenu* menu) {
    menu->ApplyChanges();
    menu->HideSettings();
}

static void OnHigherResolutionButton(MainMenu* menu) {
    menu->HigherResolution();
}

static void OnLowerResolutionButton(MainMenu* menu) {
    menu->LowerResolution();
}

static void OnFullScreenButton(MainMenu* menu) {
    menu->FullScreenButton();
}

void MainMenu::FullScreenButton() {
    s_FullScreen = !s_FullScreen;
    if (s_FullScreen) {
        m_FullScreenButton->text = "Fullscreen - Yes";
    }
    else {
        m_FullScreenButton->text = "Fullscreen - No";
    }
}

void MainMenu::DiscardChanges() {
    s_FullScreen = s_PrevFullScreen;
    s_CurrentResolution = s_PrevResolution;
}

void MainMenu::ApplyChanges() {
    if (s_FullScreen) {
        if (s_CurrentResolution != s_PrevResolution || s_FullScreen && !s_PrevFullScreen) {
            g_Window->SetFullScreen(m_Resolutions[s_CurrentResolution].width, m_Resolutions[s_CurrentResolution].height);
            s_PrevResolution = s_CurrentResolution;
            s_PrevFullScreen = s_FullScreen;
        }
    }
    else {
        if (s_CurrentResolution != s_PrevResolution || !s_FullScreen && s_PrevFullScreen) {
            g_Window->SetWindowedScreen(m_Resolutions[s_CurrentResolution].width, m_Resolutions[s_CurrentResolution].height);
            s_PrevResolution = s_CurrentResolution;
            s_PrevFullScreen = s_FullScreen;
        }
    }
}

void MainMenu::HigherResolution() {
    s_CurrentResolution = Math::Min(s_CurrentResolution + 1, 7);
    m_ResolutionLabel->text = m_ResolutionStrings[s_CurrentResolution];
}

void MainMenu::LowerResolution() {
    s_CurrentResolution = Math::Max(s_CurrentResolution - 1, 0);
    m_ResolutionLabel->text = m_ResolutionStrings[s_CurrentResolution];
}

void MainMenu::ShowSettings() {
    m_NewGameButtonEntity->SetActive(false);
    m_QuitButtonEntity->SetActive(false);
    m_SettingsButtonEntity->SetActive(false);

    m_BackgroundEntity->SetActive(true);
    m_ResolutionEntity->SetActive(true);
    m_LowerResolutionEntity->SetActive(true);
    m_HigherResolutionEntity->SetActive(true);
    m_BackToMenuEntity->SetActive(true);
    m_ApplyEntity->SetActive(true);
    m_FullScreenEntity->SetActive(true);
}

void MainMenu::HideSettings() {
    m_NewGameButtonEntity->SetActive(true);
    m_QuitButtonEntity->SetActive(true);
    m_SettingsButtonEntity->SetActive(true);

    m_BackgroundEntity->SetActive(false);
    m_ResolutionEntity->SetActive(false);
    m_LowerResolutionEntity->SetActive(false);
    m_HigherResolutionEntity->SetActive(false);
    m_BackToMenuEntity->SetActive(false);
    m_ApplyEntity->SetActive(false);
    m_FullScreenEntity->SetActive(false);
}

void MainMenu::OnStart() {
    /* Buttons */
    Font* font = g_ResourceManager->GetResource<Font>("Assets/Fonts/Alphakind.ttf48", 48.0f);

    m_NewGameButtonEntity = scene.CreateEntity("New game button");
    m_NewGameButton = m_NewGameButtonEntity->AddComponent<UI::Button>();
    m_NewGameButton->SetFont(font);
    m_NewGameButton->hasBackground = true;
    m_NewGameButton->units = UI::Units::Percents;
    m_NewGameButton->horizontalAlignment = UI::Alignment::Center;
    m_NewGameButton->size = Math::Vec2(0.24f, 0.1f);
    m_NewGameButton->text = "New game ";
    m_NewGameButton->textColor = Color(255, 255, 255, 255);
    m_NewGameButton->position = Math::Vec3(0.15f, 0.69f, 0.0f);
    m_NewGameButton->roundness = 15.0f;
    m_NewGameButton->onMouseHoverStart = std::bind(OnMouseHoverStart, m_NewGameButton);
    m_NewGameButton->onMouseHoverEnd = std::bind(OnMouseHoverEnd, m_NewGameButton);
    m_NewGameButton->onMouseClick = std::bind(OnNewGameButton, m_Game);

    m_SettingsButtonEntity = scene.CreateEntity("SettingsButton");
    UI::Button* settingsButton = m_SettingsButtonEntity->AddComponent<UI::Button>();
    settingsButton->SetFont(font);
    settingsButton->hasBackground = true;
    settingsButton->units = UI::Units::Percents;
    settingsButton->horizontalAlignment = UI::Alignment::Center;
    settingsButton->size = Math::Vec2(0.24f, 0.1f);
    settingsButton->text = "Settings";
    settingsButton->textColor = Color(255, 255, 255, 255);
    settingsButton->position = Math::Vec3(0.15f, 0.8f, 0.0f);
    settingsButton->roundness = 15.0f;
    settingsButton->onMouseHoverStart = std::bind(OnMouseHoverStart, settingsButton);
    settingsButton->onMouseHoverEnd = std::bind(OnMouseHoverEnd, settingsButton);
    settingsButton->onMouseClick = std::bind(OnSettingsButton, this);

    m_QuitButtonEntity = scene.CreateEntity("QuitButton");
    UI::Button* quitButton = m_QuitButtonEntity->AddComponent<UI::Button>();
    quitButton->SetFont(font);
    quitButton->hasBackground = true;
    quitButton->units = UI::Units::Percents;
    quitButton->horizontalAlignment = UI::Alignment::Center;
    quitButton->size = Math::Vec2(0.24f, 0.1f);
    quitButton->text = "Quit";
    quitButton->textColor = Color(255, 255, 255, 255);
    quitButton->position = Math::Vec3(0.15f, 0.91f, 0.0f);
    quitButton->roundness = 15.0f;
    quitButton->onMouseHoverStart = std::bind(OnMouseHoverStart, quitButton);
    quitButton->onMouseHoverEnd = std::bind(OnMouseHoverEnd, quitButton);
    quitButton->onMouseClick = std::bind(OnExitButton, m_Game);

    m_BackgroundEntity = scene.CreateEntity("BackgroundEntity");
    m_BackgroundEntity->SetActive(false);
    UI::Label* backgroundLabel = m_BackgroundEntity->AddComponent<UI::Label>();
    backgroundLabel->SetFont(font);
    backgroundLabel->hasBackground = true;
    backgroundLabel->units = UI::Units::Percents;
    backgroundLabel->horizontalAlignment = UI::Alignment::Center;
    backgroundLabel->size = Math::Vec2(0.35f, 0.7f);
    backgroundLabel->text = "";
    backgroundLabel->position = Math::Vec3(0.5f, 0.5f, -1.0f);
    backgroundLabel->roundness = 15.0f;

    m_HigherResolutionEntity = scene.CreateEntity("HigherResolutionEntity");
    m_HigherResolutionEntity->SetActive(false);
    UI::Button* higherResolutionButton = m_HigherResolutionEntity->AddComponent<UI::Button>();
    higherResolutionButton->SetFont(font);
    higherResolutionButton->hasBackground = true;
    higherResolutionButton->units = UI::Units::Percents;
    higherResolutionButton->horizontalAlignment = UI::Alignment::Center;
    higherResolutionButton->size = Math::Vec2(0.03f, 0.08f);
    higherResolutionButton->text = ">";
    higherResolutionButton->textColor = Color(255, 255, 255, 255);
    higherResolutionButton->backgroundColor = Color(0, 0, 80, 255);
    higherResolutionButton->position = Math::Vec3(0.64f, 0.3f, 0.0f);
    higherResolutionButton->roundness = 0.0f;
    higherResolutionButton->onMouseHoverStart = std::bind(OnMouseHoverStart, higherResolutionButton);
    higherResolutionButton->onMouseHoverEnd = std::bind(OnMouseHoverEnd, higherResolutionButton);
    higherResolutionButton->onMouseClick = std::bind(OnHigherResolutionButton, this);

    m_LowerResolutionEntity = scene.CreateEntity("LowerResolutionEntity");
    m_LowerResolutionEntity->SetActive(false);
    UI::Button* lowerResolutionButton = m_LowerResolutionEntity->AddComponent<UI::Button>();
    lowerResolutionButton->SetFont(font);
    lowerResolutionButton->hasBackground = true;
    lowerResolutionButton->units = UI::Units::Percents;
    lowerResolutionButton->horizontalAlignment = UI::Alignment::Center;
    lowerResolutionButton->size = Math::Vec2(0.03f, 0.08f);
    lowerResolutionButton->text = "<";
    lowerResolutionButton->textColor = Color(255, 255, 255, 255);
    lowerResolutionButton->position = Math::Vec3(0.36f, 0.3f, 0.0f);
    lowerResolutionButton->backgroundColor = Color(0, 0, 80, 255);
    lowerResolutionButton->roundness =0.0f;
    lowerResolutionButton->onMouseHoverStart = std::bind(OnMouseHoverStart, lowerResolutionButton);
    lowerResolutionButton->onMouseHoverEnd = std::bind(OnMouseHoverEnd, lowerResolutionButton);
    lowerResolutionButton->onMouseClick = std::bind(OnLowerResolutionButton, this);

    m_FullScreenEntity = scene.CreateEntity("ResolutionEntity");
    m_FullScreenEntity->SetActive(false);
    m_FullScreenButton = m_FullScreenEntity->AddComponent<UI::Button>();
    m_FullScreenButton->SetFont(font);
    m_FullScreenButton->hasBackground = true;
    m_FullScreenButton->units = UI::Units::Percents;
    m_FullScreenButton->horizontalAlignment = UI::Alignment::Center;
    m_FullScreenButton->size = Math::Vec2(0.22f, 0.09f);
    if (s_FullScreen) {
        m_FullScreenButton->text = "Fullscreen - Yes";
    }
    else {
        m_FullScreenButton->text = "Fullscreen - No";
    }
    m_FullScreenButton->backgroundColor = Color(0, 0, 80, 255);
    m_FullScreenButton->textColor = Color(255, 255, 255, 255);
    m_FullScreenButton->position = Math::Vec3(0.5f, 0.4f, 0.0f);
    m_FullScreenButton->roundness = 0.0f;
    m_FullScreenButton->onMouseHover = std::bind(OnMouseHoverStart, m_FullScreenButton);
    m_FullScreenButton->onMouseHoverEnd = std::bind(OnMouseHoverEnd, m_FullScreenButton);
    m_FullScreenButton->onMouseClick = std::bind(OnFullScreenButton, this);

    m_BackToMenuEntity = scene.CreateEntity("BackToMenuEntity");
    m_BackToMenuEntity->SetActive(false);
    UI::Button* backToMenuButton = m_BackToMenuEntity->AddComponent<UI::Button>();
    backToMenuButton->SetFont(font);
    backToMenuButton->hasBackground = true;
    backToMenuButton->units = UI::Units::Percents;
    backToMenuButton->horizontalAlignment = UI::Alignment::Center;
    backToMenuButton->size = Math::Vec2(0.12f, 0.09f);
    backToMenuButton->text = "Back";
    backToMenuButton->textColor = Color(255, 255, 255, 255);
    backToMenuButton->position = Math::Vec3(0.4f, 0.77f, 0.0f);
    backToMenuButton->backgroundColor = Color(0, 0, 80, 255);
    backToMenuButton->roundness = 0.0f;
    backToMenuButton->onMouseHoverStart = std::bind(OnMouseHoverStart, backToMenuButton);
    backToMenuButton->onMouseHoverEnd = std::bind(OnMouseHoverEnd, backToMenuButton);
    backToMenuButton->onMouseClick = std::bind(OnBackToMenuButton, this);

    m_ApplyEntity = scene.CreateEntity("BackToMenuEntity");
    m_ApplyEntity->SetActive(false);
    UI::Button* applyButton = m_ApplyEntity->AddComponent<UI::Button>();
    applyButton->SetFont(font);
    applyButton->hasBackground = true;
    applyButton->units = UI::Units::Percents;
    applyButton->horizontalAlignment = UI::Alignment::Center;
    applyButton->size = Math::Vec2(0.12f, 0.09f);
    applyButton->text = "Apply";
    applyButton->textColor = Color(255, 255, 255, 255);
    applyButton->position = Math::Vec3(0.6f, 0.77f, 0.0f);
    applyButton->backgroundColor = Color(0, 0, 80, 255);
    applyButton->roundness = 0.0f;
    applyButton->onMouseHoverStart = std::bind(OnMouseHoverStart, applyButton);
    applyButton->onMouseHoverEnd = std::bind(OnMouseHoverEnd, applyButton);
    applyButton->onMouseClick = std::bind(OnApplyButton, this);

    m_ResolutionEntity = scene.CreateEntity("ResolutionEntity");
    m_ResolutionEntity->SetActive(false);
    m_ResolutionLabel = m_ResolutionEntity->AddComponent<UI::Label>();
    m_ResolutionLabel->SetFont(font);
    m_ResolutionLabel->hasBackground = true;
    m_ResolutionLabel->units = UI::Units::Percents;
    m_ResolutionLabel->horizontalAlignment = UI::Alignment::Center;
    m_ResolutionLabel->size = Math::Vec2(0.22f, 0.09f);
    m_ResolutionLabel->text = m_ResolutionStrings[s_CurrentResolution];
    m_ResolutionLabel->backgroundColor = Color(0, 0, 80, 255);
    m_ResolutionLabel->textColor = Color(255, 255, 255, 255);
    m_ResolutionLabel->position = Math::Vec3(0.5f, 0.3f, 0.0f);
    m_ResolutionLabel->roundness = 0.0f;

    i32 width = g_Window->GetWidth();
    i32 height = g_Window->GetHeight();
    Entity* topCamera = scene.CreateEntity("Camera");

    Entity* perspectiveCamera = scene.CreateEntity("Camera");
    CameraComponent* camera = perspectiveCamera->AddComponent<CameraComponent>();
    camera->SetProjection(Math::Perspective(60.0f, (f32)(width)/height, 0.1f, 100.0f));
    camera->transform.SetPosition({0.0f, 10.0f, 16.0f});
    camera->transform.SetRotation(Math::Quat::Euler(-45.0f, 0.0f, 0.0f));

    m_CenterEntity = scene.CreateEntity("CenterEntity");
    camera->transform.SetParent(&m_CenterEntity->transform);
    CameraComponent::s_MainCamera = camera;

    // Floor
    Entity* floor1 = scene.CreateEntity("Floor");
    RenderComponent* floor_rc1 = 
        floor1->AddComponent<RenderComponent>(
            g_ResourceManager->GetResource<Mesh>("Box"), 
            g_ResourceManager->GetResource<Material>("FloorMaterial")
        );
    floor1->transform.SetPosition(0.0f, -1.0f, 0.0f);
    floor1->transform.SetScale(40.0f, 0.1f, 40.0f);

    // Main Lights
    Entity* mainSpotLight = scene.CreateEntity("Spotlight");
    MainLight* spotLight = mainSpotLight->AddComponent<MainLight>(
                               Math::Vec3(0.0f, 15.0f, 0.0f),
                               Math::Vec3(700.0f)
                           );
    
    Entity* magentaPointLight = scene.CreateEntity("RedPointLight");
    CornerLight* magentaLight = magentaPointLight->AddComponent<CornerLight>(
                                    Math::Vec3(-15.0f, 5.0f, -15.0f), 
                                    Math::Vec3(400/Math::Sqrt(2), 0.0f, 400/Math::Sqrt(2))
                                );

    Entity* greenPointLight = scene.CreateEntity("GreenPointLight");
    CornerLight* greenLight = greenPointLight->AddComponent<CornerLight>(
                                  Math::Vec3(-15.0f, 5.0f, 15.0f), 
                                  Math::Vec3(0.0f, 400.0f, 0.0f)
                              );

    Entity* bluePointLight = scene.CreateEntity("BluePointLight");
    CornerLight* blueLight = bluePointLight->AddComponent<CornerLight>(
                                 Math::Vec3(15.0f, 5.0f, -15.0f), 
                                 Math::Vec3(0.0f, 0.0f, 400.0f)
                             );


    Entity* yellowPointLight = scene.CreateEntity("YellowPointLight");
    CornerLight* yellowLight = yellowPointLight->AddComponent<CornerLight>(
                                   Math::Vec3(15.0f, 5.0f, 15.0f),
                                   Math::Vec3(400/Math::Sqrt(2), 400/Math::Sqrt(2), 0.0f) 
                               );

    Entity* tower = scene.CreateEntity("Tower");
    tower->AddComponent<Tower>();
}


void MainMenu::OnUpdate(f32 deltaTime) {
    m_CenterEntity->transform.Rotate(0.0f, deltaTime*2.0f, 0.0f);
}