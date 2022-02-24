#include "pause_menu.h"
#include "game.h"

using namespace Fly;

// Callbacks
static void OnMouseHoverStart(UI::Button* button) {
    button->textColor = Color(255, 0, 0, 255);
}

static void OnMouseHoverEnd(UI::Button* button) {
    button->textColor = Color(255, 255, 255, 255);
}

static void OnResumeButton(Game* game) {
    game->ResumeGame();
}

static void OnMainMenuButton(Game* game) {
    game->ResetMainMenu();
}

static void OnExitButton(Game* game) {
    game->CloseGame();
}

PauseMenu::PauseMenu(Fly::Entity& entity, Game* game): Fly::NativeScript(entity) {
    m_Game = game;

    Font* font = g_ResourceManager->GetResource<Font>("Assets/Fonts/Alphakind.ttf48", 48.0f);

    m_ResumeEntity = scene.CreateEntity("ResumeButton");
    m_ResumeEntity->SetActive(false);

    UI::Button* resumeButton = m_ResumeEntity->AddComponent<UI::Button>();
    resumeButton->SetFont(font);
    resumeButton->hasBackground = true;
    resumeButton->units = UI::Units::Percents;
    resumeButton->horizontalAlignment = UI::Alignment::Center;
    resumeButton->size = Math::Vec2(0.22f, 0.09f);
    resumeButton->text = "Resume";
    resumeButton->textColor = Color(255, 255, 255, 255);
    resumeButton->position = Math::Vec3(0.5f, 0.3f, 0.0f);
    resumeButton->onMouseHoverStart = std::bind(OnMouseHoverStart, resumeButton);
    resumeButton->onMouseHoverEnd = std::bind(OnMouseHoverEnd, resumeButton);
    resumeButton->onMouseClick = std::bind(OnResumeButton, m_Game);
    resumeButton->roundness = 15.0f;

    m_MainMenuEntity = scene.CreateEntity("MainMenuButton");
    m_MainMenuEntity->SetActive(false);
    UI::Button* mainMenuButton = m_MainMenuEntity->AddComponent<UI::Button>();
    mainMenuButton->SetFont(font);
    mainMenuButton->hasBackground = true;
    mainMenuButton->units = UI::Units::Percents;
    mainMenuButton->horizontalAlignment = UI::Alignment::Center;
    mainMenuButton->size =Math::Vec2(0.22f, 0.09f);
    mainMenuButton->text = "Main menu";
    mainMenuButton->textColor = Color(255, 255, 255, 255);
    mainMenuButton->position = Math::Vec3(0.5f, 0.5f, 0.0f);
    mainMenuButton->onMouseHoverStart = std::bind(OnMouseHoverStart, mainMenuButton);
    mainMenuButton->onMouseHoverEnd = std::bind(OnMouseHoverEnd, mainMenuButton);
    mainMenuButton->onMouseClick = std::bind(OnMainMenuButton, m_Game);
    mainMenuButton->roundness = 15.0f;

    m_ExitEntity = scene.CreateEntity("ExitButton");
    m_ExitEntity->SetActive(false);
    UI::Button* exitButton = m_ExitEntity->AddComponent<UI::Button>();
    exitButton->SetFont(font);
    exitButton->hasBackground = true;
    exitButton->units = UI::Units::Percents;
    exitButton->horizontalAlignment = UI::Alignment::Center;
    exitButton->size = Math::Vec2(0.22f, 0.09f);
    exitButton->text = "Quit";
    exitButton->textColor = Color(255, 255, 255, 255);
    exitButton->position = Math::Vec3(0.5f, 0.7f, 0.0f);
    exitButton->onMouseHoverStart = std::bind(OnMouseHoverStart, exitButton);
    exitButton->onMouseHoverEnd = std::bind(OnMouseHoverEnd, exitButton);
    exitButton->onMouseClick = std::bind(OnExitButton, m_Game);
    exitButton->roundness = 15.0f;
}

void PauseMenu::Show() {
    m_ResumeEntity->SetActive(true);
    m_MainMenuEntity->SetActive(true);
    m_ExitEntity->SetActive(true);
}

void PauseMenu::Hide() {
    m_ResumeEntity->SetActive(false);
    m_MainMenuEntity->SetActive(false);
    m_ExitEntity->SetActive(false);
}