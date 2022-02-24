#ifndef SCORING_SYSTEM_H
#define SCORING_SYSTEM_H

#include "fly.h"

class ScoringSystem: public Fly::NativeScript {
public:
    static ScoringSystem* GetInstance() {
        return instance;
    }

    ScoringSystem(Fly::Entity& entity): Fly::NativeScript(entity) {}
    void AddPoints(u32 points);
    virtual void OnStart() override;

    virtual ~ScoringSystem() {}
private:
    Fly::UI::Label* m_ScoreLabel = nullptr;
    u32 m_ScoredPoints = 0;

    static ScoringSystem* instance;
};

#endif /* End of SCORING_SYSTEM_H */