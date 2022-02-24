#ifndef BRICK_H
#define BRICK_H

#include "fly.h"

class Tower;

class Brick: public Fly::NativeScript {
public:
    Brick(Fly::Entity& entity, Tower* tower, i32 index);

    inline Fly::Collider* GetCollider() const { return m_Collider; }
    inline bool IsReplacing() const { return m_IsReplacing; }
    inline bool IsDissolving() const { return m_IsDissolving; }
    inline bool IsAppearing() const { return m_IsAppearing; }
    inline Fly::PBRMaterial* GetDissolveMaterial() const { return m_DissolveMaterial; }

    virtual void OnCollisionEnter(Fly::Collider& other) override;
    virtual void OnTriggerEnter(Fly::Collider& other) override;
    virtual void OnStart() override;
    virtual void OnUpdate(f32 deltaTime) override;

    void StartReplacement(i32 index, f32 targetScale);
    void StartAppearing();

    void SetTransparency(bool value);

    virtual void Eliminate();
    
    virtual ~Brick();
protected:
    Tower* m_Tower = nullptr;
    Fly::RenderComponent* m_RenderComponent = nullptr;
    Fly::Collider* m_Collider = nullptr;
    Fly::RigidBody* m_RigidBody = nullptr;
    Fly::PBRMaterial* m_DissolveMaterial = nullptr;

    i32 m_Index = -1;
    u32 m_Value = 10;

    Math::Vec3 m_TargetPosition;
    f32 m_TargetScale = 1.0f;
    f32 m_TotalDist = 0.0f;
    bool m_IsReplacing = false;
    bool m_IsDissolving = false;
    bool m_IsAppearing = false;
    bool m_IsTransparent = false;
};

class BonusBrick: public Brick {
public:
    BonusBrick(Fly::Entity& entity, Tower* tower, i32 index);
    virtual ~BonusBrick() {}

    virtual void OnStart() override;
    virtual void OnCollisionEnter(Fly::Collider& other) override;
    virtual void OnTriggerEnter(Fly::Collider& other) override;
    virtual void Eliminate() override;
private:
    void SpawnRandomBonus();
};

class ToughBrick: public Brick {
public:
    ToughBrick(Fly::Entity& entity, Tower* tower, i32 index);
    virtual ~ToughBrick() {}

    virtual void OnStart() override;
    virtual void OnUpdate(f32 deltaTime) override;
    virtual void OnCollisionEnter(Fly::Collider& other) override;
    virtual void OnTriggerEnter(Fly::Collider& other) override;
private:
    i32 m_Lives = 2;
    bool m_Animate = false;
    f32 m_NewThreshold = 1.0f;
};

#endif /* End of BRICK_H */