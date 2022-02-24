#ifndef MAIN_LIGHT_H
#define MAIN_LIGHT_H

#include "fly.h"

class MainLight: public Fly::NativeScript {
public:
    MainLight(Fly::Entity& entity, const Math::Vec3& position, const Math::Vec3& intensity): 
        Fly::NativeScript(entity),
        m_Position(position), 
        m_Intensity(intensity) {}
    virtual void OnStart() override;
    virtual void OnUpdate(f32 deltaTime) override;

    void StartBlinking();
    inline void SetIntensity(const Math::Vec3& intensity) { m_LightComponent->intensity = intensity; }

    virtual ~MainLight() {}
private:
    Math::Vec3 NextIntensity();

    Math::Vec3 m_Position;
    Math::Vec3 m_Intensity;

    Fly::LightComponent* m_LightComponent;
    f32 m_CurrentTime = 0.0f;
    bool m_Blinking = false;
    bool m_LightOn = true;

    f32 m_TimeMultiplier = 1.0f;
    u32 m_NumBlinks = 0;
};

#endif /* MAIN_LIGHT_H */