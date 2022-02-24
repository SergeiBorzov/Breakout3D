#ifndef PADDLES_H
#define PADDLES_H

#include "fly.h"

class Ball;

class Paddles: public Fly::NativeScript {
public:
    Paddles(Fly::Entity& entity);

    virtual void OnUpdate(f32 deltaTime) override;
    virtual void OnFixedUpdate(f32 fixedDeltaTime) override;

    Fly::Entity* GetMainPaddle() const { return m_Paddles[0]; }

    void IncreasePaddleSize();
    void DecreasePaddleSize();

    inline void SetActive(bool value) { active = value; }

    virtual ~Paddles() {}
private:
    Fly::Entity* m_Paddles[3];
    Fly::RenderComponent* m_RenderComponents[3];
    Fly::CylinderBrickCollider* m_PaddleColliders[3];
    Fly::Mesh* m_PaddleMeshes[9];
    //Ball* m_Ball;
    //Fly::PBRMaterial* paddleMaterial = nullptr;
    //Fly::Mesh* paddleMesh = nullptr;
    f32 m_DefaultPaddleBigRadius = 15.0f;
    f32 m_DefaultPaddleSmallRadius = 14.0f;
    f32 m_DefaultPaddleHeight = 1.0f;
    f32 m_DefaultPaddleAngle = 45.0f;
    f32 m_Speed = 90.0f;
    i32 m_PaddleCount = 3;
    i32 m_PaddleSize = 4;

    bool active = true;
};
#endif /* End of PADDLES_H */