#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include "fly.h"

class Game;

class SplashScreen: public Fly::NativeScript {
public:
    SplashScreen(Fly::Entity& entity, Game* game);
    virtual void OnUpdate(f32 deltaTime) override;

    virtual ~SplashScreen() {}
private:
    Fly::UI::Label* m_FirstLine = nullptr;
    Fly::UI::Label* m_SecondLine = nullptr;
    Fly::UI::Label* m_ThirdLine = nullptr;
    Game* m_Game;
    f32 m_CurrentTime = 0.0f;
};

#endif