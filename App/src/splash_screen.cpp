#include "game.h"
#include "splash_screen.h"

using namespace Fly;

SplashScreen::SplashScreen(Entity& entity, Game* game): NativeScript(entity), m_Game(game) {
    Font* font = g_ResourceManager->GetResource<Font>("Assets/Fonts/arial.ttf120", 120.0f);

    Entity* firstLine = scene.CreateEntity("SplashFirstLine");
    m_FirstLine = firstLine->AddComponent<UI::Label>();
    m_FirstLine->SetFont(font);
    m_FirstLine->hasBackground = false;
    m_FirstLine->units = UI::Units::Percents;
    m_FirstLine->horizontalAlignment = UI::Alignment::Center;
    m_FirstLine->size = Math::Vec2(0.2f, 0.05f);
    m_FirstLine->text = "PA199 project";
    m_FirstLine->textColor = Color(0, 0, 0, 255);
    m_FirstLine->position = Math::Vec3(0.5f, 0.3f, 0.0f);    

    Entity* secondLine = scene.CreateEntity("SplashSecondLine");
    m_SecondLine = secondLine->AddComponent<UI::Label>();
    m_SecondLine->SetFont(font);
    m_SecondLine->hasBackground = false;
    m_SecondLine->units = UI::Units::Percents;
    m_SecondLine->horizontalAlignment = UI::Alignment::Center;
    m_SecondLine->size = Math::Vec2(0.2f, 0.05f);
    m_SecondLine->text = "by";
    m_SecondLine->textColor = Color(0, 0, 0, 255);
    m_SecondLine->position = Math::Vec3(0.5f, 0.5f, 0.0f);

    Entity* thirdLine = scene.CreateEntity("SplashThirdLine");
    m_ThirdLine = thirdLine->AddComponent<UI::Label>();
    m_ThirdLine->SetFont(font);
    m_ThirdLine->hasBackground = false;
    m_ThirdLine->units = UI::Units::Percents;
    m_ThirdLine->horizontalAlignment = UI::Alignment::Center;
    m_ThirdLine->size = Math::Vec2(0.2f, 0.05f);
    m_ThirdLine->text = "Sergei Borzov";
    m_ThirdLine->textColor = Color(0, 0, 0, 255);
    m_ThirdLine->position = Math::Vec3(0.5f, 0.7f, 0.0f);
}

void SplashScreen::OnUpdate(f32 deltaTime) {
    m_CurrentTime += deltaTime;
    if (m_CurrentTime < 2.0f) {
        u32 c = Math::Clamp((u32)(255.0f*(1.0f - (2.0f - m_CurrentTime)/2.0f)), 0u, 255u);
        m_FirstLine->textColor = Color(c, c, c, 255);
        m_SecondLine->textColor = Color(c, c, c, 255);
        m_ThirdLine->textColor = Color(c, c, c, 255);
    }
    else {
        m_FirstLine->textColor = Color(255, 255, 255, 255);
        m_SecondLine->textColor = Color(255, 255, 255, 255);
        m_ThirdLine->textColor = Color(255, 255, 255, 255);

        m_Game->LoadMainMenu();
    }
}