#include <assert.h>

#include "scoring_system.h"
using namespace Fly;

ScoringSystem* ScoringSystem::instance = nullptr;

void ScoringSystem::OnStart() {

    //assert(instance == nullptr);
    instance = this;

    Font* font = g_ResourceManager->GetResource<Font>("Assets/Fonts/Alphakind.ttf48", 48.0f);

    m_ScoreLabel = entity.AddComponent<UI::Label>();
    m_ScoreLabel->SetFont(font);
    m_ScoreLabel->hasBackground = false;
    m_ScoreLabel->units = UI::Units::Percents;
    m_ScoreLabel->horizontalAlignment = UI::Alignment::Left;
    m_ScoreLabel->size = Math::Vec2(0.2f, 0.05f);
    m_ScoreLabel->text = "Score: " + std::to_string(m_ScoredPoints);
    m_ScoreLabel->textColor = Color(255, 255, 255, 255);
    m_ScoreLabel->position = Math::Vec3(0.9f, 0.05f, 0.0f);    
}

void ScoringSystem::AddPoints(u32 points) {
    m_ScoredPoints += points;
    m_ScoreLabel->text = "Score: " + std::to_string(m_ScoredPoints);
}