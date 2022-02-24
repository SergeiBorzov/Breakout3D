#ifndef BALL_H
#define BALL_H

#include "fly.h"

class GameManager;
class Ball: public Fly::NativeScript {
public:
    Ball(Fly::Entity& entity, GameManager* gameManager): 
        Fly::NativeScript(entity), 
        m_GameManager(gameManager) 
    {
    }

    virtual void OnStart() override;

    void Release(const Math::Vec3& velocity);
    void Withhold();

    virtual void OnUpdate(f32 deltaTime) override;
    virtual void OnFixedUpdate(f32 fixedDeltaTime) override;
    virtual void OnCollisionEnter(Fly::Collider& other) override;
    virtual void OnTriggerEnter(Fly::Collider& other) override;

    void IncreaseSpeed(f32 amount);
    void DecreaseSpeed(f32 amount);

    void StartTemporaryPierce();

    virtual ~Ball() {}
private:
    void NextColor();

    Fly::RenderComponent* m_RenderComponent = nullptr;
    Fly::RigidBody* m_RigidBody = nullptr;
    Fly::SphereCollider* m_Collider = nullptr;

    GameManager* m_GameManager;
    //f32 m_MapRadius = 20.0f;
    //f32 m_BallRadius = 0.5f;
    f32 m_MaxSpeedDiff = 0.15f;
    f32 m_MagnitudeLastFrame = 0.0f;
    f32 m_PierceTimer = 0.0f;
    f32 m_BlinkTime = 0.05f;
    f32 m_BlinkTimer = 0.0f;

    bool m_IsRedColor = true;
    bool m_IsPiercing = false;
    bool m_IsBlinking = false;
};

#endif /* End of BALL_H */