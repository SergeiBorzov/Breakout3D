#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "fly.h"

class Game;

struct Resolution {
    i32 width;
    i32 height;
};

class MainMenu: public Fly::NativeScript {
public:
    MainMenu(Fly::Entity& entity, Game* game);
    virtual void OnStart() override;
    virtual void OnUpdate(f32 deltaTime) override;

    void ShowSettings();
    void HideSettings();
    void HigherResolution();
    void LowerResolution();
    void FullScreenButton();

    void ApplyChanges();
    void DiscardChanges();

    virtual ~MainMenu() {}
private:
    Resolution m_Resolutions[8] = {{1280, 720}, {1280, 800}, {1366, 768}, {1440, 900}, {1600, 900}, {1680, 1050}, {1920, 1080}, {1920, 1200}};
    std::string m_ResolutionStrings[8] = {"1280x720", "1280x800", "1366x768", "1440x900", "1600x900", "1680x1050", "1920x1080", "1920x1200"};
    Game* m_Game;
    Fly::Entity* m_CenterEntity;
    Fly::Entity* m_NewGameButtonEntity;
    Fly::Entity* m_SettingsButtonEntity;
    Fly::Entity* m_QuitButtonEntity;
    Fly::Entity* m_BackgroundEntity;
    Fly::Entity* m_ResolutionEntity;
    Fly::Entity* m_LowerResolutionEntity;
    Fly::Entity* m_HigherResolutionEntity;
    Fly::Entity* m_BackToMenuEntity;
    Fly::Entity* m_ApplyEntity;
    Fly::Entity* m_FullScreenEntity;

    Fly::UI::Label* m_ResolutionLabel;
    Fly::UI::Button* m_FullScreenButton;
    Fly::UI::Button* m_NewGameButton;

    static i32 s_CurrentResolution;
    static bool s_FullScreen;
    static i32 s_PrevResolution;
    static bool s_PrevFullScreen;
};

#endif /* End of MAIN_MENU_H */